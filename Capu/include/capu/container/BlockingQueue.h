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

namespace capu
{
    /**
     * Queue for concurrent access. All methods are threadsafe.
     */
    template <class T>
    class BlockingQueue: private Queue<T>
    {
    public:
        virtual ~BlockingQueue();

        /**
         * Insert an element into the queue
         * @param element The element to insert
         */
        void push(const T& element);

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
        bool empty();

    private:
        Semaphore mSemaphore;
        Mutex mMutex;
    };

    template <class T>
    inline BlockingQueue<T>::~BlockingQueue()
    {
    }

    template <class T>
    inline bool BlockingQueue<T>::empty()
    {
        ScopedMutexLock locker(mMutex);
        return Queue<T>::empty();
    }

    template <class T>
    inline void BlockingQueue<T>::push(const T& element)
    {
        ScopedMutexLock locker(mMutex);
        Queue<T>::push(element);
        mSemaphore.release();
    }

    template <class T>
    inline status_t BlockingQueue<T>::peek(T& element)
    {
        ScopedMutexLock locker(mMutex);
        return Queue<T>::peek(element);
    }

    template <class T>
    inline status_t BlockingQueue<T>::pop(T* element, const uint32_t timeoutMillis)
    {
        // wait outside the lock
        status_t retVal = mSemaphore.tryAquire(timeoutMillis);
        if (retVal != CAPU_OK)
        {
            return retVal;
        }
        ScopedMutexLock locker(mMutex);
        return Queue<T>::pop(element);
    }
}

#endif // CAPU_BLOCKINGQUEUE_H
