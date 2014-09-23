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
        static uint32_t AtomicAdd(volatile uint32_t& mem, uint32_t summand);

        /**
         * atomically add 'summand' to an int32_t
         * @param mem reference to the object
         * @param summand amount to add
         * @return returns the initial value of mem
         */
        static int32_t AtomicAdd(volatile int32_t& mem, int32_t summand);

        /**
         * atomically add 'summand' to an uint64_t
         * @param mem reference to the object
         * @param summand amount to add
         * @return returns the initial value of mem
         */
        static uint64_t AtomicAdd(volatile uint64_t& mem, uint64_t summand);

        /**
         * atomically add 'summand' to an int64_t
         * @param mem reference to the object
         * @param summand amount to add
         * @return returns the initial value of mem
         */
        static int64_t AtomicAdd(volatile int64_t& mem, int64_t summand);


        /**
         * atomically subtract 'substrahend' to an uint32_t
         * @param mem reference to the object
         * @param subtrahend amount to subtract
         * @return returns the initial value of mem
         */
        static uint32_t AtomicSub(volatile uint32_t& mem, uint32_t subtrahend);

        /**
         * atomically subtract 'substrahend' to an int32_t
         * @param mem reference to the object
         * @param subtrahend amount to subtract
         * @return returns the initial value of mem
         */
        static int32_t AtomicSub(volatile int32_t& mem, int32_t subtrahend);

        /**
         * atomically subtract 'substrahend' to an uint64_t
         * @param mem reference to the object
         * @param subtrahend amount to subtract
         * @return returns the initial value of mem
         */
        static uint64_t AtomicSub(volatile uint64_t& mem, uint64_t subtrahend);

        /**
         * atomically subtract 'substrahend' to an int64_t
         * @param mem reference to the object
         * @param subtrahend amount to subtract
         * @return returns the initial value of mem
         */
        static int64_t AtomicSub(volatile int64_t& mem, int64_t subtrahend);

        /**
         * atomically increment an uint32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint32_t AtomicInc(volatile uint32_t& mem);

        /**
         * atomically increment an int32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static int32_t  AtomicInc(volatile int32_t& mem);

        /**
         * atomically increment an uint64_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint64_t AtomicInc(volatile uint64_t& mem);

        /**
         * atomically increment an int64_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static int64_t  AtomicInc(volatile int64_t& mem);

        /**
         * atomically decrement an uint32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint32_t AtomicDec(volatile uint32_t& mem);

        /**
         * atomically decrement an int32_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static int32_t  AtomicDec(volatile int32_t& mem);

        /**
         * atomically decrement an uint64_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static uint64_t AtomicDec(volatile uint64_t& mem);

        /**
         * atomically decrement an int64_t
         * @param mem reference to the object
         * @return returns the initial value of mem
         */
        static int64_t  AtomicDec(volatile int64_t& mem);

    };


    inline
    uint32_t
    AtomicOperation::AtomicAdd(volatile uint32_t& mem, uint32_t summand)
    {
        return os::arch::AtomicOperation::AtomicAdd(mem, summand);
    }

    inline
    int32_t
    AtomicOperation::AtomicAdd(volatile int32_t& mem, int32_t summand)
    {
        return os::arch::AtomicOperation::AtomicAdd(mem, summand);
    }

    inline
    uint64_t
    AtomicOperation::AtomicAdd(volatile uint64_t& mem, uint64_t summand)
    {
        return os::arch::AtomicOperation::AtomicAdd(mem, summand);
    }

    inline
    int64_t
    AtomicOperation::AtomicAdd(volatile int64_t& mem, int64_t summand)
    {
        return os::arch::AtomicOperation::AtomicAdd(mem, summand);
    }


    inline
    uint32_t
    AtomicOperation::AtomicSub(volatile uint32_t& mem, uint32_t subtrahend)
    {
        return os::arch::AtomicOperation::AtomicSub(mem, subtrahend);
    }

    inline
    int32_t
    AtomicOperation::AtomicSub(volatile int32_t& mem, int32_t subtrahend)
    {
        return os::arch::AtomicOperation::AtomicSub(mem, subtrahend);
    }

    inline
    uint64_t
    AtomicOperation::AtomicSub(volatile uint64_t& mem, uint64_t subtrahend)
    {
        return os::arch::AtomicOperation::AtomicSub(mem, subtrahend);
    }

    inline
    int64_t
    AtomicOperation::AtomicSub(volatile int64_t& mem, int64_t subtrahend)
    {
        return os::arch::AtomicOperation::AtomicSub(mem, subtrahend);
    }


    inline
    uint32_t
    AtomicOperation::AtomicInc(volatile uint32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicInc(mem);
    }

    inline
    int32_t
    AtomicOperation::AtomicInc(volatile int32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicInc(mem);
    }

    inline
    uint64_t
    AtomicOperation::AtomicInc(volatile uint64_t& mem)
    {
        return os::arch::AtomicOperation::AtomicInc(mem);
    }

    inline
    int64_t
    AtomicOperation::AtomicInc(volatile int64_t& mem)
    {
        return os::arch::AtomicOperation::AtomicInc(mem);
    }

    inline
    uint32_t
    AtomicOperation::AtomicDec(volatile uint32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicDec(mem);
    }

    inline
    int32_t
    AtomicOperation::AtomicDec(volatile int32_t& mem)
    {
        return os::arch::AtomicOperation::AtomicDec(mem);
    }

    inline
    uint64_t
    AtomicOperation::AtomicDec(volatile uint64_t& mem)
    {
        return os::arch::AtomicOperation::AtomicDec(mem);
    }

    inline
    int64_t
    AtomicOperation::AtomicDec(volatile int64_t& mem)
    {
        return os::arch::AtomicOperation::AtomicDec(mem);
    }
}

#endif // CAPU_ATOMIC_OPERATION_H
