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

#include "capu/util/Traits.h"
#include <cstring>

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

        private:
            int_t m_member;
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

        template<typename T, int TYPE>
        struct CopyHelper
        {
            static void DeepCopyDescending(T* dst, const T* src, const uint_t count)
            {
                // default copy method: deep copy with assignment operator

                const T* currentSrc = &src[count];
                T* currentDst = &dst[count];
                switch (count)
                {
                default:
                {
                    const T* end = src + 10;
                    while (currentSrc > end)
                    {
                        *(--currentDst) = *(--currentSrc);
                    }
                }
                case 10 :
                    *(--currentDst) = *(--currentSrc);
                case 9  :
                    *(--currentDst) = *(--currentSrc);
                case 8  :
                    *(--currentDst) = *(--currentSrc);
                case 7  :
                    *(--currentDst) = *(--currentSrc);
                case 6  :
                    *(--currentDst) = *(--currentSrc);
                case 5  :
                    *(--currentDst) = *(--currentSrc);
                case 4  :
                    *(--currentDst) = *(--currentSrc);
                case 3  :
                    *(--currentDst) = *(--currentSrc);
                case 2  :
                    *(--currentDst) = *(--currentSrc);
                case 1  :
                    *(--currentDst) = *(--currentSrc);
                }
            }

            static void DeepCopyAscending(T* dst, const T* src, const uint_t count)
            {
                // default copy method: deep copy with assignment operator

                const T* currentSrc = &src[0];
                T* currentDst = &dst[0];

                const T* end = &src[count];
                while (currentSrc < end)
                {
                    *currentDst = *currentSrc;
                    ++currentDst;
                    ++currentSrc;
                }
            }

            static void Copy(T* dst, const T* src, const uint_t count)
            {
                DeepCopyDescending(dst, src, count);
            }
        };

        template<typename T>
        struct CopyHelper<T, CAPU_TYPE_PRIMITIVE>
        {
            static void Copy(T* dst, const T* src, const uint_t count)
            {
                Memory::Copy(dst, src, count * sizeof(T));
            }
        };

        template<typename T>
        struct CopyHelper<T, CAPU_TYPE_POINTER>
        {
            static void Copy(T* dst, const T* src, const uint_t count)
            {
                Memory::Copy(dst, src, count * sizeof(T));
            }
        };

        template<typename T>
        void Memory::CopyObject(T* dst, const T* src, const uint_t count)
        {
            CopyHelper<T, Type<T>::Identifier>::Copy(dst, src, count);
        }

        template<typename T, int TYPE>
        struct MoveHelper
        {
            static void Move(T* dst, const T* src, const uint_t size)
            {
                if (dst == src)
                {
                    // no need to copy
                    return;
                }

                // In order to handle overlapping moves the move direction must be taken
                // into account. If copying forward we must copy in descending order
                // and vice versa.
                if (dst > src)
                {
                    // copying forward -> copy in descending order
                    CopyHelper<T, TYPE>::DeepCopyDescending(dst, src, size);
                }
                else
                {
                    // copying backward -> copy in ascending order
                    CopyHelper<T, TYPE>::DeepCopyAscending(dst, src, size);
                }
            }
        };

        template<typename T>
        struct MoveHelper<T, CAPU_TYPE_PRIMITIVE>
        {
            static void Move(T* dst, const T* src, const uint_t count)
            {
                Memory::Move(dst, src, count * sizeof(T));
            }
        };

        template<typename T>
        struct MoveHelper<T, CAPU_TYPE_POINTER>
        {
            static void Move(T* dst, const T* src, const uint_t count)
            {
                Memory::Move(dst, src, count * sizeof(T));
            }
        };

        template<typename T>
        void Memory::MoveObject(T* dst, const T* src, const uint_t count)
        {
            MoveHelper<T, Type<T>::Identifier>::Move(dst, src, count);
        }
    }
}

#endif //CAPU_GENERIC_MEMORY_H
