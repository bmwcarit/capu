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
#include "capu/util/StaticAllocator.h"

namespace capu
{
    /**
     * Implements a non-synchronized stack with the common push and pop operations (LIFO - last-in, first-out).
     */
    template <class T, class A = Allocator<GenericListNode<T> >, class C = Comparator>
    class Stack : private List<T, A, C>
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
        bool_t isEmpty() const;

        /**
         * Remove all elements from the stack.
         */
        void clear();
    };

    template <class T, class A, class C>
    inline Stack<T, A, C>::Stack()
    {
    }

    template <class T, class A, class C>
    inline Stack<T, A, C>::~Stack()
    {
    }

    template <class T, class A, class C>
    status_t Stack<T, A, C>::pop(T* element)
    {
        return List<T, A, C>::erase(0, element);
    }

    template <class T, class A, class C>
    int_t Stack<T, A, C>::size() const
    {
        return List<T, A, C>::size();
    }

    template <class T, class A, class C>
    void Stack<T, A, C>::clear()
    {
        List<T, A, C>::clear();
    }

    template <class T, class A, class C>
    bool_t Stack<T, A, C>::isEmpty() const
    {
        return List<T, A, C>::isEmpty();
    }

    template <class T, class A, class C>
    status_t Stack<T, A, C>::push(const T& element)
    {
        return List<T, A, C>::insert(0, element);
    }

    template <class T, class A, class C>
    status_t Stack<T, A, C>::peek(T& element) const
    {
        if (isEmpty())
        {
            return CAPU_EINVAL;
        }
        element = List<T, A, C>::front();
        return CAPU_OK;
    }

    /**
     * A stack class with a defined amount of static memory.
     */
     template<typename T, uint32_t COUNT, class C = Comparator>
     class StaticStack: public Stack<T, StaticAllocator<GenericListNode<T>, COUNT>, C>
     {
     };
}

#endif // CAPU_STACK_H
