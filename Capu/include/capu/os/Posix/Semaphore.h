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

#ifndef CAPU_UNIXBASED_SEMAPHORE_H
#define CAPU_UNIXBASED_SEMAPHORE_H

#include <semaphore.h>
#include <errno.h>
#include "capu/os/Time.h"

namespace capu
{
    namespace posix
    {
        class Semaphore
        {
        public:
            Semaphore(uint32_t initialPermits);
            ~Semaphore() ;
            status_t aquire();
            status_t tryAquire(uint32_t timeoutMillis);
            status_t release(uint32_t permits);
        private:
            sem_t mSemaphore;
        };

        inline
        Semaphore::Semaphore(uint32_t initialPermits)
        {
            sem_init(&mSemaphore, 0, initialPermits);
        }

        inline
        Semaphore::~Semaphore()
        {
            sem_destroy(&mSemaphore);
        }

        inline
        status_t Semaphore::aquire()
        {
            return tryAquire(0);
        }

        inline
        status_t
        Semaphore::tryAquire(uint32_t timeoutMillis)
        {
            int waitResult = 0;

            if (timeoutMillis != 0)
            {
                // use platform independent GetMilliseconds() function here because clock_gettime is not always available
                uint64_t endTime = Time::GetMilliseconds() + timeoutMillis;
                struct timespec timeout;
                timeout.tv_sec = endTime / 1000;
                timeout.tv_nsec = (endTime % 1000) * 1000000;

                while ((waitResult = sem_timedwait(&mSemaphore, &timeout)) == -1 && errno == EINTR)
                {
                    // loop until timeout or error
                    continue;
                }
            }
            else
            {
                waitResult = sem_wait(&mSemaphore);
            }

            if (0 == waitResult)
            {
                return CAPU_OK;
            }
            else
            {
                if (errno == ETIMEDOUT)
                {
                    return CAPU_ETIMEOUT;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }
        }

        inline
        status_t
        Semaphore::release(uint32_t permits)
        {
            uint32_t _permits = permits;
            bool    result  = true;
            while (_permits)
            {
                result &= sem_post(&mSemaphore) == 0;
                --_permits;
            }
            return result ? CAPU_OK : CAPU_ERROR;
        }
    }
}

#endif // CAPU_UNIXBASED_SEMAPHORE_H
