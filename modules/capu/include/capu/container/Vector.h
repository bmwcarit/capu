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
    public:

        /**
         * Iterator for Vector
         */
        class Iterator
        {
        public:
            friend class Vector<T>;

            /**
             * Sets the current position to the next element
             */
            void operator++();

            /**
             * Compares two Iterators if their internal position is the same
             * @param other Iterator to compare with
             * @return true if Iterators don't point to the same data, false otherwise
             */
            bool_t operator!=(const Iterator& other);

            /**
             * Dereferences the iterator to access the internal data
             */
            T& operator*();

            /**
             * Allows access to methods and member of the internal data
             */
            T* operator->();
        protected:
        private:
            /**
             * Creates a Iterator for the Vector
             * @param start pointer for the iterator
             */
            Iterator(T* start);

            /**
             * Pointer to the current data
             */
            T* m_current;
        };

        /**
         * Creates a new vector. The initial capacity is 16
         */
        Vector();

        /**
         * Creates a new Vector with a given initial capacity
         * @param initialCapacity for the Vector
         */
        Vector(const uint32_t initialCapacity);

        /**
         * Initializes the Vector with the given capacity and sets all elements
         * to the given value
         * @param initialCapacity for the Vector
         * @param value to set for all elements
         */
        Vector(const uint32_t initialCapacity, const T& value);

        /**
         * Adds an Element to the end of the vector
         * @param reference to the value to add
         */
        status_t push_back(const T& value);

        /**
         * Returns the current size of the Vector
         * @return size of the current Vector
         */
        uint32_t size() const;

        /**
         * Resizes the vector to the given size
         * The currently containing data of the vector will
         * be still available. If the new size is smaller than the
         * old size only the elements which fit into the new size are
         * available after resizing
         * @param new size of the Vector
         */
        void resize(const uint32_t size);

        /**
         * Removes all elements from the Vector
         */
        void clear();

        /**
         * Operator to access internal data with index
         * @param index of the element to access
         * @return reference to the element
         */
        T& operator[](const uint32_t index) const;

        /**
         * Returns a new Iterator to the start of the Vector
         * @return a new Iterator to the start of the Vector
         */
        Iterator begin();

        /**
         * Returns a new Iterator to the end of the Vector
         * @return a new Iterator to the end of the Vector
         */
        Iterator end();

    protected:
    private:
        /**
         * Internal Array to store the elements
         */
        Array<T> m_data;

        /**
         * The current size of the vector
         */
        uint32_t m_size;

        /**
         * Internal method to double the current memory
         */
        void grow();
    };

    template<typename T>
    inline
    Vector<T>::Vector()
        : m_data(16)
        , m_size(0)
    {

    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint32_t initialCapacity, const T& value)
        : m_data(initialCapacity)
        , m_size(static_cast<uint32_t>(m_data.size()))
    {
        m_data.set(value);
    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint32_t initialSize)
        : m_data(initialSize)
        , m_size(0)
    {

    }

    template<typename T>
    inline
    status_t
    Vector<T>::push_back(const T& value)
    {
        status_t status = CAPU_OK;
        if (m_size == m_data.size())
        {
            grow();
        }

        if (CAPU_OK == status)
        {
            m_data[m_size] = value;
            ++m_size;
        }

        return status;
    }

    template<typename T>
    inline
    void
    Vector<T>::clear()
    {
        m_size = 0;
    }

    template<typename T>
    inline
    void
    Vector<T>::grow()
    {
        Array<T> tmpArray(m_data.size()  * 2);
        m_data.swap(tmpArray);

        Memory::CopyObject(m_data.getRawData(), tmpArray.getRawData(), tmpArray.size());
    }

    template<typename T>
    inline
    void
    Vector<T>::resize(const uint32_t size)
    {
        Array<T> tmpArray(size);
        const uint32_t sizeToCopy = size < m_size ? size : m_size;

        tmpArray.copy(m_data.getRawData(), sizeToCopy);
        m_data.swap(tmpArray);
    }

    template<typename T>
    inline
    T&
    Vector<T>::operator[](const uint32_t index) const
    {
        return m_data[index];
    }

    template<typename T>
    inline
    uint32_t
    Vector<T>::size() const
    {
        return m_size;
    }

    template<typename T>
    inline
    typename Vector<T>::Iterator
    Vector<T>::begin()
    {
        return Iterator(m_data.getRawData());
    }

    template<typename T>
    inline
    typename Vector<T>::Iterator
    Vector<T>::end()
    {
        return Iterator(m_data.getRawData() + m_size);
    }

    template<typename T>
    inline
    Vector<T>::Iterator::Iterator(T* start)
        : m_current(start)
    {

    }

    template<typename T>
    inline
    void
    Vector<T>::Iterator::operator++()
    {
        ++m_current;
    }

    template<typename T>
    inline
    bool_t
    Vector<T>::Iterator::operator!=(const Iterator& other)
    {
        return m_current != other.m_current;
    }

    template<typename T>
    inline
    T&
    Vector<T>::Iterator::operator*()
    {
        return *m_current;
    }

    template<typename T>
    inline
    T*
    Vector<T>::Iterator::operator->()
    {
        return &** this;
    }
}

#endif // CAPU_VECTOR_H
