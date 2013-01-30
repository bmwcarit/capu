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

#ifndef CAPU_STATICALLOCATOR_H
#define CAPU_STATICALLOCATOR_H

#include <capu/Config.h>

namespace capu
{
    template<typename T, uint32_t COUNT>
    class StaticAllocator
    {
    public:
        StaticAllocator();

        typedef T* pointer;
        typedef const T* const_pointer;

        pointer allocate();
        void deallocate(pointer& ptr);

        /**
        * Checks if the given pointer is inside the managed memory of the allocator.
        * @param ptr The pointer to check.
        * @return True if the given pointer is managed by the allocator, false otherwise.
        */
        bool_t isManagedMemory(T* ptr);

    protected:
    private:
        struct MemoryEntry
        {
            char_t       element[sizeof(T)];
            MemoryEntry* nextFreeEntry;

            MemoryEntry()
            {
                nextFreeEntry = this + 1; // Preconnect memory entries
            }
        };

        MemoryEntry  mData[COUNT];
        MemoryEntry* mFreeEntry;
        uint32_t     mFreeCount;
    };

    template<typename T, uint32_t COUNT>
    inline StaticAllocator<T, COUNT>::StaticAllocator()
        : mFreeEntry(mData)
        , mFreeCount(COUNT)
    {
        mData[COUNT - 1].nextFreeEntry = 0; // repair last entry
    }

    template<typename T, uint32_t COUNT>
    inline typename StaticAllocator<T, COUNT>::pointer StaticAllocator<T, COUNT>::allocate()
    {
        T* result = 0;

        if (0 != mFreeEntry)
        {
            void* memory = &mFreeEntry->element;

// disable Visual Studio specific warning
#if (_MSC_VER >= 1400)
#pragma warning(disable : 4345)
#endif

            // construct object of type T at memory
            result = new(memory) T();

#if (_MSC_VER >= 1400)
#pragma warning(default : 4345)
#endif
            mFreeEntry = mFreeEntry->nextFreeEntry;
            --mFreeCount;
        }

        return result;
    }

    template<typename T, uint32_t COUNT>
    inline bool_t StaticAllocator<T, COUNT>::isManagedMemory(T* ptr)
    {
        // do some pointer arithmetic to check if the given ptr is inside our memory bounds
        MemoryEntry* toCheck = reinterpret_cast<MemoryEntry*>(ptr);
        MemoryEntry* myStart = reinterpret_cast<MemoryEntry*>(&mData[0]);
        MemoryEntry* myEnd = reinterpret_cast<MemoryEntry*>(&mData[COUNT-1]);
        return toCheck >= myStart && toCheck <= myEnd;
    }

    template<typename T, uint32_t COUNT>
    inline void StaticAllocator<T, COUNT>::deallocate(typename StaticAllocator<T, COUNT>::pointer& ptr)
    {
        MemoryEntry* entry = reinterpret_cast<MemoryEntry*>(ptr); // the MemoryEntry should be at the same position as the T*

        entry->nextFreeEntry = mFreeEntry;
        mFreeEntry = entry;

        --mFreeCount;
        ptr->~T();
        ptr = 0;
    }
}

#endif // CAPU_StaticAllocator_H
