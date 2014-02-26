/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#ifndef CAPU_VECTOR_H
#define CAPU_VECTOR_H

#include <capu/container/Array.h>

namespace capu
{
    /**
     * Basic Vector implementation
     * The internal memory is doubled if the vector is full
     */
    template<typename T>
    class Vector
    {
    private:

        /**
         * Iterator for Vector
         */
        template<typename TYPE>
        class InternalIterator
        {
        public:
            friend class Vector<T>;

            /**
             * Sets the current position to the next element
             */
            void operator++()
            {
                ++m_current;
            }


            /**
             * Compares two Iterators if their internal position is the same
             * @param other Iterator to compare with
             * @return true if Iterators don't point to the same data, false otherwise
             */
            bool_t operator!=(const InternalIterator<TYPE>& other) const
            {
                return m_current != other.m_current;
            }

            /**
             * Dereferences the iterator to access the internal data
             * return a reference to the current data
             */
            TYPE& operator*()
            {
                return *m_current;
            }

            /**
             * Autocast to the pointer of the current element
             * @return the pointer to the current element
             */
            operator TYPE*() const
            {
                return m_current;
            }

            /**
             * Allows access to methods and member of the internal data
             * @return the pointer to the current data
             */
            TYPE* operator->()
            {
                return &** this;
            }

            /**
             * Checks if the pointer of the other iterator is bigger than my own pointer
             * @param other InternalIterator to check with
             * @return true if other pointer is bigger than my one one, false otherwise
             */
            bool_t operator <(const InternalIterator<TYPE>& other)
            {
                return m_current < other.m_current;
            }

            /**
             * Checks if the pointer of the other iterator is smaller than my own pointer
             * @param other InternalIterator to check with
             * @return true if other pointer is smaller than my one one, false otherwise
             */
            bool_t operator >(const InternalIterator<TYPE>& other)
            {
                return m_current > other.m_current;
            }

            /**
             * Adds the given value to the internal pointer and returns a new iterator
             * @param value to add to the internal pointer
             * @return InternalIterator with the new pointer
             */
            InternalIterator operator+(const uint_t value) const
            {
                return InternalIterator(m_current + value);
            }

            /**
             * Substracts the given value to the internal pointer and returns a new iterator
             * @param value to substract from the internal pointer
             * @return InternalIterator with the new pointer
             */
            InternalIterator operator-(const uint_t value) const
            {
                return InternalIterator(m_current - value);
            }

        protected:
        private:
            /**
             * Creates a Iterator for the Vector
             * @param start pointer for the iterator
             */
            InternalIterator(TYPE* start)
                : m_current(start)
            {

            }

            /**
             * Pointer to the current data
             */
            TYPE* m_current;
        };

    public:

        typedef InternalIterator<T> Iterator;
        typedef InternalIterator<const T> ConstIterator;

        /**
         * Creates a new vector. The initial capacity is 16
         */
        Vector();

        /**
         * Creates a new Vector with a given initial capacity
         * @param initialCapacity for the Vector
         */
        Vector(const uint_t initialCapacity);

        /**
         * Initializes the Vector with the given capacity and sets all elements
         * to the given value
         * @param initialCapacity for the Vector
         * @param value to set for all elements
         */
        Vector(const uint_t initialCapacity, const T& value);

        /**
         * Adds an Element to the end of the vector
         * @param reference to the value to add
         */
        status_t push_back(const T& value);

        /**
         * Returns the current size of the Vector
         * @return size of the current Vector
         */
        uint_t size() const;

        /**
         * Resizes the vector to the given size
         * The currently containing data of the vector will
         * be still available. If the new size is smaller than the
         * old size only the elements which fit into the new size are
         * available after resizing
         * @param new size of the Vector
         */
        void resize(const uint_t size);

        /**
         * Removes all elements from the Vector
         */
        void clear();

        /**
         * Operator to access internal data with index
         * @param index of the element to access
         * @return reference to the element
         */
        T& operator[](const uint_t index) const;

        /**
         * Returns a new Iterator to the start of the Vector
         * @return a new Iterator to the start of the Vector
         * @{
         */
        Iterator begin();
        ConstIterator begin() const;
        /**
         * @}
         */

        /**
         * Returns a new Iterator to the end of the Vector
         * @return a new Iterator to the end of the Vector
         * @{
         */
        Iterator end();
        ConstIterator end() const;
        /**
        * @}
        */
		
    protected:
    private:
        /**
         * Internal Array to store the elements
         */
        Array<T> m_data;

        /**
         * Iterator which points to the start of the data
         */
        Iterator m_start;

        /**
         * Iterator which points to one after the end of the data
         */
        Iterator m_end;

        /**
         * Internal method to double the current memory
         */
        void grow();
    };

    template<typename T>
    inline
    Vector<T>::Vector()
        : m_data(16)
        , m_start(m_data.getRawData())
        , m_end(m_data.getRawData())
    {

    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint_t initialCapacity, const T& value)
        : m_data(initialCapacity)
        , m_start(m_data.getRawData())
        , m_end(m_data.getRawData() + m_data.size())
    {
        m_data.set(value);
    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint_t initialSize)
        : m_data(initialSize)
        , m_start(m_data.getRawData())
        , m_end(m_data.getRawData())
    {

    }

    template<typename T>
    inline
    status_t
    Vector<T>::push_back(const T& value)
    {
        status_t status = CAPU_OK;
        if (size() == m_data.size())
        {
            /*status = */
            // todo: should check status of grow here
            grow();
        }

        (*m_end) = value;
        ++m_end;

        return status;
    }

    template<typename T>
    inline
    void
    Vector<T>::clear()
    {
        m_end = m_start;
    }

    template<typename T>
    inline
    void
    Vector<T>::grow()
    {
        Array<T> tmpArray(m_data.size()  * 2);
        m_data.swap(tmpArray);

        m_start = m_data.getRawData();
        m_end   = m_data.getRawData() + tmpArray.size();

        Memory::CopyObject(m_start.m_current, tmpArray.getRawData(), tmpArray.size());
    }

    template<typename T>
    inline
    void
    Vector<T>::resize(const uint_t size)
    {
        Array<T> tmpArray(size);
        const uint_t mySize = Vector<T>::size();
        const uint_t sizeToCopy = size < mySize ? size : mySize;

        tmpArray.copy(m_start.m_current, sizeToCopy);
        m_data.swap(tmpArray);

        m_start = m_data.getRawData();
        m_end   = m_data.getRawData() + mySize;
    }

    template<typename T>
    inline
    T&
    Vector<T>::operator[](const uint_t index) const
    {
        return *(m_start + index);
    }

    template<typename T>
    inline
    uint_t
    Vector<T>::size() const
    {
        return m_end.m_current - m_start.m_current;
    }

    template<typename T>
    inline
    typename Vector<T>::Iterator
    Vector<T>::begin()
    {
        return m_start;
    }

    template<typename T>
    inline
    typename Vector<T>::ConstIterator
    Vector<T>::begin() const
    {
        return ConstIterator(m_start.m_current);
    }

    template<typename T>
    inline
    typename Vector<T>::Iterator
    Vector<T>::end()
    {
        return Iterator(m_end);
    }

    template<typename T>
    inline
    typename Vector<T>::ConstIterator
    Vector<T>::end() const
    {
        return ConstIterator(m_end);
    }

}

#endif // CAPU_VECTOR_H
