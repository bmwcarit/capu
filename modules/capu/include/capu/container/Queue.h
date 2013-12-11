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

#ifndef CAPU_QUEUE_H
#define CAPU_QUEUE_H

#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/List.h"
#include "capu/util/StaticAllocator.h"

namespace capu
{
    /**
     * Queue. A container to which one can push and pop objects to and from the same side. The operations are not thread safe.
     */
    template <class T, class A = Allocator<GenericListNode<T> >, class C = Comparator>
    class Queue : private List<T, A, C>
    {
    public:
        /**
         * Default Constructor
         */
        Queue();

        /**
         * Destructor
         */
        virtual ~Queue();

        /**
         * Push element to the end of the queue
         * @param element element that will be pushed
         * @return CAPU_ENO_MEMORY if allocation of element is failed
         *         CAPU_OK if the element is successfully added
         */
        status_t push(const T& element);

        /**
         * Pop an element from the queue.
         * @param element The element to which the removed value should get copied. Default value is 0.
         * @return CAPU_OK if an element was removed from the queue.
         *         CAPU_EINVAL if the queue was empty.
         */
        status_t pop(T* element = 0);

        /**
         * Peeks an element from the queue but does not remove it.
         * @param element The element to which the peeked value should get copied.
         * @return CAPU_OK if an element was peeked from the queue.
         *         CAPU_EINVAL if the queue was empty.
         */
        status_t peek(T& element) const;

        /**
         * Returns a reference to the first element in the queue
         *
         * @return reference to first element
         */
        T& front();

        /**
         * Returns a const reference to the first element in the queue
         *
         * @return const reference to first element
         */
        const T& front() const;

        /**
         * Returns a reference to the last element in the queue
         *
         * @return reference to the last element in the queue
         */
        T& back();

        /**
         * Returns a const reference to the last element in the queue
         *
         * @return const reference to the last element in the queue
         */
        const T& back() const;

        /**
         * Pop all elements in correct order out of the queue.
         * @param list the list in which the elements are added.
         * @return CAPU_OK if all elements were added to the queue, CAPU_ERROR otherwise.
         */
        status_t popAll(List<T, A, C>& list);

        /**
         * Return size of the queue
         * @return return the size of queue
         */
        uint_t size() const;

        /**
         * Check the queue is empty or not
         * @return true if empty
         *         false otherwise
         */
        bool_t empty() const;

        /**
         * Remove all elements from queue
         */
        void clear();
    };

    /*
     * Implementation queue
     */

    template <class T, class A, class C>
    Queue<T, A, C>::Queue()
    {
    }

    template <class T, class A, class C>
    Queue<T, A, C>::~Queue()
    {
    }

    template <class T, class A, class C>
    inline T& Queue<T, A, C>::front()
    {
        return List<T, A, C>::front();
    }

    template <class T, class A, class C>
    inline const T& Queue<T, A, C>::front() const
    {
        return List<T, A, C>::front();
    }

    template <class T, class A, class C>
    inline T& Queue<T, A, C>::back()
    {
        return List<T, A, C>::back();
    }

    template <class T, class A, class C>
    inline const T& Queue<T, A, C>::back() const
    {
        return List<T, A, C>::back();
    }

    template <class T, class A, class C>
    inline status_t Queue<T, A, C>::peek(T& element) const
    {
        if (empty())
        {
            return CAPU_EINVAL;
        }
        element = List<T, A, C>::front();
        return CAPU_OK;
    }

    template <class T, class A, class C>
    inline status_t Queue<T, A, C>::pop(T* element)
    {
        return List<T, A, C>::erase(0, element);
    }

    template <class T, class A, class C>
    inline status_t Queue<T, A, C>::popAll(List<T, A, C>& list)
    {
        T current;
        while (pop(&current) == CAPU_OK)
        {
            if (list.insert(current) != CAPU_OK)
            {
                return CAPU_ERROR;
            }
        }
        return CAPU_OK;
    }

    template <class T, class A, class C>
    inline uint_t Queue<T, A, C>::size() const
    {
        return List<T, A, C>::size();
    }

    template <class T, class A, class C>
    inline void Queue<T, A, C>::clear()
    {
        List<T, A, C>::clear();
    }

    template <class T, class A, class C>
    inline bool_t Queue<T, A, C>::empty() const
    {
        return List<T, A, C>::isEmpty();
    }

    template <class T, class A, class C>
    inline status_t Queue<T, A, C>::push(const T& element)
    {
        return List<T, A, C>::insert(element);
    }

    /**
     * A queue class with a defined amount of static memory.
     */
     template<typename T, uint32_t COUNT, class C = Comparator>
     class StaticQueue: public Queue<T, StaticAllocator<GenericListNode<T>, COUNT>, C>
     {
     };
}

#endif /* CAPU_QUEUE_H */
