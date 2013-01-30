/*
 * Copyright 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAPU_HYBRIDALLOCATOR_H
#define CAPU_HYBRIDALLOCATOR_H

#include "capu/util/StaticAllocator.h"
#include "capu/util/Allocator.h"

namespace capu
{
    /**
    * Allocator class that has a defined amount of static memory. If the memory is exhausted, dynamic memory is allocated, which is the
    * hybrid approach. As soon as static memory is available after a deallocation, it will get reused.
    */
    template<class T, uint32_t COUNT>
    class HybridAllocator
    {
    public:
        T* allocate();
        void deallocate(T*& ptr);

    private:
        StaticAllocator<T, COUNT> m_staticMemory;
        Allocator<T> m_dynamicMemory;
    };

    template<class T, uint32_t COUNT>
    inline T* HybridAllocator<T, COUNT>::allocate()
    {
        T* element = m_staticMemory.allocate();
        if (!element)
        {
            // static allocation failed (all memory in use)
            // -> dynamic memory allocation necessary
            element = m_dynamicMemory.allocate();
        }
        return element;
    }

    template<class T, uint32_t COUNT>
    inline void HybridAllocator<T, COUNT>::deallocate(T*& ptr)
    {
        // call correct deallocator
        if (m_staticMemory.isManagedMemory(ptr))
        {
            m_staticMemory.deallocate(ptr);
        }
        else
        {
            m_dynamicMemory.deallocate(ptr);
        }
    }
}

#endif // CAPU_HYBRIDALLOCATOR_H
