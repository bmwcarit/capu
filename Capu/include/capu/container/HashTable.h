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

#ifndef CAPU_HASHTABLE_H
#define CAPU_HASHTABLE_H

#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/Comparator.h"
#include "capu/container/Pair.h"
#include "capu/container/Hash.h"
#include "capu/os/Memory.h"

//defines the threshold after which the list will get resized.
#define DEFAULT_HASH_TABLE_MAX_LOAD_FACTOR 0.8f
//defines to amount of bits to use for hash set size
#define DEFAULT_HASH_TABLE_BIT_SIZE 4
//defines to amount of bits to use for hash set size
#define DEFAULT_HASH_SET_BIT_SIZE 4

namespace capu
{
    /**
     * Table object container where keys are found and retrieved via hashs.
     */
    template <class Key, class T, class C = Comparator, class H = CapuDefaultHashFunction>
    class HashTable
    {
    public:

        /**
         * Data structure to hold a key/value pair inside the hash table
         */
        class HashTableEntry
        {
        public:
            HashTableEntry()
                : value()
                , internalKey()
                , key(internalKey)
                , next(0)
                , previous(0)
                , isChainElement(0)

            {
                // 'preconnect' free entries
                next = this + 1;
            }

            HashTableEntry& operator=(const HashTableEntry& other)
            {
                internalKey = other.key;
                value = other.value;
                next = other.next;
                previous = other.previous;
                isChainElement = other.isChainElement;
                return *this;
            }

            HashTableEntry(const HashTableEntry& other)
                : value(other.value)
                , internalKey(other.key)
                , key(internalKey)
                , next(other.next)
                , previous(other.previous)
                , isChainElement(other.isChainElement)
            {
            }

            T value;

        private:
            Key internalKey; // private non const key
        public:
            const Key& key; // public const reference to the key
        private:
            HashTableEntry* next; // pointer to the next entry (chaining)
            HashTableEntry* previous; // pointer to the previous entry (chaining)
            uint8_t isChainElement; // true if the element is not the first element in a chain

            friend class HashTable<Key, T, C, H>;
        };

        class ConstIterator
        {
        public:

            friend class HashTable;
            friend class Iterator;

            /**
             * Constructor.
             * @param begin Pointer to the entry on which iteration should start.
             * @param end Pointer to an entry on which iteration should end (if begin == end).
             */
            ConstIterator(HashTableEntry* begin, HashTableEntry* end)
                : mCurrentHashMapEntry(begin)
                , mLastHashMapEntry(end)
            {
            }

            /**
             * Copy Constructor.
             * @param iter non-const iterator to copy
             */
            ConstIterator(const ConstIterator& iter)
                : mCurrentHashMapEntry(iter.mCurrentHashMapEntry)
                , mLastHashMapEntry(iter.mLastHashMapEntry)
            {
            }

            /**
             * Indirection
             * @return the current value referenced by the iterator
             */
            const HashTableEntry& operator*()
            {
                return *mCurrentHashMapEntry;
            }

            /**
             * Dereference
             * @return a pointer to the current object the iterator points to
             */
            const HashTableEntry* operator->()
            {
                return mCurrentHashMapEntry;
            }

            /**
             * Compares two iterators
             * @return true if the iterators point to the same position
             */
            capu::bool_t operator==(const ConstIterator& iter) const
            {
                return (mCurrentHashMapEntry == iter.mCurrentHashMapEntry);
            }

            /**
             * Compares two iterators
             * @return true if the iterators do not point to the same position
             */
            capu::bool_t operator!=(const ConstIterator& iter) const
            {
                return (mCurrentHashMapEntry != iter.mCurrentHashMapEntry);
            }

            /**
             * Step the iterator forward to the next element (prefix operator)
             * @return the next iterator
             */
            ConstIterator& operator++()
            {
                mCurrentHashMapEntry = mCurrentHashMapEntry->next;
                return *this;
            }

            /**
             * Step the iterator forward to the next element (postfix operator)
             * @return the next iterator
             */
            ConstIterator operator++(int32_t)
            {
                ConstIterator oldValue(*this);
                ++(*this);
                return oldValue;
            }

        private:
            HashTableEntry* mCurrentHashMapEntry;
            HashTableEntry* mLastHashMapEntry;
        };
        /**
         * Internal helper class to perform iterations over the map entries.
         */
        class Iterator
        {
        public:

            friend class HashTable;

