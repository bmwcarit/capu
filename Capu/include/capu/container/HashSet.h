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

#ifndef CAPU_HASHSET_H
#define CAPU_HASHSET_H

#include "capu/container/Hash.h"
#include "capu/container/HashTable.h"

namespace capu
{
    /**
     * Unordered set of objects with fast lookup and retrieval.
     */
    template <class T, class C = Comparator, class H = CapuDefaultHashFunction<sizeof(uint_t)*8> >
    class HashSet
    {
    private:

        /**
         * Iterator for HashSet
         */
        template<typename HashTableIteratorType>
        class HashSetIterator
        {
        public:

            /**
             * Copy Constructor
             * @param iter iterator to copy
             */
            HashSetIterator(const HashSetIterator<HashTableIteratorType>& iter)
                : m_iter(iter.m_iter)
            {
            }

            /*
            * Assignment Operator
            * @param iter r-value iterator for assignment
            */
            HashSetIterator& operator=(const HashSetIterator<HashTableIteratorType>& iter)
            {
                m_iter = iter.m_iter;
                return *this;
            }

            /**
             * Indirection
             * @return the current value referenced by the iterator
             */
            const T& operator*()
            {
                return (*m_iter).key;
            }

            /**
             * Dereference
             * @return a pointer to the current object the iterator points to
             */
            const T* operator->()
            {
                return &((*m_iter).key);
            }

            /**
             * Compares two iterators
             * @return true if the iterators point to the same position
             */
            bool operator==(const HashSetIterator<HashTableIteratorType>& iter) const
            {
                return (m_iter == iter.m_iter);
            }

            /**
             * Compares two iterators
             * @return true if the iterators do not point to the same position
             */
            bool operator!=(const HashSetIterator<HashTableIteratorType>& iter) const
            {
                return (m_iter != iter.m_iter);
            }

            /**
             * Step the iterator forward to the next element (prefix operator)
             * @return the next iterator
             */
            HashSetIterator<HashTableIteratorType>& operator++()
            {
                m_iter++;
                return *this;
            }

            /**
             * Step the iterator forward to the next element (postfix operator)
             * @return the next iterator
             */
            HashSetIterator<HashTableIteratorType> operator++(int32_t)
            {
                HashSetIterator<HashTableIteratorType> oldValue(*this);
                ++(*this);
                return oldValue;
            }

        private:

            friend class HashSet<T, C, H>;

            /**
             * Internal constructor for HashSet
             *
             * @para iter iterator of the underlying hash table
             */
            HashSetIterator(const HashTableIteratorType& iter)
                : m_iter(iter)
            {
            }

            HashTableIteratorType m_iter;
        };

    public:

        /// defines to amount of bits to use for hash set size
        static const uint8_t DefaultHashSetBitSize;

        /**
         * Iterator for hashsets
         */
        typedef HashSetIterator< typename HashTable<T, char, C, H>::Iterator >        Iterator;
        typedef HashSetIterator< typename HashTable<T, char, C, H>::ConstIterator >   ConstIterator;

        /**
         * Default Constructor
         */
        HashSet();

        /**
         * Parameterized Constructor
         * @param bitsize size of initial HashSet
         */
        HashSet(const uint8_t bitsize);

        /**
         * Copy constructor.
         */
        HashSet(const HashSet& other);

        /**
         * Destructor
         */
        ~HashSet();

        /**
         * put a new value to the hash set.
         *
         * @param value             new value that will be put to hash set
         *
         * @return CAPU_OK if remove is successful
         *         CAPU_ERROR if value already exists in the set
         *
         */
        status_t put(const T& value);

        /**
         * Remove value associated with key in the hash set.
         *
         * @param value             value that will be removed
         *
         * @return CAPU_OK if remove is successful
         *         CAPU_ERANGE if specified value does not exist in hash set
         *
         */
        status_t remove(const T& value);

        /**
         * Remove iterator associated with key in the hash set.
         *
         * @param iterator iterator pointing to value that will be removed
         *
         * @return CAPU_OK if removal was successful
         *         CAPU_ENOT_EXISTS if the key was not found in the set
         *
         */
        status_t removeAt(Iterator& iterator);

        /**
         * Checks if the provided value is already contained in the hash set.
         *
         * @param value             value that will be checked
         *
         * @return true if element is already contained in the hash set
         *         false otherwise
         *
         */
        bool hasElement(const T& value) const;

