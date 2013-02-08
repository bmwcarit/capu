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

#ifndef CAPU_ATOMIC_OPERATION_H
#define CAPU_ATOMIC_OPERATION_H

#include "capu/Config.h"
#include "capu/os/PlatformInclude.h"

#include CAPU_PLATFORM_INCLUDE(AtomicOperation)

namespace capu
{
    /**
     * Operations that are guaranteed to be atomic in operation.
     */
    class AtomicOperation: private os::arch::AtomicOperation
    {
    public:

        /**
         * atomically add 'summand' to an uint32_t
         * @param mem reference to the object
         * @param summand amount to add
         * @return returns the initial value of mem
         */
        static uint32_t AtomicAdd32(volatile uint32_t& mem, uint32_t summand);

        /**
         * atomically subtract 'substrahend' from an uint32_t
         * @param mem reference to the object
         * @param subtrahend amount to subtract
         * @return returns the initial value of mem
         */
        static uint32_t AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend);

        /**
         * atomically increment an uint32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint32_t AtomicInc32(volatile uint32_t& mem);

        /**
         * atomically decrement  an uint32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint32_t AtomicDec32(volatile uint32_t& mem);
    };


    inline
    uint32_t
    AtomicOperation::AtomicAdd32(volatile uint32_t& mem, uint32_t summand)
    {
        return os::arch::AtomicOperation::AtomicAdd32(mem, summand);
    }

    inline
    uint32_t
    AtomicOperation::AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend)
    {
        return os::arch::AtomicOperation::AtomicSub32(mem, subtrahend);
    }

    inline
    uint32_t
    AtomicOperation::AtomicInc32(volatile uint32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicInc32(mem);
    }

    inline
    uint32_t
    AtomicOperation::AtomicDec32(volatile uint32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicDec32(mem);
    }
}

#endif // CAPU_ATOMIC_OPERATION_H
