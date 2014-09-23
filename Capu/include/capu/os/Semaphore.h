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

#ifndef CAPU_SEMAPHORE_H
#define CAPU_SEMAPHORE_H

#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>
#include CAPU_PLATFORM_INCLUDE(Semaphore)

namespace capu
{
    /**
     * Synchronization object to provide controlled access to a shared resource between multiple threads.
     */
    class Semaphore: private capu::os::arch::Semaphore
    {
    public:
        /**
         * Constructor.
         * @param initialPermits The amout of initial permits on the semaphore.
         */
        Semaphore(uint32_t initialPermits = 0);

        /**
         * Aquires a permit on the semaphore. Blocks until one is available.
         * @return CAPU_OK is the permit was aquired, CAPU_ERROR otherwise.
         */
        status_t aquire();

        /**
         * Aquires a permit on the semaphore. Blocks until one is available or the specified timeout occurred.
         * @param timeoutMillis The timeout. A timeout of 0 means to block forever (=aquire()).
         * @return CAPU_OK is the permit was aquired, CAPU_ETIMEOUT if a timeout occurred, CAPU_ERROR otherwise.
         */
        status_t tryAquire(uint32_t timeoutMillis);

        /**
         * Releases the semaphore, which means to return a permit.
         * @return CAPU_OK if the permit was returned successfully.
         */
        status_t release(uint32_t permits = 1);
    };

    inline
    Semaphore::Semaphore(uint32_t initialPermits)
        : capu::os::arch::Semaphore(initialPermits)
    {
    }

    inline
    status_t
    Semaphore::aquire()
    {
        return capu::os::arch::Semaphore::aquire();
    }

    inline
    status_t
    Semaphore::tryAquire(uint32_t timeoutMillis)
    {
        return capu::os::arch::Semaphore::tryAquire(timeoutMillis);
    }

    inline
    status_t
    Semaphore::release(uint32_t permits)
    {
        return capu::os::arch::Semaphore::release(permits);
    }
}

#endif //CAPU_SEMAPHORE_H
