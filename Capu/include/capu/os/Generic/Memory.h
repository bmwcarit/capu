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

#ifndef CAPU_GENERIC_MEMORY_H
#define CAPU_GENERIC_MEMORY_H

#include "capu/Config.h"
#include <cstring>
#include <algorithm>

namespace capu
{
    namespace generic
    {
        class Memory
        {
        public:
            static void Set(void* dst, int32_t val    , uint_t size);
            static void Move(void* dst, const void* src, uint_t size);
            static int32_t Compare(const void* ptr1, const void* ptr2, uint_t num);
            static void Copy(void* dst, const void* src, const uint_t size);

            template<typename T>
            static void CopyObject(T* dst, const T* src, const uint_t size); // copy with assignment operator

            template<typename T>
            static void MoveObject(T* dst, const T* src, const uint_t size); // move with assignment operator
        };

        inline
        void Memory::Set(void* dst, int32_t val, uint_t size)
        {
            memset(dst, val, size);
        }

        inline
        void Memory::Move(void* dst, const void* src, uint_t size)
        {
            memmove(dst, src, size);
        }

        inline
        int32_t Memory::Compare(const void* ptr1, const void* ptr2, uint_t num)
        {
            return memcmp(ptr1, ptr2, num);
        }

        inline
        void Memory::Copy(void* dst, const void* src, const uint_t size)
        {
            memcpy(dst, src, size);
        }

        template<typename T>
        void Memory::CopyObject(T* dst, const T* src, const uint_t count)
        {
            std::copy(src, src + count, dst);
        }

        template<typename T>
        void Memory::MoveObject(T* dst, const T* src, const uint_t count)
        {
            // In order to handle overlapping moves the move direction must be taken
            // into account. If copying forward we must copy in descending order
            // and vice versa.
            if (dst > src)
            {
                std::copy_backward(src, src + count, dst + count);
            }
            else
            {
                std::copy(src, src + count, dst);
            }
        }
    }
}

#endif //CAPU_GENERIC_MEMORY_H