        /**
         * Returns count of the hash set.
         * @return number of element in hash set
         */
        uint_t count() const;

        /**
         * Clear all values of the hash set.
         *
         * @return CAPU_OK if all elements in list have been deleted
         */
        status_t clear();

        /**
         * Return iterator for iterating key value tuples.
         * @return Iterator
         */
        Iterator begin();

        /**
         * Return ConstIterator for iterating read only key value tuples.
         * @return ConstIterator
         */
        ConstIterator begin() const;

        /**
         * returns an iterator pointing after the last element of the list
         * @return iterator
         */
        Iterator end();

        /**
         * returns a ConstIterator pointing after the last element of the list
         * @return ConstIterator
         */
        ConstIterator end() const;

        /**
         * Reserve space for given number of bits elements. Does nothing if the
         * HashSet is already bigger.
         * @param bitsize The requested bit size of the set.
         */
        void reserve(uint8_t bitsize);

        /**
        * Swap this HashSet with another
        * @param other HashSet to copy from
        */
        void swap(HashSet<T, C, H>& other);

    private:
        HashTable<T, char, C, H> m_table;
    };

    /**
     * swap specialization for HashSet<T, C, H>
     * @param first first HashSet
     * @param second HashSet to swap with first
     */
    template <class Key, class T, class C, class H>
    inline void swap(HashSet<T, C, H>& first, HashSet<T, C, H>& second)
    {
        first.swap(second);
    }

    template <class T, class C, class H>
    const uint8_t HashSet<T, C, H>::DefaultHashSetBitSize = 4u;

    template <class T, class C, class H>
    HashSet<T, C, H>::HashSet(const HashSet& other)
        : m_table(other.m_table) // just copy the inner hash table (which defines a copy constructor)
    {
    }

    template <class T, class C, class H>
    HashSet<T, C, H>::HashSet()
        : m_table(DefaultHashSetBitSize)
    {
    }

    template <class T, class C, class H>
    HashSet<T, C, H>::~HashSet()
    {
    }

    template <class T, class C, class H>
    HashSet<T, C, H>::HashSet(const uint8_t bitsize)
        : m_table(bitsize)
    {
    }

    template <class T, class C, class H>
    status_t HashSet<T, C, H>::put(const T& value)
    {
        if (m_table.contains(value))
        {
            return CAPU_ERROR;
        }
        return m_table.put(value, 0);
    }

    template <class T, class C, class H>
    status_t HashSet<T, C, H>::remove(const T& value)
    {
        return m_table.remove(value);
    }

    template <class T, class C, class H>
    status_t HashSet<T, C, H>::removeAt(Iterator& it)
    {
        return m_table.remove(it.m_iter);
    }

    template <class T, class C, class H>
    bool HashSet<T, C, H>::hasElement(const T& value) const
    {
        return m_table.contains(value);
    }

    template <class T, class C, class H>
    uint_t HashSet<T, C, H>::count() const
    {
        return m_table.count();
    }

    template <class T, class C, class H>
    status_t HashSet<T, C, H>::clear()
    {
        m_table.clear();
        return CAPU_OK;
    }

    template <class T, class C, class H>
    typename HashSet<T, C, H>::Iterator HashSet<T, C, H>::begin()
    {
        return Iterator(m_table.begin());
    }

    template <class T, class C, class H>
    typename HashSet<T, C, H>::ConstIterator HashSet<T, C, H>::begin() const
    {
        return ConstIterator(m_table.begin());
    }

    template <class T, class C, class H>
    typename HashSet<T, C, H>::Iterator HashSet<T, C, H>::end()
    {
        return Iterator(m_table.end());
    }

    template <class T, class C, class H>
    typename HashSet<T, C, H>::ConstIterator HashSet<T, C, H>::end() const
    {
        return ConstIterator(m_table.end());
    }

    template <class T, class C, class H>
    void HashSet<T, C, H>::reserve(uint8_t bitsize)
    {
        m_table.reserve(bitsize);
    }

    template <class T, class C, class H>
    void HashSet<T, C, H>::swap(HashSet<T, C, H>& other)
    {
        m_table.swap(other.m_table);
    }
}

#endif /* CAPU_HASHSET_H */
