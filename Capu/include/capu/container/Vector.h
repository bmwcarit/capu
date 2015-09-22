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

#include "capu/Error.h"
#include "capu/util/Traits.h"
#include "capu/os/Memory.h"
#include "capu/util/Iterator.h"
#include "capu/util/AlgorithmRaw.h"
#include "capu/util/Algorithm.h"
#include <new>
#include <assert.h>

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
        class InternalIterator : public iterator<random_access_iterator_tag, TYPE>
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
             * Compares two Iterators if their internal position is not the same
             * @param other Iterator to compare with
             * @return true if Iterators don't point to the same data, false otherwise
             */
            bool operator!=(const InternalIterator<T>& other) const
            {
                return m_current != other.m_current;
            }

            /**
             * @copydoc operator!=(const InternalIterator<T>& other) const
             *
             **/
            bool operator!=(const InternalIterator<const T>& other) const
            {
                return m_current != other.m_current;
            }

            /**
            * Compares two Iterators if their internal position is the same
            * @param other Iterator to compare with
            * @return true if Iterators point to the same data, false otherwise
            */
            bool operator==(const InternalIterator<T>& other) const
            {
                return m_current == other.m_current;
            }

            /**
            * @copydoc operator==(const InternalIterator<T>& other) const
            *
            **/
            bool operator==(const InternalIterator<const T>& other) const
            {
                return m_current == other.m_current;
            }

            /**
             * Dereferences the iterator to access the internal data
             * return a reference to the current data
             */
            TYPE& operator*()
            {
                return *m_current;
            }

            const TYPE& operator*() const
            {
                return *m_current;
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

            /**
            * Difference of the positions the iterators point
            * @param value to substract from this iterator
            * @return number of elements between iterators. may be negative
            */
            int_t operator-(const InternalIterator<TYPE>& other) const
            {
                return m_current - other.m_current;
            }

        protected:
        private:
            /**
             * Creates a Iterator for the Vector
             * @param start pointer for the iterator
             */
            explicit InternalIterator(TYPE* start)
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
         * Creates a new vector with a given initial size
         * and initializes elements with default values
         * @param initialSize for the Vector
         */
        Vector(const uint_t initialSize);

        /**
         * Initializes the vector with the given size and sets all elements
         * to the given value
         * @param initialSize for the Vector
         * @param value to set for all elements
         */
        Vector(const uint_t initialSize, const T& value);

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
         * Destructor
         */
        ~Vector();

        /**
         * Adds an Element to the end of the vector
         * @param reference to the value to add
         */
        status_t push_back(const T& value);

        /**
         * Removes the last element of the vector.
         * Returns CAPU_ERANGE if vector was empty.
         */
        status_t pop_back();

        /**
         * Returns the current size of the Vector
         * @return size of the current Vector
         */
        uint_t size() const;

        /**
         * Returns if the vector is empty.
         */
        bool empty() const;

        /**
         * Returns the first element.
         */
        T& front();

        /**
         * Returns the first element.
         */
        const T& front() const;

        /**
         * Returns the current capacity of the vector.
         * The capacity is the total number of elements the vector can hold
         * without causing reallocation because the memory is already reserved.
         * The capacity can be influenced by use of the reserve method.
         */
        uint_t capacity() const;

        /**
         * Resizes the vector to the given size
         * If the new size is smaller than the old size
         * only the elements which fit into the new size are
         * available after resizing.
         * if new size is larger than the old size
         * the new elements are initialized with default values.
         * @param new size of the Vector
         */
        void resize(const uint_t size);

        /**
         * Reserves given capacity for the vector
         * Only has effect if new capacity is larger than the
         * old capacity.
         * @param new capacity of the Vector
         */
        void reserve(const uint_t capacity);

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
         * @param iterator iterator of element that will be removed
         * @param elementOld the buffer which will keep the copy of the removed element
         * @return CAPU_EINVAL invalid iterator
         *         CAPU_OK if the element is successfully removed
         *
         */
        status_t erase(const Iterator& iterator, T* elementOld = 0);

        /**
         * Inserts the given value at the specified iterator position
         * @param iterator iterator where value is to be inserted
         * @param value value to insert
         **/
        status_t insert(const Iterator& iterator, const T& value);

        /**
        * Inserts the given range of value at the specified iterator position
        * @param iterator iterator where value is to be inserted
        * @param first Iterator pointing to beginning of range to insert from
        * @param last Iterator pointing after last value to insert
        **/
        status_t insert(const Iterator& iterator, Iterator first, Iterator last);

    protected:
    private:
        /**
         * Internal data to store the elements
         */
        T* m_data;

        /**
         * Iterator which points one after the end of the data
         */
        T* m_dataEnd;

        /**
         * Iterator which points to one after the end of the capacity
         */
        T* m_capacityEnd;

        /**
         * Internal method to double the current memory
         */
        void grow();
    };

    template<typename T>
    T& capu::Vector<T>::front()
    {
        return *m_data;
    }

    template<typename T>
    const T& capu::Vector<T>::front() const
    {
        return *m_data;
    }

    template<typename T>
    status_t capu::Vector<T>::insert(const Iterator& iterator, const T& value)
    {
        const uint_t numberFromBeginning = (iterator.m_current - m_data);
        if (m_dataEnd == m_capacityEnd)
        {
            /*status = */
            // todo: should check status of grow here
            grow();
        }

        // move back
        new(m_dataEnd)T();
        copy_backward(m_data + numberFromBeginning, m_dataEnd, m_dataEnd + 1u);
        ++m_dataEnd;

        m_data[numberFromBeginning] = value;
        return CAPU_OK;
    }

    template<typename T>
    status_t capu::Vector<T>::insert(const Iterator& iterator, Iterator first, Iterator last)
    {
        const uint_t numberOfNewElements = last - first;
        const uint_t insertOffset = (iterator.m_current - m_data);
        reserve(size() + numberOfNewElements);

        const uint_t numberOfExistingElementsThatNeedToBeMoved = size() - insertOffset;
        if (numberOfExistingElementsThatNeedToBeMoved > numberOfNewElements)
        {
            // existing elements move must be split
            copy_to_raw(m_dataEnd - numberOfNewElements, m_dataEnd, m_dataEnd);
            const uint_t numberOfExistingElementsToCopyDirectly = numberOfExistingElementsThatNeedToBeMoved - numberOfNewElements;
            copy_backward(m_data + insertOffset, m_data + insertOffset + numberOfExistingElementsToCopyDirectly, m_dataEnd);
            copy(first, last, m_data + insertOffset);
        }
        else
        {
            // insert new elements must be split
            copy_to_raw(m_data + insertOffset, m_dataEnd, m_dataEnd + numberOfNewElements - numberOfExistingElementsThatNeedToBeMoved);
            const uint_t numberOfElementsToInsertDirectly = numberOfExistingElementsThatNeedToBeMoved;
            copy(first, first + numberOfElementsToInsertDirectly, m_data + insertOffset);
            copy_to_raw(first + numberOfElementsToInsertDirectly, last, m_dataEnd);
        }
        m_dataEnd += numberOfNewElements;

        return CAPU_OK;
    }

    template<typename T>
    bool capu::Vector<T>::empty() const
    {
        return m_data == m_dataEnd;
    }

    template<typename T>
    capu::Vector<T>::Vector(const Vector& other)
        : m_data(reinterpret_cast<T*>(new uint8_t[sizeof(T) * other.capacity()]))
        , m_dataEnd( m_data + other.size())
        , m_capacityEnd( m_data + other.capacity())
    {
        copy_to_raw(other.m_data, other.m_dataEnd, m_data);
    }

    template<typename T>
    inline
    Vector<T>::Vector()
        : m_data(reinterpret_cast<T*>(new uint8_t[sizeof(T) * 16]))
        , m_dataEnd(m_data)
        , m_capacityEnd(m_data + 16)
    {
    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint_t initialSize, const T& value)
        : m_data(reinterpret_cast<T*>(new uint8_t[sizeof(T) * initialSize]))
        , m_dataEnd(m_data + initialSize)
        , m_capacityEnd(m_data + initialSize)
    {
        fill_n_raw(m_data, initialSize, value);
    }

    template<typename T>
    inline
    Vector<T>::Vector(const uint_t initialSize)
        : m_data(reinterpret_cast<T*>(new uint8_t[sizeof(T) * initialSize]))
        , m_dataEnd(m_data + initialSize)
        , m_capacityEnd(m_data + initialSize)
    {
        fill_n_raw(m_data, initialSize);
    }

    template<typename T>
    inline
    Vector<T>&
    Vector<T>::operator=(const Vector<T>& other)
    {
        clear();
        const uint_t numberOfElementsInOther = other.size();
        reserve(numberOfElementsInOther);

        copy_to_raw(other.m_data, other.m_dataEnd, m_data);
        m_dataEnd = m_data + numberOfElementsInOther;

        return *this;
    }

    template<typename T>
    inline Vector<T>::~Vector()
    {
        clear();
        const uint8_t* untypedMemory = reinterpret_cast<uint8_t*>(m_data);
        delete[] untypedMemory;
    }

    template<typename T>
    inline
    status_t
    Vector<T>::push_back(const T& value)
    {
        status_t status = CAPU_OK;
        if (m_dataEnd == m_capacityEnd)
        {
            /*status = */
            // todo: should check status of grow here
            grow();
        }

        T* thing = reinterpret_cast<T*>(m_dataEnd);
        new(thing)T(value);

        ++m_dataEnd;

        return status;
    }

    template<typename T>
    inline
    status_t
    Vector<T>::pop_back()
    {
        if (m_dataEnd > m_data)
        {
            (m_dataEnd - 1)->~T();
            --m_dataEnd;
            return CAPU_OK;
        }
        else
        {
            return CAPU_ERANGE;
        }
    }

    template<typename T>
    inline
    void
    Vector<T>::clear()
    {
        destruct_raw(m_data, m_dataEnd);
        m_dataEnd = m_data;
    }

    template<typename T>
    inline
    void
    Vector<T>::grow()
    {
        const uint_t currentNumberOfElements = size();
        uint_t newSize = 2 * currentNumberOfElements;
        if(0 == newSize)
        {
            newSize = 1;
        }

        reserve(newSize);
    }

    template<typename T>
    inline
    void
    Vector<T>::resize(const uint_t newSize)
    {
        const uint_t previousNumberOfElements = size();
        if (newSize < previousNumberOfElements)
        {
            // must delete excess elements
            destruct_raw(m_data + newSize, m_data + previousNumberOfElements);
        }
        else
        {
            if (newSize > capacity())
            {
                // new size does not fit, must grow first
                reserve(newSize);
            }
            // initialize new objects
            const uint_t numberOfNewObjects = newSize - previousNumberOfElements;
            fill_n_raw(m_data + previousNumberOfElements, numberOfNewObjects);

        }
        m_dataEnd = m_data + newSize;
    }

    template<typename T>
    inline
    void
    Vector<T>::reserve(const uint_t newSize)
    {
        if (newSize > capacity())
        {
            const uint_t currentNumberOfElements = size();
            void* newMemory = new uint8_t[sizeof(T) * newSize];
            T* newTypedMemory = reinterpret_cast<T*>(newMemory);

            copy_to_raw(m_data, m_dataEnd, newTypedMemory);
            destruct_raw(m_data, m_dataEnd);

            // delete previous memory
            uint8_t* untypedMemory = reinterpret_cast<uint8_t*>(m_data);
            delete[] untypedMemory;
            m_data = newTypedMemory;
            m_dataEnd = m_data + currentNumberOfElements;
            m_capacityEnd = m_data + newSize;
        }
    }

    template<typename T>
    inline
    T&
    Vector<T>::operator[](const uint_t index) const
    {
        return *(m_data + index);
    }

    template<typename T>
    inline
    uint_t
    Vector<T>::size() const
    {
        const uint_t numberOfElements = (m_dataEnd - m_data);
        return numberOfElements;
    }

    template<typename T>
    inline
    uint_t Vector<T>::capacity() const
    {
        return (m_capacityEnd - m_data);
    }

    template <typename T>
    inline
    typename Vector<T>::Iterator Vector<T>::begin()
    {
        return Iterator(m_data);
    }

    template<typename T>
    inline
    typename Vector<T>::ConstIterator
    Vector<T>::begin() const
    {
        return ConstIterator(m_data);
    }

    template<typename T>
    inline
    typename Vector<T>::Iterator
    Vector<T>::end()
    {
        return Iterator(m_dataEnd);
    }

    template<typename T>
    inline
    typename Vector<T>::ConstIterator
    Vector<T>::end() const
    {
        return ConstIterator(m_dataEnd);
    }

    template<typename T>
    inline
    status_t Vector<T>::erase(const uint_t index, T* elementOld)
    {
        const Iterator current = Iterator(m_data + index);
        return erase(current, elementOld);
    }

    template<typename T>
    inline
    status_t Vector<T>::erase(const Iterator& iterator, T* elementOld)
    {
        if(iterator.m_current >= m_dataEnd)
        {
            return CAPU_EINVAL;
        }

        if(0 != elementOld)
        {
            *elementOld = *iterator;
        }

        copy(iterator + 1u, Iterator(m_dataEnd), iterator);
        resize(size() - 1);

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
        return equal(m_data, m_dataEnd, other.m_data);
    }

}

#endif // CAPU_VECTOR_H
