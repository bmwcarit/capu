/*
 * Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_MACOSX_SEMAPHORE_H
#define CAPU_MACOSX_SEMAPHORE_H

#include "capu/Config.h"
#include "capu/Error.h"
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/mach_traps.h>
#include <mach/mach.h>

namespace capu
{
    namespace os
    {
        class Semaphore
        {
        public:
            Semaphore(uint32_t initialPermits);
            ~Semaphore();
            status_t aquire();
            status_t tryAquire(uint32_t timeoutMillis);
            status_t release(uint32_t permits);
        private:
            semaphore_t mSemaphore;
            mach_port_t mTask;
        };

        inline
        Semaphore::Semaphore(uint32_t initialPermits)
        {
            UNUSED(initialPermits);
            mTask = mach_task_self();
            semaphore_create(mTask, &mSemaphore, SYNC_POLICY_FIFO, 0);
        }

        inline
        Semaphore::~Semaphore()
        {
            semaphore_destroy(mTask, mSemaphore);
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
            kern_return_t waitResult;

            if (timeoutMillis != 0)
            {
                mach_timespec_t timeout;
                timeout.tv_sec = timeoutMillis / 1000;
                timeout.tv_nsec = (timeoutMillis % 1000) * 1000000;

                bool_t keepWaiting = true;
                while (keepWaiting)
                {
                    waitResult = semaphore_timedwait(mSemaphore, timeout);
                    if (waitResult == KERN_SUCCESS || waitResult == KERN_OPERATION_TIMED_OUT)
                    {
                        keepWaiting = false;
                    }
                }
            }
            else
            {
                waitResult = semaphore_wait(mSemaphore);
            }

            if (KERN_SUCCESS == waitResult)
            {
                return CAPU_OK;
            }
            else
            {
                if (waitResult == KERN_OPERATION_TIMED_OUT)
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
            bool_t result  = true;
            while (_permits)
            {
                result &= semaphore_signal(mSemaphore) == KERN_SUCCESS;
                --_permits;
            }
            return result ? CAPU_OK : CAPU_ERROR;
        }
    }
}

#endif // CAPU_MACOSX_SEMAPHORE_H
