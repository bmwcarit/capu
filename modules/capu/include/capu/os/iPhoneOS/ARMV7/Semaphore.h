/*
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

#ifndef CAPU_IPHONEOS_ARMV7_SEMAPHORE_H
#define CAPU_IPHONEOS_ARMV7_SEMAPHORE_H

#include <capu/os/iPhoneOS/Semaphore.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Semaphore: private capu::iphoneos::Semaphore
            {
            public:
                Semaphore(uint32_t initialPermits);
                using capu::iphoneos::Semaphore::aquire;
                using capu::iphoneos::Semaphore::tryAquire;
                using capu::iphoneos::Semaphore::release;
            };

            inline
            Semaphore::Semaphore(uint32_t initialPermits)
                : capu::iphoneos::Semaphore::Semaphore(initialPermits)
            {
            }
        }
    }
}

#endif // CAPU_IPHONEOS_ARMV7_SEMAPHORE_H
