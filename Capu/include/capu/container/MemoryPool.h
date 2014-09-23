/*
* Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_MEMORYPOOL_H
#define CAPU_MEMORYPOOL_H

#include "capu/Config.h"

namespace capu
{

    /**
     * The MemoryPool holds a given amount of items without creation and deletion on the heap 
     * during runtime
     */
    template<typename T, uint_t SIZE>
    class MemoryPool
    {
    private:

        /**
         * Internal struct for the date and a pointer to next entry
         */
        struct MemoryPoolEntry
        {
            MemoryPoolEntry* nextFreePointer;
            T m_data;
        };

    public:

        /**
         * Constructor of MemoryPool
         */
        MemoryPool();

        /**
         * Returns a pointer to a free element or 0 if pool is full
         * @ return a pointer to a free element of 0 if pool is full
         */
        T* getEntry();

        /**
         * Releases the given entry
         */
        void releaseEntry(const T* entry);

    private:
        /**
         * Internal array with the data
         */
        MemoryPoolEntry m_entries[SIZE];

        /**
         * Pointer to the next free entry or 0 if the pool is full
         */
        MemoryPoolEntry* m_nextFreeEntry;
    };

    template<typename T, uint_t SIZE>
    inline
    MemoryPool<T, SIZE>::MemoryPool()
        : m_nextFreeEntry(&m_entries[0])
    {
        MemoryPoolEntry* current = &m_entries[0];
        MemoryPoolEntry* end = &m_entries[SIZE - 1];
        end->nextFreePointer = 0;

        while (current < end)
        {
            current->nextFreePointer = (current + 1);
            ++current;
        }
    }

    template<typename T, uint_t SIZE>
    inline
    T* MemoryPool<T, SIZE>::getEntry()
    {
        T* result = 0;
        
        if (m_nextFreeEntry != 0)
        {
            result = &m_nextFreeEntry->m_data;
            m_nextFreeEntry = m_nextFreeEntry->nextFreePointer;
        }

        return result;
    }

    template<typename T, uint_t SIZE>
    inline
    void MemoryPool<T, SIZE>::releaseEntry(const T* entry)
    {
        MemoryPoolEntry* current = &m_entries[0];
        MemoryPoolEntry* end = &m_entries[SIZE - 1];

        while (end >= current)
        {
            if (&end->m_data == entry)
            {
                end->nextFreePointer = m_nextFreeEntry;
                m_nextFreeEntry = end;
                return;
            }
            --end;
        }
    }
}

#endif // CAPU_MEMORYPOOL_H