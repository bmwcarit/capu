/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAPU_BLOCKINGQUEUE_H
#define CAPU_BLOCKINGQUEUE_H

#include "capu/container/Queue.h"
#include "capu/os/Semaphore.h"
#include "capu/os/Mutex.h"
#include "capu/util/ScopedLock.h"
#include "capu/util/StaticAllocator.h"

namespace capu
{
    /**
     * Queue for concurrent access. All methods are threadsafe.
     */
    template <class T, class A = Allocator<GenericListNode<T> >, class C = Comparator>
    class BlockingQueue: private Queue<T, A, C>
    {
    public:
        virtual ~BlockingQueue();

        /**
         * Insert an element into the queue
         * @param element The element to insert
         * @return CAPU_OK if push was successful.
         */
        status_t push(const T& element);

        /**
         * Remove and return an element from the queue
         * @param element Pointer which will receive the removed element
         * @param timeoutMillis Milliseconds to wait blocking for removal
         * @return CAPU_OK if removal was successfull, CAPU_ETIMEOUT if timeout occured
         */
        status_t pop(T* element = 0, const uint32_t timeoutMillis = 0);

        /**
         * Peeks an element from the queue but does not remove it.
         * @param element The element to which the peeked value should get copied.
         * @return CAPU_OK if an element was peeked from the queue.
         *         CAPU_EINVAL if the queue was empty.
         */
        status_t peek(T& element);

        /**
         * Check the queue is empty or not
         * @return true if empty
         *         false otherwise
         */
        bool_t empty();

        /**
         * Return size of the queue
         * @return return the size of queue
         */
        uint_t size();

        /**
         * Remove all elements from queue
         */
        void clear();

    private:
        Semaphore mSemaphore;
        Mutex mMutex;
    };

    template <class T, class A, class C>
    inline BlockingQueue<T, A, C>::~BlockingQueue()
    {
    }

    template <class T, class A, class C>
    inline bool_t BlockingQueue<T, A, C>::empty()
    {
        ScopedMutexLock locker(mMutex);
        return Queue<T, A, C>::empty();
    }

    template <class T, class A, class C>
    inline status_t BlockingQueue<T, A, C>::push(const T& element)
    {
        ScopedMutexLock locker(mMutex);
        status_t retVal = Queue<T, A, C>::push(element);
        mSemaphore.release();
        return retVal;
    }

    template <class T, class A, class C>
    inline status_t BlockingQueue<T, A, C>::peek(T& element)
    {
        ScopedMutexLock locker(mMutex);
        return Queue<T, A, C>::peek(element);
    }

    template <class T, class A, class C>
    inline status_t BlockingQueue<T, A, C>::pop(T* element, const uint32_t timeoutMillis)
    {
        // wait outside the lock
        status_t retVal = mSemaphore.tryAquire(timeoutMillis);
        if (retVal != CAPU_OK)
        {
            return retVal;
        }
        ScopedMutexLock locker(mMutex);
        return Queue<T, A, C>::pop(element);
    }

    template <class T, class A, class C>
    inline void BlockingQueue<T, A, C>::clear()
    {
        ScopedMutexLock locker(mMutex);
        Queue<T, A, C>::clear();
    }

    template <class T, class A, class C>
    inline uint_t BlockingQueue<T, A, C>::size()
    {
        ScopedMutexLock locker(mMutex);
        return Queue<T, A, C>::size();
    }

    /**
     * A blocking queue class with a defined amount of static memory.
     */
     template<typename T, uint32_t COUNT, class C = Comparator>
     class StaticBlockingQueue: public BlockingQueue<T, StaticAllocator<GenericListNode<T>, COUNT>, C>
     {
     };
}

#endif // CAPU_BLOCKINGQUEUE_H