            /**
             * Constructor.
             * @param begin Pointer to the entry on which iteration should start.
             * @param end Pointer to an entry on which iteration should end (if begin == end).
             */
            Iterator(HashTableEntry* begin, HashTableEntry* end)
                : mCurrentHashMapEntry(begin)
                , mLastHashMapEntry(end)
            {
            }

            /**
             * Copy Constructor.
             * @param iter non-const iterator to copy
             */
            Iterator(const Iterator& iter)
                : mCurrentHashMapEntry(iter.mCurrentHashMapEntry)
                , mLastHashMapEntry(iter.mLastHashMapEntry)
            {
            }

            /**
             * Convert Constructor
             * @param iter ConstIterator to convert from
             */
            Iterator(const ConstIterator& iter)
                : mCurrentHashMapEntry(iter.mCurrentHashMapEntry)
                , mLastHashMapEntry(iter.mLastHashMapEntry)
            {
            }

            /**
             * Indirection
             * @return the current value referenced by the iterator
             */
            HashTableEntry& operator*()
            {
                return *mCurrentHashMapEntry;
            }

            /**
             * Dereference
             * @return a pointer to the current object the iterator points to
             */
            HashTableEntry* operator->()
            {
                return mCurrentHashMapEntry;
            }

            /**
             * Compares two iterators
             * @return true if the iterators point to the same position
             */
            capu::bool_t operator==(const Iterator& iter) const
            {
                return (mCurrentHashMapEntry == iter.mCurrentHashMapEntry);
            }

            /**
             * Compares two iterators
             * @return true if the iterators do not point to the same position
             */
            capu::bool_t operator!=(const Iterator& iter) const
            {
                return (mCurrentHashMapEntry != iter.mCurrentHashMapEntry);
            }

            /**
             * Step the iterator forward to the next element (prefix operator)
             * @return the next iterator
             */
            Iterator& operator++()
            {
                mCurrentHashMapEntry = mCurrentHashMapEntry->next;
                return *this;
            }

            /**
             * Step the iterator forward to the next element (postfix operator)
             * @return the next iterator
             */
            Iterator operator++(int32_t)
            {
                Iterator oldValue(*this);
                ++(*this);
                return oldValue;
            }

        private:
            HashTableEntry* mCurrentHashMapEntry;
            HashTableEntry* mLastHashMapEntry;
        };



        /**
         * Copy constructor
         */
        HashTable(const HashTable& other);

        /**
         * Constructs HashTable.
         */
        HashTable();

        /**
         * Constructor.
         * @param initialBitSize The bit size of the initial size of the map.
         * @param resizeable Indicates if the map resizes automatically if necessary. If set to false, a 'put' may
         *                   return NO_MEMORY if too many items were added.
         */
        HashTable(const uint8_t initialBitSize, const bool_t resizeable = true);

        /**
         * Destructor.
         */
        virtual ~HashTable();

        /**
         * overloading subscript operator to get read and write access to element referenced by given key.
         *
         * @param key Key value
         * @return value Value referenced by key. If no value is stored for given key, a default constructed object is added and returned
         */
        T& operator[](const Key& key);

        /**
         * put a new value to the hashtable.
         *
         * NOTE: Not STL compatible
         *
         * @param key               Key value
         * @param value             new value that will be put to hash table
         * @return CAPU_OK if put is successful
         *         CAPU_ENO_MEMORY if allocation of element is failed
         *
         */
        status_t put(const Key& key, const T& value, T* oldValue = NULL);

        /**
         * Get const value associated with key in the hashtable.
         * @param key        Key
         * @param returnCode parameter to retrieve status code. Optional.
         *       Possible status codes:
         *       CAPU_OK if the key is contained in the hash table and the element has been retrieved successfully
         *       CAPU_ENOT_EXIST if there is no element in hash table with specified key
         *
         * @return element
         */
        const T& at(const Key& key, status_t* returnCode = 0) const;

        /**
         * Get value associated with key in the hashtable.
         * @param key        Key
         * @param returnCode parameter to retrieve status code. Optional.
         *       Possible status codes:
         *       CAPU_OK if the key is contained in the hash table and the element has been retrieved successfully
         *       CAPU_ENOT_EXIST if there is no element in hash table with specified key
         *
         * @return element
         */
        T& at(const Key& key, status_t* returnCode = 0);

