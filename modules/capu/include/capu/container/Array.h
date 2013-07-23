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

#ifndef CAPU_ARRAY_H
#define CAPU_ARRAY_H

#include "capu/Config.h"
#include "capu/Error.h"
#include "capu/os/Memory.h"
#include "capu/util/ScopedPointer.h"
#include "capu/util/Swap.h"

namespace capu
{

    /**
     * Array of objects
     */
    template<typename T>
    class Array
    {
    public:

        /**
         * Default constructor
         */
        Array<T>();

        /**
         * Constructor
         * @param size Fix size of the array
         */
        Array<T>(const uint_t size);

        /**
         * Constructor
         * @param size Fix size of the array
         * @param value Value to initialize the elements of the array
         */
        Array<T>(const uint_t size, const T& value);

        /**
         * Constructor that copies a certain number of elements from a native array
         * @param other array to copy from
         * @param size number of elements to copy
         */
        Array<T>(const T other[], const uint_t size);

        /**
         * Copy constructor
         * @param other The array to copy from
         */
        Array<T>(const Array<T>& other);

        /**
         * Assignment operator
         * @param other The array to assign from
         */
        Array<T>& operator=(const Array<T>& other);

        /**
         * Destructor
         */
        virtual ~Array(void);

        /**
         * Returns size of the array
         * @return returns the size of the array
         */
        uint_t size() const;

        /**
         * Initializes the array with the given value
         * @param value initialization value
         */
        void set(const T& value);

        /**
         * Sets every array index between start and start+count to value
         * @param value initialization value
         * @param start start index
         * @param count number of elements
         * @return CAPU_ERANGE if out of bounds
         *         CAPU_OK otherwise
         */
        status_t set(const T& value, const uint_t start, const uint_t count);

        /**
         * Moves elements inside the array
         * @param start start index
         * @param count number of elements
         * @param dst destination index
         * @return CAPU_ERANGE if out of bounds
         *         CAPU_OK otherwise
         */
        status_t move(const uint_t start, const uint_t count, const uint_t dst);

        /**
         * Copies the given values into the array
         * @param other values for the contents of the array
         * @param size fix size of the array
         * @return CAPU_ERANGE if size is bigger array size
         *         CAPU_OK otherwise
         */
        status_t copy(const T other[], const uint_t size);

        /**
         * Swaps the contents with another array
         */
        void swap(Array<T>& other);

        /**
         * Allows access of an array element
         * @param index the index of the element to retrieve
         * @return reference to the element at the given position
         */
        T& operator [](const uint_t index) const;

        /**
         * Sets the raw data of the array to the given value
         * @param value the byte value for all array elements
         */
        void setRawData(const int32_t value);

        /**
         * Sets the size of the array. If the array allready has a size the internal
         * buffer will be deleted
         * @param size the number of elements which can be stored in the array
         */
        void setSize(const uint_t size);

        /**
         * Returns a pointer to directly write to the array
         * @return a const pointer to the internal buffer
         */
        const T* getRawData() const;

        /**
         * Returns a pointer to directly write to the array
         * @return a pointer to the internal buffer
         */
        T* getRawData();

    private:
        typedef ScopedArray<T> InternalArray;

        status_t setData(const T& value, const uint_t start, const uint_t count);

        uint_t mSize;
        InternalArray mInternalArray;
    };


    /*
     * Implementation specialized swap for array
     */

    template<typename T>
    void swap(Array<T>& first, Array<T>& second)
    {
        first.swap(second);
    }


    /*
     * Implementation Array
     */

    template<typename T>
    Array<T>::Array()
        : mSize(0)
        , mInternalArray(0)
    {
    }

    template<typename T>
    Array<T>::Array(const uint_t size)
        : mSize(size)
        , mInternalArray(size)
    {
    }

    template<typename T>
    Array<T>::Array(const uint_t size, const T& value)
        : mSize(size)
        , mInternalArray(size)
    {
        set(value);
    }

    template<typename T>
    Array<T>::Array(const T other[], uint_t size)
        : mSize(size), mInternalArray(size)
    {
        Memory::CopyObject(mInternalArray, other, size);
    }


