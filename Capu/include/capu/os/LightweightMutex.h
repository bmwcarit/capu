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

#ifndef CAPU_LIGHTWEIGHTMUTEX_H
#define CAPU_LIGHTWEIGHTMUTEX_H

#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(LightweightMutex)

namespace capu
{
    /**
     * LightweightMutex which can be used for non-recursive locking
     * Depending on the platform it might be faster as the regular mutex
     * Hint: at windows platforms the LightweightMutex is equivalent to the regular mutex
     */
    class LightweightMutex: public capu::os::arch::LightweightMutex
    {
    public:

        /**
         * Constructor
         */
        LightweightMutex();

        /**
         * Destructor
         */
        ~LightweightMutex();

        /**
         * used for locking if lock is not currently available, then wait until the lock is captured.
         * results in undefined behavior if already locked in the same thread.
         * @return CAPU_OK if the locking is successful
         *         CAPU_ERROR otherwise
         */
        status_t lock();

        /**
         * it will attempt to lock a mutex. However if the mutex is already locked, the routine will return false immediately
         * @return true if the mutex is successfully locked
         *         false if the mutex is already locked
         */
        bool trylock();

        /**
         *release the lock
         *@return CAPU_OK if the unlocking is successful
         *        CAPU_ERROR otherwise
         */
        status_t unlock();

    };

    inline
    LightweightMutex::LightweightMutex()
    {

    }

    inline
    LightweightMutex::~LightweightMutex()
    {

    }

    inline
    status_t LightweightMutex::lock()
    {
        return capu::os::arch::LightweightMutex::lock();
    }

    inline
    bool   LightweightMutex::trylock()
    {
        return capu::os::arch::LightweightMutex::trylock();
    }

    inline
    status_t LightweightMutex::unlock()
    {
        return capu::os::arch::LightweightMutex::unlock();
    }
}

#endif //CAPU_LIGHTWEIGHTMUTEX_H