        /**
         * Tries to find an element in the Hash Table.
         * If the element is found, it returns an iterator to HashTableEntry for the given key
         * If the element is not contained in the Hash Table, the end() iterator is returned.
         *
         * @param key       Key
         * @return iterator pointing to the Hash Table entry where the key got found
         *         iterator pointing to the end() element otherwise
         */
        Iterator find(const Key& key);

        /**
         * Tries to find an element in read only Hash Table.
         * If the element is found, it returns a ConstIterator to HashTableEntry for the given key
         * If the element is not contained in the Hash Table, the end() constIterator is returned.
         *
         * @param key       Key
         * @return ConstIterator pointing to the Hash Table entry where the key got found
         *         ConstIterator pointing to the end() element otherwise
         */
        ConstIterator find(const Key& key) const;

        /**
         * Checks weather the given key is present in the table.
         *
         * NOTE: Not STL compatible
         *
         * @param key The key.
         * @return True if the key is present, false otherwise.
         */
        bool_t contains(const Key& key) const;

        /**
         * Removes the value associated with key in the hashtable.
         *
         * NOTE: Not STL compatible
         *
         * @param key               Key value.
         * @param value_old         Buffer which will be used to store value of removed element.
         *                          Default value is 0 to indicate that it should be discarded.
         *
         * @return CAPU_OK if remove is successful
         *         CAPU_ENOT_EXIST if the key was not found in the map.
         */
        status_t remove(const Key& key, T* value_old = 0);

        /**
         * Remove the element where the iterator is pointing to
         * @param the iterator to the element to remove
         * @param out parameter to the removed element
         * @return   CAPU_OK if remove is successful
         *           CAPU_ENOT_EXISTS if the key was not found in the map
         */
        status_t remove(Iterator& iter, T* value_old = 0);

        /**
         * Returns count of the hashtable.
         * @return number of elements in hash table
         */
        uint_t count() const;

        /**
         * Clears all keys and values of the hashtable.
         */
        void clear();

        /**
         * Returns an iterator for iterating over the key and values in the map.
         * @return Iterator
         */
        Iterator begin();

        /**
         * Returns a ConstIterator for iterating over the key and values in the map.
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
         * Assignment operator for HashTable
         * @param HashTable to copy from
         * @return reference to HashTable with copied data
         */
        HashTable<Key, T, C, H>& operator=(const HashTable<Key, T, C, H>& other);

    private:
        // defines the threshold after which the list will get resized.
#define DEFAULT_HASH_TABLE_MAX_LOAD_FACTOR 0.8f
#define DEFAULT_HASH_TABLE_BIT_SIZE 4
        uint8_t mBitCount; // bit size for the hash function
        uint_t mSize; // the size of the data list
        uint_t mThreshold; // defines when a rehashing may occur
        HashTableEntry** mBuckets; // bucket list
        HashTableEntry* mData; // placeholder for the data
        HashTableEntry* mLastHashMapEntry; // pointer to the last entry in the map
        HashTableEntry* mFirstFreeHashMapEntry; // start of pointer list of free entries
        uint_t mCount; // the current entry count
        const bool_t mResizeable; // indicates if rehashing will be done
        const C mComparator; // compares keys

        void rehash();
        uint_t calcHashValue(const Key& key) const;
        HashTableEntry* internalGet(const Key& key) const;
        void internalPut(HashTableEntry* entry, uint_t hashValue);
        void internalRemove(HashTableEntry* entry, const uint_t hashValue, T* value_old = 0);
    };

