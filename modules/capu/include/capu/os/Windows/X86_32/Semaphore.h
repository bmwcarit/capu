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

#ifndef CAPU_WINDOWS_X86_32_SEMAPHORE_H
#define CAPU_WINDOWS_X86_32_SEMAPHORE_H

#include <capu/os/Windows/Semaphore.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Semaphore: private capu::os::Semaphore
            {
            public:
                Semaphore(uint32_t initialPermits);
                using capu::os::Semaphore::aquire;
                using capu::os::Semaphore::tryAquire;
                using capu::os::Semaphore::release;

            private:
                HANDLE mSemaphore;
            };

            inline
            Semaphore::Semaphore(uint32_t initialPermits)
                : capu::os::Semaphore(initialPermits)
            {
            }
        }
    }
}

#endif //CAPU_WINDOWS_X86_32_SEMAPHORE_H
