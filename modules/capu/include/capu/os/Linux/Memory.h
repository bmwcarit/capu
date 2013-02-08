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

#ifndef CAPU_LINUX_MEMORY_H_
#define CAPU_LINUX_MEMORY_H_

#include <capu/os/Posix/Memory.h>

namespace capu
{
    namespace os
    {
        class Memory: private capu::posix::Memory
        {
        public:
            using capu::posix::Memory::Set;
            using capu::posix::Memory::Move;
            using capu::posix::Memory::MoveObject;
            using capu::posix::Memory::Compare;
            using capu::posix::Memory::Copy;
            using capu::posix::Memory::CopyObject;
            using capu::posix::Memory::CurrentMemoryUsage;
        };
    }
}


#endif // CAPU_LINUX_MEMORY_H_
