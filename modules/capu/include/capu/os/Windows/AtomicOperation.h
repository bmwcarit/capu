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

#ifndef CAPU_WINDOWS_ATOMICOPERATION_H
#define CAPU_WINDOWS_ATOMICOPERATION_H

#include <Windows.h>

namespace capu
{
    namespace os
    {
        class AtomicOperation
        {
        public:
            static uint32_t AtomicAdd32(volatile uint32_t& mem, uint32_t summand);
            static uint32_t AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend);
            static uint32_t AtomicInc32(volatile uint32_t& mem);
            static uint32_t AtomicDec32(volatile uint32_t& mem);
        };

        inline
        uint32_t
        AtomicOperation::AtomicAdd32(volatile uint32_t& mem, uint32_t summand)
        {
            return InterlockedExchangeAdd((long*)&mem, summand);
        }

        inline
        uint32_t
        AtomicOperation::AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend)
        {
            return InterlockedExchangeAdd((long*)&mem, 0 - subtrahend);
        }

        inline
        uint32_t
        AtomicOperation::AtomicInc32(volatile uint32_t& mem)
        {
            return AtomicAdd32(mem, 1);
        }

        inline
        uint32_t
        AtomicOperation::AtomicDec32(volatile uint32_t& mem)
        {
            return AtomicSub32(mem, 1);

        }
    }
}




#endif //CAPU_WINDOWS_ATOMICOPERATION_H
