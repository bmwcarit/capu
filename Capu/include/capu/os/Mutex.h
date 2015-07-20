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

#ifndef CAPU_MUTEX_H
#define CAPU_MUTEX_H

#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(Mutex)

namespace capu
{
    /**
     * Mutex which can be used for locking
     */
    class Mutex: public capu::os::arch::Mutex
    {
    public:

        /**
         * Constructor
         */
        Mutex();

        /**
         * Destructor
         */
        ~Mutex();

        /**
         * used for locking if lock is not currently available, then wait until the lock is captured
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
    Mutex::Mutex()
    {

    }

    inline
    Mutex::~Mutex()
    {

    }

    inline
    status_t Mutex::lock()
    {
        return capu::os::arch::Mutex::lock();
    }

    inline
    bool   Mutex::trylock()
    {
        return capu::os::arch::Mutex::trylock();
    }

    inline
    status_t Mutex::unlock()
    {
        return capu::os::arch::Mutex::unlock();
    }
}

#endif //CAPU_MUTEX_H
