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

#ifndef CAPU_WINDOWS_SEMAPHORE_H
#define CAPU_WINDOWS_SEMAPHORE_H

#include <windows.h>

namespace capu
{
    namespace os
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
            HANDLE mSemaphore;
        };

        inline
        Semaphore::Semaphore(uint32_t initialPermits)
        {
            mSemaphore = CreateSemaphore(NULL, initialPermits, INT_MAX, NULL);
        }

        inline
        Semaphore::~Semaphore()
        {
            CloseHandle(mSemaphore);
        }

        inline
        status_t
        Semaphore::aquire()
        {
            return tryAquire(0);
        }

        inline
        status_t
        Semaphore::tryAquire(uint32_t timeoutMillis)
        {
            if (timeoutMillis == 0)
            {
                timeoutMillis = INFINITE;
            }

            DWORD waitResult = WaitForSingleObject(mSemaphore, timeoutMillis);
            if (waitResult == WAIT_TIMEOUT)
            {
                return CAPU_ETIMEOUT;
            }
            else if (waitResult == WAIT_FAILED)
            {
                return CAPU_ERROR;
            }
            else
            {
                return CAPU_OK;
            }
        }

        inline
        status_t
        Semaphore::release(uint32_t permits)
        {
            return ReleaseSemaphore(mSemaphore, permits, NULL) == TRUE ? CAPU_OK : CAPU_ERROR;
        }
    }
}

#endif //CAPU_WINDOWS_SEMAPHORE_H