    template <class Key, class T, class C, class H>
    inline HashTable<Key, T, C, H>::HashTable()
        : mBitCount(DEFAULT_HASH_TABLE_BIT_SIZE)
        , mSize(static_cast<uint_t>(1) << mBitCount)
        , mThreshold((static_cast<uint_t>((mSize) * DEFAULT_HASH_TABLE_MAX_LOAD_FACTOR)))
        , mBuckets(new HashTableEntry*[mSize])
        , mData(new HashTableEntry[mThreshold + 1])  // One dummy for the end
        , mLastHashMapEntry(mData + mThreshold)
        , mFirstFreeHashMapEntry(mData)
        , mCount(0)
        , mResizeable(true)
        , mComparator()
    {
        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize);
        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;
    }

    template <class Key, class T, class C, class H>
    inline HashTable<Key, T, C, H>::HashTable(const HashTable<Key, T, C, H>& other)
        : mBitCount(other.mBitCount)
        , mSize(other.mSize)
        , mThreshold(other.mThreshold)
        , mBuckets(new HashTableEntry*[mSize])
        , mData(new HashTableEntry[mThreshold + 1])  //One dummy for the end
        , mLastHashMapEntry(mData + mThreshold)
        , mFirstFreeHashMapEntry(mData)
        , mCount(0) // will get increased by internalPut
        , mResizeable(other.mResizeable)
        , mComparator()
    {
        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize);
        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;

        // right here, we have an empty map with the exact same size as the given other table
        // now we put each entry of the other map into this map (this may be a lot less than the actual size)

        typename HashTable<Key, T, C, H>::ConstIterator iter = other.begin();
        while (iter != other.end())
        {
            mFirstFreeHashMapEntry->internalKey = iter->key;
            mFirstFreeHashMapEntry->value = iter->value;
            internalPut(mFirstFreeHashMapEntry, calcHashValue(iter->key));
            ++iter;
            ++mFirstFreeHashMapEntry;
        }
    }

    template <class Key, class T, class C, class H>
    inline HashTable<Key, T, C, H>::HashTable(const uint8_t initialBitSize, const bool_t resizeable)
        : mBitCount(initialBitSize)
        , mSize(static_cast<uint_t>(1) << mBitCount)
        , mThreshold((static_cast<uint_t>((mSize) * DEFAULT_HASH_TABLE_MAX_LOAD_FACTOR)))
        , mBuckets(new HashTableEntry*[mSize])
        , mData(new HashTableEntry[mThreshold + 1])  //One dummy for the end
        , mLastHashMapEntry(mData + mThreshold)
        , mFirstFreeHashMapEntry(mData)
        , mCount(0)
        , mResizeable(resizeable)
        , mComparator()
    {
        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize);
        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;
    }

    template <class Key, class T, class C, class H>
    inline HashTable<Key, T, C, H>& HashTable<Key, T, C, H>::operator=(const HashTable<Key, T, C, H>& other)
    {
        if (&other == this)
        {
            // self assignment
            return *this;
        }
        if (!mResizeable && mSize != other.mSize)
        {
            // no modification allowed
            return *this;
        }
        if (0 != mBuckets)
        {
            delete[] mBuckets;
        }
        if (0 != mData)
        {
            delete[] mData;
        }

        mCount = 0;
        mSize = other.mSize;
        mBitCount = other.mBitCount;
        mThreshold = other.mThreshold;
        mBuckets = new HashTableEntry*[mSize];
        mData    = new HashTableEntry[mThreshold + 1];

        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize);
        mFirstFreeHashMapEntry = mData;
        mLastHashMapEntry = mData + mThreshold;
        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;

        HashTable::ConstIterator iter = other.begin();
        while (iter != other.end())
        {
            HashTableEntry entry = *iter;
            iter++;
            put(entry.key, entry.value);
        }

        return *this;
    }

    template <class Key, class T, class C, class H>
    inline HashTable<Key, T, C, H>::~HashTable()
    {
        delete[] mBuckets;
        delete[] mData;
    }

    template <class Key, class T, class C, class H>
    inline uint_t HashTable<Key, T, C, H>::count() const
    {
        return mCount;
    }

    template <class Key, class T, class C, class H>
    inline bool_t HashTable<Key, T, C, H>::contains(const Key& key) const
    {
        return internalGet(key) != 0;
    }

    template <class Key, class T, class C, class H>
    inline uint_t HashTable<Key, T, C, H>::calcHashValue(const Key& key) const
    {
        return H::Digest(key, mBitCount);
    }

    template <class Key, class T, class C, class H>
    inline T& HashTable<Key, T, C, H>::operator[](const Key& key)
    {
        status_t returnCode = CAPU_OK;
        T& value = at(key, &returnCode);
        if (returnCode == CAPU_OK)
        {
            return value;
        }
        //if key is not in hash table, add default constructed value to it
        put(key, T());
        return at(key);
    }

    template <class Key, class T, class C, class H>
    inline status_t HashTable<Key, T, C, H>::put(const Key& key, const T& value, T* oldValue)
    {
        uint_t hashValue = calcHashValue(key);

        // check if we already have the key in the map, if so, just override the value
        HashTableEntry* current = mBuckets[hashValue];
        if (current)
        {
            do
            {
                if (mComparator(current->key, key))
                {
                    if (oldValue)
                    {
                        *oldValue = current->value;
                    }
                    current->value = value;
                    return CAPU_OK;
                }
                current = current->next;
            }
            while (current->isChainElement);
        }

        // check if the next free entry is outside of the threshold (resizing would be necessary)
        if (mFirstFreeHashMapEntry == mData + mThreshold)
        {
            if (!mResizeable)
            {
                // if resizing is disabled, we're done here!
                return CAPU_ENO_MEMORY;
            }
            rehash();
            hashValue = calcHashValue(key); // calculate the new index (in the resized map)
        }

        // the entry holding the values is the first free entry
        HashTableEntry* newentry = mFirstFreeHashMapEntry;

        // adjust the pointer to the next free entry ('preconnected' through constructor)
        mFirstFreeHashMapEntry = mFirstFreeHashMapEntry->next;

        newentry->internalKey = key;   // copy operation
        newentry->value = value; // copy operation

        internalPut(newentry, hashValue);

        // done
        return CAPU_OK;
    }

    template <class Key, class T, class C, class H>
    const T& HashTable<Key, T, C, H>::at(const Key& key, status_t* returnCode) const
    {
        HashTableEntry* entry = internalGet(key);
        if (entry)
        {
            if (returnCode)
            {
                *returnCode = CAPU_OK;
            }
            return entry->value;
        }

        if (returnCode)
        {
            *returnCode = CAPU_ENOT_EXIST;
        }

        return mLastHashMapEntry->value;
    }

    template <class Key, class T, class C, class H>
    T& HashTable<Key, T, C, H>::at(const Key& key, status_t* returnCode)
    {
        HashTableEntry* entry = internalGet(key);
        if (entry)
        {
            if (returnCode)
            {
                *returnCode = CAPU_OK;
            }
            return entry->value;
        }

        if (returnCode)
        {
            *returnCode = CAPU_ENOT_EXIST;
        }

        return mLastHashMapEntry->value;
    }

    template <class Key, class T, class C, class H>
    typename HashTable<Key, T, C, H>::Iterator HashTable<Key, T, C, H>::find(const Key& key)
    {
        if (HashTableEntry* entry = internalGet(key))
        {
            // set new iterator to this position
            Iterator it(entry, mLastHashMapEntry);
            return it;
        }

        // no entry found -> return end iterator
        return end();
    }

    template <class Key, class T, class C, class H>
    typename HashTable<Key, T, C, H>::ConstIterator HashTable<Key, T, C, H>::find(const Key& key) const
    {
        if (HashTableEntry* entry = internalGet(key))
        {
            // set new iterator to this position
            ConstIterator it(entry, mLastHashMapEntry);
            return it;
        }

        // no entry found -> return end iterator
        return end();
    }

    template <class Key, class T, class C, class H>
    inline status_t HashTable<Key, T, C, H>::remove(const Key& key, T* value_old)
    {
        const uint_t hashValue = calcHashValue(key);
        HashTableEntry* current = mBuckets[hashValue];
        if (current)
        {
            do
            {
                if (mComparator(current->key, key))
                {
                    internalRemove(current, hashValue, value_old);

                    // done
                    return CAPU_OK;
                }
                current = current->next;
            }
            while (current->isChainElement);
        }

        // element was not found
        return CAPU_ERANGE;
    }

    template <class Key, class T, class C, class H>
    inline status_t HashTable<Key, T, C, H>::remove(Iterator& iter, T* value_old)
    {
        HashTableEntry* current = iter.mCurrentHashMapEntry;
        uint_t hashValue = calcHashValue(current->key);

        iter.mCurrentHashMapEntry = current->next;

        internalRemove(current, hashValue, value_old);

        // done
        return CAPU_OK;
    }

    template <class Key, class T, class C, class H>
    inline void HashTable<Key, T, C, H>::internalRemove(HashTableEntry* entry, const uint_t hashValue, T* value_old)
    {
        if (value_old)
        {
            // perform the copy operation into the old value
            *value_old = entry->value;
        }

        // change the pointer to point to the next element,
        // so that the current element is taken out of the chain
        entry->previous->next = entry->next;
        entry->next->previous = entry->previous;
        if (mBuckets[hashValue] == entry)
        {
            mBuckets[hashValue] = entry->next && entry->next->isChainElement ? entry->next : 0;
            entry->next->isChainElement = 0;
        }

        // connect the unused entries:
        // our next entry is the current first unused entry
        // and we are the new first free entry.
        entry->next          = mFirstFreeHashMapEntry;
        mFirstFreeHashMapEntry = entry;
        mFirstFreeHashMapEntry->previous = mLastHashMapEntry;

        // decrease map count and adjust begin pointer
        --mCount;
    }

    template <class Key, class T, class C, class H>
    inline void HashTable<Key, T, C, H>::clear()
    {
        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize);
        HashTableEntry* entry = mData;
        for (capu::uint32_t i = 0; i < mThreshold + 1; ++i)
        {
            entry->previous = 0;
            entry->next = entry + 1;
            entry->isChainElement = 0;
            ++entry;
        }
        mFirstFreeHashMapEntry = mData;
        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;
        mCount = 0;
    }

    template <class Key, class T, class C, class H>
    inline typename HashTable<Key, T, C, H>::Iterator HashTable<Key, T, C, H>::begin()
    {
        return typename HashTable<Key, T, C, H>::Iterator(mLastHashMapEntry->next, mLastHashMapEntry);
    }

    template <class Key, class T, class C, class H>
    inline typename HashTable<Key, T, C, H>::ConstIterator HashTable<Key, T, C, H>::begin() const
    {
        return typename HashTable<Key, T, C, H>::ConstIterator(mLastHashMapEntry->next, mLastHashMapEntry);
    }

    template <class Key, class T, class C, class H>
    inline typename HashTable<Key, T, C, H>::Iterator HashTable<Key, T, C, H>::end()
    {
        return typename HashTable<Key, T, C, H>::Iterator(mLastHashMapEntry, mLastHashMapEntry);
    }

    template <class Key, class T, class C, class H>
    inline typename HashTable<Key, T, C, H>::ConstIterator HashTable<Key, T, C, H>::end() const
    {
        return typename HashTable<Key, T, C, H>::ConstIterator(mLastHashMapEntry, mLastHashMapEntry);
    }

    template <class Key, class T, class C, class H>
    inline typename HashTable<Key, T, C, H>::HashTableEntry* HashTable<Key, T, C, H>::internalGet(const Key& key) const
    {
        const uint_t hashValue = calcHashValue(key);
        HashTableEntry* current = mBuckets[hashValue];
        if (current)
        {
            do
            {
                if (mComparator(current->key, key))
                {
                    return current;
                }
                current = current->next;
            }
            while (current->isChainElement);
        }

        return 0;
    }

    template <class Key, class T, class C, class H>
    inline void HashTable<Key, T, C, H>::internalPut(HashTableEntry* newentry, const uint_t hashValue)
    {
        // chaining of entries
        HashTableEntry* entry = mBuckets[hashValue];
        if (entry)
        {
            // chaining
            entry->isChainElement = 1;
        }
        else
        {
            // we hit a free bucket
            entry = mLastHashMapEntry->next;
        }

        mBuckets[hashValue]   = newentry;
        newentry->next        = entry;
        newentry->previous    = entry->previous;
        newentry->isChainElement = 0;
        entry->previous->next = newentry;
        entry->previous       = newentry;
        ++mCount;
    }

    template <class Key, class T, class C, class H>
    inline void HashTable<Key, T, C, H>::rehash()
    {
        // remember old values
        HashTableEntry*  old_data    = mData;
        HashTableEntry** old_buckets = mBuckets;
        uint_t old_threshold       = mThreshold;

        // prepare new values for the instance fields
        mCount                 = 0;
        mBitCount              = mBitCount + 1;
        mSize               = static_cast<uint_t>(static_cast<uint_t>(1) << mBitCount);
        mThreshold             = static_cast<uint_t>(mSize * DEFAULT_HASH_TABLE_MAX_LOAD_FACTOR);
        mBuckets               = new HashTableEntry*[mSize];
        mData                  = new HashTableEntry[mThreshold + 1];

        mFirstFreeHashMapEntry = mData + old_threshold;
        mLastHashMapEntry      = mData + mThreshold;

        // prepare new memory
        Memory::Set(mBuckets, 0, sizeof(HashTableEntry*) * mSize); // clear the new bucket list
        Memory::CopyObject(mData, old_data, old_threshold);

        mLastHashMapEntry->previous = mLastHashMapEntry;
        mLastHashMapEntry->next = mLastHashMapEntry;

        // now perform the rehashing on each entry
        HashTableEntry* newentry = mData;
        for (uint_t i = old_threshold; i != 0; --i)
        {
            internalPut(newentry, calcHashValue(newentry->key));
            ++newentry;
        }

        // cleanup old data
        delete[] old_buckets;
        delete[] old_data;
    }
}

#endif // CAPU_HASHTABLE_H

