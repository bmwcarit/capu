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

#ifndef CAPU_IPHONEOS_ARMV7S_MEMORY_H
#define CAPU_IPHONEOS_ARMV7S_MEMORY_H

#include <capu/os/iPhoneOS/Memory.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Memory: private capu::os::Memory
            {
            public:
                using capu::os::Memory::Copy;
                using capu::os::Memory::CopyObject;
                using capu::os::Memory::Compare;
                using capu::os::Memory::Move;
                using capu::os::Memory::MoveObject;
                using capu::os::Memory::Set;
                using capu::os::Memory::CurrentMemoryUsage;
            };
        }
    }
}

#endif // CAPU_IPHONEOS_ARMV7S_MEMORY_H
