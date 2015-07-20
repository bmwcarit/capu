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

#ifndef CAPU_STACK_H
#define CAPU_STACK_H

#include "capu/container/List.h"

namespace capu
{
    /**
     * Implements a non-synchronized stack with the common push and pop operations (LIFO - last-in, first-out).
     */
    template <typename T>
    class Stack : private List<T>
    {
    public:
        /**
         * Constructor.
         */
        Stack();

        /**
         * Destructor.
         */
        virtual ~Stack();

        /**
         * Push element to the top of the stack.
         * @param element The element that will be pushed.
         * @return CAPU_OK if the element is successfully added.
         */
        status_t push(const T& element);

        /**
         * Pops an element from the stack.
         * @param element The element to which the removed value should get copied. Default value is 0.
         * @return CAPU_OK if an element was removed from the stack.
         *         CAPU_EINVAL if the stack was empty.
         */
        status_t pop(T* element = 0);

        /**
        * Peeks the first element from the stack. Does not remove the element.
        * @param element The element to which the value will get copied
        * @return CAPU_OK if peek was successful.
        */
        status_t peek(T& element) const;

        /**
         * Returns the size of the stack.
         * @return The size of the stack.
         */
        int_t size() const;

        /**
         * Check the stack is empty or not.
         * @return true if empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * Remove all elements from the stack.
         */
        void clear();
    };

    template <typename T>
    inline Stack<T>::Stack()
    {
    }

    template <typename T>
    inline Stack<T>::~Stack()
    {
    }

    template <typename T>
    status_t Stack<T>::pop(T* element)
    {
        return List<T>::erase(0, element);
    }

    template <typename T>
    int_t Stack<T>::size() const
    {
        return List<T>::size();
    }

    template <typename T>
    void Stack<T>::clear()
    {
        List<T>::clear();
    }

    template <typename T>
    bool Stack<T>::isEmpty() const
    {
        return List<T>::isEmpty();
    }

    template <typename T>
    status_t Stack<T>::push(const T& element)
    {
        return List<T>::insert(0, element);
    }

    template <typename T>
    status_t Stack<T>::peek(T& element) const
    {
        if (isEmpty())
        {
            return CAPU_EINVAL;
        }
        element = List<T>::front();
        return CAPU_OK;
    }
}

#endif // CAPU_STACK_H