    template<typename T>
    Array<T>::Array(const Array<T>& other)
        : mSize(other.mSize), mInternalArray(other.mSize)
    {
        Memory::CopyObject(mInternalArray.getRawData(), other.getRawData(), other.size());
    }


    template<typename T>
    void Array<T>::swap(Array<T>& other)
    {
        capu::swap(mSize, other.mSize);
        capu::swap(mInternalArray, other.mInternalArray);
    }

    template<typename T>
    Array<T>&  Array<T>::operator=(const Array<T>& other)
    {
        if (mSize != other.mSize)
        {
            mSize = other.mSize;

            // create temporary array with the correct size and then swap
            // this ensures automatic release of the old memory block
            InternalArray tmpArray(mSize);
            mInternalArray.swap(tmpArray);
        }

        Memory::CopyObject(getRawData(), other.getRawData(), other.size());
        return *this;
    }


    template<typename T>
    Array<T>::~Array(void)
    {
    }

    template<typename T>
    void Array<T>::set(const T& value)
    {
        setData(value, 0, mSize);
    }

    template<typename T>
    status_t Array<T>::set(const T& value, const uint_t index, const uint_t count)
    {
        return setData(value, index, count);
    }

    template<typename T>
    status_t Array<T>::move(const uint_t start, const uint_t count, const uint_t dst)
    {
        if ((start >= mSize) || ((count + start) > mSize) || ((dst + count) > mSize))
        {
            return CAPU_ERANGE;
        }
        Memory::MoveObject(mInternalArray.getRawData() + dst, mInternalArray.getRawData() + start, count);

        return CAPU_OK;
    }

    template<typename T>
    status_t Array<T>::copy(const T other[], const uint_t size)
    {
        if (size > mSize)
        {
            return CAPU_ERANGE;
        }

        Memory::CopyObject(mInternalArray.getRawData(), other, size);

        return CAPU_OK;
    }

    template<typename T>
    T& Array<T>::operator[](const uint_t index) const
    {
        return mInternalArray[index];
    }

    template<typename T>
    uint_t Array<T>::size() const
    {
        return mSize;
    }

    template<typename T>
    status_t Array<T>::setData(const T& value, const uint_t index, const uint_t count)
    {

        if ((index >= mSize) || ((count + index) > mSize))
        {
            return CAPU_ERANGE;
        }
        T* start = &mInternalArray[mSize - (mSize - count) + index];

        switch (count)
        {
        default:
        {
            T* end = start - (count - 10);

            // process the first "count % 4" elements
            uint_t count = start - end;
            switch (count & 0x03)
            {
            case 3 :
                *(--start) = value;
            case 2 :
                *(--start) = value;
            case 1 :
                *(--start) = value;
            }

            // loop unrolling with 4 values per iteration
            while (start > end)
            {
                *(--start) = value;
                *(--start) = value;
                *(--start) = value;
                *(--start) = value;
            }
        }
        case 10 :
            *(--start) = value;
        case 9  :
            *(--start) = value;
        case 8  :
            *(--start) = value;
        case 7  :
            *(--start) = value;
        case 6  :
            *(--start) = value;
        case 5  :
            *(--start) = value;
        case 4  :
            *(--start) = value;
        case 3  :
            *(--start) = value;
        case 2  :
            *(--start) = value;
        case 1  :
            *(--start) = value;
        case 0  :
        {} // needed for count == 0
        }

        return CAPU_OK;
    }

    template<typename T>
    void Array<T>::setRawData(const int32_t value)
    {
        Memory::Set(mInternalArray.getRawData(), value, sizeof(T) * mSize);
    }

    template<typename T>
    const T* Array<T>::getRawData() const
    {
        return mInternalArray.getRawData();
    }

    template<typename T>
    T* Array<T>::getRawData()
    {
        return mInternalArray.getRawData();
    }

    template<typename T>
    void Array<T>::setSize(const uint_t size)
    {
        InternalArray tmpArray(size);
        mInternalArray.swap(tmpArray);
        mSize = size;
    }
}


#endif // CAPU_ARRAY_H
