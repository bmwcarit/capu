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
             *  Sets the current position to the previous element
             */
            void operator--()
            {
                --m_current;
            }

            /**
             * Compares two Iterators if their internal position is the same
             * @param other Iterator to compare with
             * @return true if Iterators don't point to the same data, false otherwise
             */
            bool operator!=(const InternalIterator<TYPE>& other) const
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
            bool operator <(const InternalIterator<TYPE>& other)
            {
                return m_current < other.m_current;
            }

            /**
             * Checks if the pointer of the other iterator is smaller than my own pointer
             * @param other InternalIterator to check with
             * @return true if other pointer is smaller than my one one, false otherwise
             */
            bool operator >(const InternalIterator<TYPE>& other)
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
         * Creates a new vector with a given initial capacity
         * @param initialCapacity for the Vector
         */
        Vector(const uint_t initialCapacity);

        /**
         * Initializes the vector with the given capacity and sets all elements
         * to the given value
         * @param initialCapacity for the Vector
         * @param value to set for all elements
         */
        Vector(const uint_t initialCapacity, const T& value);

        /**
         * Initializes the vector from another vector
         */
        Vector(const Vector& other);

        /**
         * Assignment operator for vector
         * @param Vector to copy from
         * @return reference to Vector with copied data
         */
        Vector& operator=(const Vector& other);

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
         * Compares the content of two Vectors
         * @param other Vector to compare with
         * @return true if both vectors are identical
         */
        bool operator==(const Vector<T>& other) const;

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

         /**
         * remove the element in the specified index and if the element_old
         * parameter is not NULL, the removed element will be put to element_old
         *
         * NOTE: Not STL compatible
         *
         * @param index index of element that will be removed
         * @param elementOld the buffer which will keep the copy of the removed element
         * @return CAPU_EINVAL invalid index
         *         CAPU_OK if the element is successfully removed
         */
        status_t erase(const uint_t index, T* elementOld = 0);

        /**
         * Removes the element in the specified iterator position and if the element_old
         * parameter is not NULL, the removed element will be put to element_old
         * @param iterator of element that will be removed
         * @param elementOld the buffer which will keep the copy of the removed element
         * @return CAPU_EINVAL invalid iterator
         *         CAPU_OK if the element is successfully removed
         *
         */
        status_t erase(const Iterator& iterator, T* elementOld = 0);

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
    capu::Vector<T>::Vector(const Vector& other)
        : m_data(other.m_data)
        , m_start(m_data.getRawData())
        , m_end(m_data.getRawData() + other.size())
    {

    }

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
    Vector<T>&
    Vector<T>::operator=(const Vector<T>& other)
    {
        m_data  = other.m_data;
        m_start = m_data.getRawData();
        m_end   = m_data.getRawData() + other.size();

        return *this;
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
        uint_t newSize = 2 * m_data.size();
        if(0 == newSize)
        {
            newSize = 1;
        }

        Array<T> tmpArray(newSize);
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

    template<typename T>
    inline
    status_t Vector<T>::erase(const uint_t index, T* elementOld)
    {
        Iterator current = Iterator(m_start + index);
        return erase(current, elementOld);
    }

    template<typename T>
    inline
    status_t Vector<T>::erase(const Iterator& iterator, T* elementOld)
    {
        if(iterator >= m_end)
        {
            return CAPU_EINVAL;
        }

        if(0 != elementOld)
        {
            *elementOld = *iterator;
        }

        if(iterator == m_end - 1u)
        {
            --m_end;
        }
        else
        {
            Memory::Move(iterator, iterator + 1u, (m_end.m_current - (iterator.m_current + 1u)) * sizeof(T));
            --m_end;
        }

        return CAPU_OK;
    }

    template<typename T>
    inline
    bool
    Vector<T>::operator==(const Vector<T>& other) const
    {
        if (size() != other.size())
        {
            return false;
        }
        return m_data.truncatedEquals(other.m_data, size());
    }

}

#endif // CAPU_VECTOR_H
