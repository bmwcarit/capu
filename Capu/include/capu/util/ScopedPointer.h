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

#ifndef CAPU_SCOPED_POINTER_H
#define CAPU_SCOPED_POINTER_H

#include "capu/Config.h"
#include <algorithm>

namespace capu
{
    template<typename T>
    struct DefaultDeleter
    {
        static void performDelete(T* ptr)
        {
            delete ptr;
        }
    };

    template<typename T>
    struct DefaultArrayDeleter
    {
        static void performDelete(T* ptr)
        {
            delete[] ptr;
        }
    };

    /**
     * Scoped pointer
     *
     * This class takes ownership of a pointer to a resource.
     * On deconstruction it deletes the pointer with either delete or delete[]
     * depending on the deleter.
     *
     * A custom deleter can be passed in. This deleter must have a function
     * of "static void performDelete(T* ptr)" which must perform the delete.
     */
    template<typename T, typename DELETER = DefaultDeleter<T> >
    class ScopedPointer
    {
    public:

        /**
         * Constructor with already existing resource.
         *
         * This constructor takes the ownership of the given resource so the resource
         * will be deleted on deconstruction.
         *
         * @param resource the resource to handle. If no resource is passed, a new one will get created as default value.
         */
        explicit ScopedPointer<T, DELETER>(T* resource);

        /**
         * Destructor to take care of destroying the data.
         */
        ~ScopedPointer();

        /**
         * Overload pointer access operator to be able to access the object
         * referenced by the pointer
         */
        T* operator->() const;

        /**
         * Overload dereference operator to be able to get the object
         * referenced by the pointer. Use with care!
         */
        T& operator*() const;

        /**
         * Swaps the data pointers to exchange ownership of the memory.
         */
        void swap(ScopedPointer<T, DELETER>& other);

        /**
         * Non const access to the underlying raw data.
         */
        T* get();

        /**
         * Const access to the underlying raw data.
         */
        const T* get() const;

        /**
        * Calls DELETER for the current object and replaces it with a new one
        * @param ptr pointer to the new object
        */
        void reset(T* ptr = 0);

        /**
        * Releases the ownership without calling DELTER.
        * @return the currently owned object or 0 if none owned
        */
        T* release();

    protected:
        T* mRawData;

    private:

        /**
         * Private assignment operator with no implementation in order to prevent assignment
         *
         * @param other the other internal array
         */
        ScopedPointer<T, DELETER>& operator=(const ScopedPointer<T, DELETER>& other);

        /**
         * Private copy constructor with no implementation in order to prevent copies
         */
        ScopedPointer(const ScopedPointer<T, DELETER>& other);
    };

    /**
     * Scoped array
     *
     * This non-copyable scoped pointer manages an array of resources and destroys
     * it when being deconstructed.
     */
    template <typename T, typename DELETER = DefaultArrayDeleter<T> >
    class ScopedArray : private ScopedPointer<T, DELETER>
    {
    public:

        /**
         * Constructor with size
         *
         * @param size if greater than 0 it will initialize the underlying array with the
         *             given size. Otherwise it will initialize with NULL
         */
        explicit ScopedArray(uint_t size);

        /**
         * Allows access of an array element
         * @param index the index of the element to retrieve
         * @return reference to the element at the given position
         */
        T& operator [](const uint_t index) const;

        /**
         * Swaps the data pointers to exchange ownership of the memory.
         */
        void swap(ScopedArray<T, DELETER>& other);

        using ScopedPointer<T, DELETER>::get;
        using ScopedPointer<T, DELETER>::reset;
        using ScopedPointer<T, DELETER>::release;

    private:

        typedef ScopedPointer<T, DELETER> ScopedArrayBaseClass;

        /**
         * Private assignment operator with no implementation in order to prevent assignment
         *
         * @param other the other internal array
         */
        ScopedArray<T, DELETER>& operator=(const ScopedArray<T, DELETER>& other);

        /**
         * Private copy constructor with no implementation in order to prevent copies
         */
        ScopedArray(const ScopedArray<T, DELETER>& other);
    };

    /*
     * Specialized swap templates
     */

    template<typename T, typename DELETER>
    void swap(ScopedArray<T, DELETER>& first, ScopedArray<T, DELETER>& second)
    {
        first.swap(second);
    }

    template<typename T, typename DELETER>
    void swap(ScopedPointer<T, DELETER>& first, ScopedPointer<T, DELETER>& second)
    {
        first.swap(second);
    }

    /*
     * Implementation ScopedPointer
     */

    template<typename T, typename DELETER>
    ScopedPointer<T, DELETER>::~ScopedPointer()
    {
        DELETER::performDelete(mRawData);
    }

    template<typename T, typename DELETER>
    ScopedPointer<T, DELETER>::ScopedPointer(T* resource)
        : mRawData(resource)
    {
    }

    template<typename T, typename DELETER>
    void ScopedPointer<T, DELETER>::swap(ScopedPointer<T, DELETER>& other)
    {
        using std::swap;
        swap(mRawData, other.mRawData);
    }

    template<typename T, typename DELETER>
    inline T* ScopedPointer<T, DELETER>::get()
    {
        return mRawData;
    }

    template<typename T, typename DELETER>
    inline const T* ScopedPointer<T, DELETER>::get() const
    {
        return mRawData;
    }

    template<typename T, typename DELETER>
    inline void ScopedPointer<T, DELETER>::reset(T* ptr)
    {
        if (mRawData)
        {
            DELETER::performDelete(mRawData);
        }
        mRawData = ptr;
    }

    template<typename T, typename DELETER>
    inline T* ScopedPointer<T, DELETER>::release()
    {
        T* ptr = mRawData;
        mRawData = 0;
        return ptr;
    }

    template<typename T, typename DELETER>
    inline T* ScopedPointer<T, DELETER>::operator->() const
    {
        return mRawData;
    }

    template<typename T, typename DELETER>
    inline T& ScopedPointer<T, DELETER>::operator*() const
    {
        return *mRawData;
    }

    /*
     * Implementation ScopedArray
     */

    template<typename T, typename DELETER>
    ScopedArray<T, DELETER>::ScopedArray(uint_t size)
        : ScopedArrayBaseClass(size > 0 ? new T[size] : NULL)
    {
    }

    template<typename T, typename DELETER>
    inline T& ScopedArray<T, DELETER>::operator[](const uint_t index) const
    {
        return ScopedArrayBaseClass::mRawData[index];
    }

    template<typename T, typename DELETER>
    void ScopedArray<T, DELETER>::swap(ScopedArray<T, DELETER>& other)
    {
        ScopedArrayBaseClass::swap(other);
    }
}

#endif //CAPU_SCOPED_POINTER_H
