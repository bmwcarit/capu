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

namespace capu
{
    /**
     * Queue. A container to which one can push and pop objects to and from the same side. The operations are not thread safe.
     */
    template <typename T>
    class Queue : private List<T>
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
        status_t popAll(List<T>& list);

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
        bool empty() const;

        /**
         * Remove all elements from queue
         */
        void clear();

        /**
         * Swaps the queue with another queue
         * @param other the other queue
         */
        Queue<T>& swap(Queue<T>& other);
    };


    /*
     * Implementation specialized swap for queue
     */

    template <typename T>
    void swap(Queue<T>& first, Queue<T>& second)
    {
        first.swap(second);
    }


    /*
     * Implementation queue
     */

    template <typename T>
    Queue<T>::Queue()
    {
    }

    template <typename T>
    Queue<T>::~Queue()
    {
    }

    template <typename T>
    inline Queue<T>& Queue<T>::swap(Queue< T >& other)
    {
        List<T>::swap(other);
        return *this;
    }


    template <typename T>
    inline T& Queue<T>::front()
    {
        return List<T>::front();
    }

    template <typename T>
    inline const T& Queue<T>::front() const
    {
        return List<T>::front();
    }

    template <typename T>
    inline T& Queue<T>::back()
    {
        return List<T>::back();
    }

    template <typename T>
    inline const T& Queue<T>::back() const
    {
        return List<T>::back();
    }

    template <typename T>
    inline status_t Queue<T>::peek(T& element) const
    {
        if (empty())
        {
            return CAPU_EINVAL;
        }
        element = List<T>::front();
        return CAPU_OK;
    }

    template <typename T>
    inline status_t Queue<T>::pop(T* element)
    {
        return List<T>::erase(0, element);
    }

    template <typename T>
    inline status_t Queue<T>::popAll(List<T>& list)
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

    template <typename T>
    inline uint_t Queue<T>::size() const
    {
        return List<T>::size();
    }

    template <typename T>
    inline void Queue<T>::clear()
    {
        List<T>::clear();
    }

    template <typename T>
    inline bool Queue<T>::empty() const
    {
        return List<T>::isEmpty();
    }

    template <typename T>
    inline status_t Queue<T>::push(const T& element)
    {
        return List<T>::insert(element);
    }
}

#endif /* CAPU_QUEUE_H */
