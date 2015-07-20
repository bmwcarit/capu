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

#ifndef CAPU_WINDOWS_MUTEX_H
#define CAPU_WINDOWS_MUTEX_H


#include <windows.h>
#include "capu/Config.h"
#include "capu/Error.h"

namespace capu
{
    namespace os
    {
        //forward declaration in order to use it in friend declaration
        class CondVar;

        class Mutex
        {
        public:
            friend class capu::os::CondVar;

            Mutex();
            ~Mutex();
            status_t lock();
            bool trylock();
            status_t unlock();

        private:
            CRITICAL_SECTION mLock;
        };

        inline
        Mutex::Mutex()
        {
            InitializeCriticalSection(&mLock);
        }

        inline
        Mutex::~Mutex()
        {
            DeleteCriticalSection(&mLock);
        }

        inline
        status_t Mutex::lock()
        {
            EnterCriticalSection(&mLock);
            return CAPU_OK;
        }

        inline
        status_t
        Mutex::unlock()
        {
            LeaveCriticalSection(&mLock);
            return CAPU_OK;
        }

        inline
        bool
        Mutex::trylock()
        {
            if (TryEnterCriticalSection(&mLock))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
#endif //CAPU_WINDOWS_MUTEX_H
