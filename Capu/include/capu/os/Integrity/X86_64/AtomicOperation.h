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

#ifndef CAPU_INTEGRITY_X86_64_ATOMICOPERATION_H
#define CAPU_INTEGRITY_X86_64_ATOMICOPERATION_H

#include "capu/Config.h"
#include <pthread.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class AtomicOperation
            {
            public:
                static uint32_t AtomicAdd(volatile uint32_t& mem, uint32_t summand);
                static int32_t  AtomicAdd(volatile int32_t& mem, int32_t summand);
                static uint64_t AtomicAdd(volatile uint64_t& mem, uint64_t summand);
                static int64_t  AtomicAdd(volatile int64_t& mem, int64_t summand);

                static uint32_t AtomicSub(volatile uint32_t& mem, uint32_t subtrahend);
                static int32_t  AtomicSub(volatile int32_t& mem, int32_t subtrahend);
                static uint64_t AtomicSub(volatile uint64_t& mem, uint64_t subtrahend);
                static int64_t  AtomicSub(volatile int64_t& mem, int64_t subtrahend);

                static uint32_t AtomicInc(volatile uint32_t& mem);
                static int32_t  AtomicInc(volatile int32_t& mem);
                static uint64_t AtomicInc(volatile uint64_t& mem);
                static int64_t  AtomicInc(volatile int64_t& mem);

                static uint32_t AtomicDec(volatile uint32_t& mem);
                static int32_t  AtomicDec(volatile int32_t& mem);
                static uint64_t AtomicDec(volatile uint64_t& mem);
                static int64_t  AtomicDec(volatile int64_t& mem);
            };

            inline
            uint32_t
            AtomicOperation::AtomicAdd(volatile uint32_t& mem, uint32_t summand)
            {
                uint32_t oldValue;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(summand));
                return oldValue;
            }

            inline
            uint32_t
            AtomicOperation::AtomicSub(volatile uint32_t& mem, uint32_t subtrahend)
            {
                uint32_t oldValue;
                int32_t subValue = -subtrahend;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(subValue));
                return oldValue;
            }

            inline
            uint32_t
            AtomicOperation::AtomicInc(volatile uint32_t& mem)
            {
                return AtomicAdd(mem, 1);
            }

            inline
            uint32_t
            AtomicOperation::AtomicDec(volatile uint32_t& mem)
            {
                return AtomicSub(mem, 1);
            }
            
            inline
            int32_t
            AtomicOperation::AtomicAdd(volatile int32_t& mem, int32_t summand)
            {
                int32_t oldValue;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(summand));
                return oldValue;
            }

            inline
            int32_t
            AtomicOperation::AtomicSub(volatile int32_t& mem, int32_t subtrahend)
            {
                int32_t oldValue;
                int32_t subValue = -subtrahend;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(subValue));
                return oldValue;
            }

            inline
            int32_t
            AtomicOperation::AtomicInc(volatile int32_t& mem)
            {
                return AtomicAdd(mem, 1);
            }

            inline
            int32_t
            AtomicOperation::AtomicDec(volatile int32_t& mem)
            {
                return AtomicSub(mem, 1);
            }
            
            inline
            uint64_t
            AtomicOperation::AtomicAdd(volatile uint64_t& mem, uint64_t summand)
            {
                uint64_t oldValue;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(summand));
                return oldValue;
            }

            inline
            uint64_t
            AtomicOperation::AtomicSub(volatile uint64_t& mem, uint64_t subtrahend)
            {
                uint64_t oldValue;
                int64_t subValue = -subtrahend;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(subValue));
                return oldValue;
            }

            inline
            uint64_t
            AtomicOperation::AtomicInc(volatile uint64_t& mem)
            {
                return AtomicAdd(mem, 1);
            }

            inline
            uint64_t
            AtomicOperation::AtomicDec(volatile uint64_t& mem)
            {
                return AtomicSub(mem, 1);
            }
            
            inline
            int64_t
            AtomicOperation::AtomicAdd(volatile int64_t& mem, int64_t summand)
            {
                int64_t oldValue;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(summand));
                return oldValue;
            }

            inline
            int64_t
            AtomicOperation::AtomicSub(volatile int64_t& mem, int64_t subtrahend)
            {
                int64_t oldValue;
                int64_t subValue = -subtrahend;
                AtomicModify((Address*)&mem, (Address*)&oldValue, 0, (Address)(subValue));
                return oldValue;
            }

            inline
            int64_t
            AtomicOperation::AtomicInc(volatile int64_t& mem)
            {
                return AtomicAdd(mem, 1);
            }

            inline
            int64_t
            AtomicOperation::AtomicDec(volatile int64_t& mem)
            {
                return AtomicSub(mem, 1);
            }
        }
    }
}
#endif // CAPU_INTEGRITY_X86_64_ATOMICOPERATION_H
