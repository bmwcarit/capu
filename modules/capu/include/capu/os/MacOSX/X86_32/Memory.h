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

#ifndef CAPU_MACOSX_X86_32_MEMORY_H
#define CAPU_MACOSX_X86_32_MEMORY_H

#include <capu/os/Generic/Memory.h>
#include <mach/mach.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Memory: private capu::generic::Memory
            {
            public:
                using capu::generic::Memory::Copy;
                using capu::generic::Memory::CopyObject;
                using capu::generic::Memory::Compare;
                using capu::generic::Memory::Move;
                using capu::generic::Memory::MoveObject;
                using capu::generic::Memory::Set;
                static uint_t CurrentMemoryUsage();
            };

            inline uint_t Memory::CurrentMemoryUsage()
            {
                struct task_basic_info result;
                mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
                kern_return_t retVal = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t) &result, &infoCount);
                if (retVal != KERN_SUCCESS)
                {
                    // something went wrong!
                    return -1;
                }
                return result.virtual_size;
            }
        }
    }
}

#endif // CAPU_MACOSX_X86_32_MEMORY_H
