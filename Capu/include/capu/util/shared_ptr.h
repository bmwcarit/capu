/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_SHAREDPTR_H
#define CAPU_SHAREDPTR_H

#include "capu/Config.h"
#include "capu/os/AtomicOperation.h"
#include "capu/util/Swap.h"

namespace capu
{
    namespace internal
    {
        class MetadataBase;
    }

    /**
     * Wraps a normal pointer and manages its memory with reference counting.
     */
    template<class T>
    class shared_ptr
    {
    public:
        /**
        * Default constructor
        */
        shared_ptr();

        /**
         * Constructor
         * @param ptr pointer to object
         */
        template <typename U>
        explicit shared_ptr(U* ptr);

        /**
         * Constructor with Deleter
         * @param ptr pointer to object
         * @param deleter callable object that gets invoked with ptr when reference count drops to 0
         */
        template <typename U, class Deleter>
        shared_ptr(U* ptr, const Deleter& deleter);

        /**
         * Copy constructor
         * @param sharedPtr reference to shared_ptr
         */
        shared_ptr(const shared_ptr& sharedPtr);

        /**
         * Copy constructor for different, but castable type
         * @param sharedPtr reference to shared_ptr
         */
        template <typename U>
        shared_ptr(const shared_ptr<U>& sharedPtr);

        /**
         * Destructor
         */
        ~shared_ptr();

        /**
         * Assignment operator. Makes this shared_ptr drop its reference to
         * whatever it pointed to before and take a reference to what other
         * points to.
         * @param other reference to other shared_ptr
         */
        shared_ptr& operator=(const shared_ptr& other);

        /**
         * Assignment operator for castable, but different type. Makes
         * this shared_ptr drop its reference to whatever it pointed
         * to before and take a reference to what other points to.
         * @param other reference to other shared_ptr
         */
        template <typename U>
        shared_ptr& operator=(const shared_ptr<U>& other);

        /**
         * Resets this shared_ptr to empty state and drop the reference
         * to what it pointed to.
         */
        void reset();

        /**
         * Resets this shared_ptr to a new pointer. It drops the reference
         * to what it pointed to and takes ownership of ptr.
         * @param ptr pointer to object that shared_ptr shall own
         */
        template <typename U>
        void reset(U* ptr);

        /**
         * Resets this shared_ptr to a new pointer and deleter. It drops the reference
         * to what it pointed to and takes ownership of ptr.
         * @param ptr pointer to object that shared_ptr shall own
         * @param deleter callable object that gets invoked with ptr when reference count drops to 0
         */
        template <typename U, class Deleter>
        void reset(U* ptr, const Deleter& d);

        /**
         * Exchange the contents with another shared_ptr.
         * @param other shared_ptr to swap the encapsulated object with
         */
        void swap(shared_ptr& other);

        /**
         * Get the pointer to the current object.
         * @return encapsulated object or 0 if has none
         */
        T* get() const;

        /**
         * Dereference operator. Gives a reference to the current object.
         * Undefined if has no object.
         * @return reference to the current object
         */
        T& operator*() const;

        /**
         * Arrow operator. Allows member access on the object.
         * Undefined if has no object.
         * @return pointer to the current object
         */
        T* operator->() const;

        /**
         * Returns the number of references to the encapsulated object.
         * @return number of references or 0 if has no object
         */
        uint_t use_count() const;

        /**
         * Conversion operator to bool. Returns of shared_ptr encapsulates an object.
         * @return true if has an object, false otherwise
         */
        operator bool() const;

        /**
         * Equality operator.
         * @param other shared_ptr to compare to
         * @return true if this and other point to the same object, false otherwise
         */
        template <typename U>
        bool operator==(const shared_ptr<U>& other) const;

        /**
         * Inequality operator.
         * @param other shared_ptr to compare to
         * @return true if this and other point different objects, false otherwise
         */
        template <typename U>
        bool operator!=(const shared_ptr<U>& other) const;

    private:
        template<class U>
        friend class shared_ptr;

        void incRefCount();
        void decRefCount();

        capu::internal::MetadataBase* mMetadata;
        T* mData;
    };

    /**
     * swap specialization for shared_ptr<T>
     * @param first first shared_ptr
     * @param second shared_ptr to swap with first
     */
    template <typename T>
    void swap(const shared_ptr<T>& first, const shared_ptr<T>& second)
    {
        first.swap(second);
    }


    namespace internal
    {
        class MetadataBase
        {
        public:
            MetadataBase(uint_t initialReferenceCount)
                : mReferenceCount(initialReferenceCount)
            {
            }

            virtual ~MetadataBase() {}

            virtual void callDeleter() = 0;

            void incRefCount()
            {
                capu::AtomicOperation::AtomicInc(mReferenceCount);
            }

            void decRefCount()
            {
                capu::AtomicOperation::AtomicDec(mReferenceCount);
            }

            uint_t numReferences() const
            {
                return mReferenceCount;
            }

        private:
            uint_t mReferenceCount;
        };

        template <typename T>
        class MetadataDefaultDeleter : public capu::internal::MetadataBase
        {
        public:
            MetadataDefaultDeleter(uint_t initialReferenceCount, T* ptr)
                : capu::internal::MetadataBase(initialReferenceCount)
                , mPtr(ptr)
            {
            }

            virtual void callDeleter()
            {
                delete mPtr;
            }

        protected:
            T* mPtr;
        };

        template <typename T, class Deleter>
        class MetadataDeleterFunctor : public MetadataDefaultDeleter<T>
        {
        public:
            MetadataDeleterFunctor(uint_t initialReferenceCount, T* ptr, const Deleter& deleter)
                : MetadataDefaultDeleter<T>(initialReferenceCount, ptr)
                , mDeleter(deleter)
            {
            }

            virtual void callDeleter()
            {
                mDeleter(MetadataDefaultDeleter<T>::mPtr);
            }

        private:
            Deleter mDeleter;
        };
    }


    template<class T>
    inline
    shared_ptr<T>::shared_ptr()
        : mMetadata(0)
        , mData(0)
    {
    }

    template<class T>
    template <typename U>
    inline
    shared_ptr<T>::shared_ptr(U* ptr)
        : mMetadata(ptr ? new capu::internal::MetadataDefaultDeleter<U>(1, ptr) : 0)
        , mData(static_cast<T*>(ptr))
    {
    }

    template<class T>
    template <typename U, class Deleter>
    inline
    shared_ptr<T>::shared_ptr(U* ptr, const Deleter& d)
        : mMetadata(ptr ? new capu::internal::MetadataDeleterFunctor<U, Deleter>(1, ptr, d) : 0)
        , mData(static_cast<T*>(ptr))
    {
    }

    template<class T>
    inline
    shared_ptr<T>::shared_ptr(const shared_ptr& sharedPtr)
        : mMetadata(sharedPtr.mMetadata)
        , mData(sharedPtr.mData)
    {
        incRefCount();
    }

    template<class T>
    template <typename U>
    inline
    shared_ptr<T>::shared_ptr(const shared_ptr<U>& sharedPtr)
        : mMetadata(sharedPtr.mMetadata)
        , mData(static_cast<T*>(sharedPtr.mData))
    {
        incRefCount();
    }

    template<class T>
    inline
    shared_ptr<T>::~shared_ptr()
    {
        decRefCount();
    }

    template<class T>
    inline
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other)
    {
        if (mMetadata != other.mMetadata)
        {
            decRefCount();
            mData = other.mData;
            mMetadata = other.mMetadata;
            incRefCount();
        }
        return *this;
    }

    template<class T>
    template <typename U>
    inline
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<U>& other)
    {
        if (mMetadata != other.mMetadata)
        {
            decRefCount();
            mData = static_cast<T*>(other.mData);
            mMetadata = other.mMetadata;
            incRefCount();
        }
        return *this;
    }

    template<class T>
    inline
    void shared_ptr<T>::reset()
    {
        decRefCount();
        mData = 0;
        mMetadata = 0;
    }

    template<class T>
    template <typename U>
    inline
    void shared_ptr<T>::reset(U* ptr)
    {
        decRefCount();
        mData = 0;
        mMetadata = new capu::internal::MetadataDefaultDeleter<U>(1, ptr);
        mData = static_cast<T*>(ptr);
    }

    template<class T>
    template <typename U, class Deleter>
    inline
    void shared_ptr<T>::reset(U* ptr, const Deleter& d)
    {
        decRefCount();
        mData = 0;
        mMetadata = new capu::internal::MetadataDeleterFunctor<U, Deleter>(1, ptr, d);
        mData = static_cast<T*>(ptr);
    }

    template<class T>
    inline
    void shared_ptr<T>::swap(shared_ptr& other)
    {
        using capu::swap;
        swap(mData, other.mData);
        swap(mMetadata, other.mMetadata);
    }

    template<class T>
    inline
    T* shared_ptr<T>::get() const
    {
        return mData;
    }

    template<class T>
    inline
    T* shared_ptr<T>::operator->() const
    {
        return mData;
    }

    template<class T>
    inline
    T& shared_ptr<T>::operator*() const
    {
        return *mData;
    }

    template<class T>
    inline
    uint_t shared_ptr<T>::use_count() const
    {
        if (mMetadata)
        {
            return mMetadata->numReferences();
        }
        return 0;
    }

    template<class T>
    inline
    shared_ptr<T>::operator bool() const
    {
        return mData != 0;
    }

    template<class T>
    inline
    void shared_ptr<T>::incRefCount()
    {
        if (mMetadata)
        {
            mMetadata->incRefCount();
        }
    }

    template<class T>
    template <typename U>
    inline
    bool shared_ptr<T>::operator==(const shared_ptr<U>& other) const
    {
        return mData == other.mData;
    }

    template<class T>
    template <typename U>
    inline
        bool shared_ptr<T>::operator!=(const shared_ptr<U>& other) const
    {
        return mData != other.mData;
    }

    template<class T>
    inline
    void shared_ptr<T>::decRefCount()
    {
        if (mMetadata)
        {
            mMetadata->decRefCount();
            if (0 == mMetadata->numReferences())
            {
                mMetadata->callDeleter();
                delete mMetadata;
            }

            mMetadata = 0;
            mData = 0;
        }
    }

}

#endif // CAPU_SHAREDPTR_H
