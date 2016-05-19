/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#ifndef CAPU_ANDROID_ARM_V7L_ATOMICOPERATION_H
#define CAPU_ANDROID_ARM_V7L_ATOMICOPERATION_H

#include <capu/os/Posix/AtomicOperation.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class AtomicOperation: private capu::os::AtomicOperation
            {
            public:
                using capu::os::AtomicOperation::AtomicLoad;
                using capu::os::AtomicOperation::AtomicStore;
                using capu::os::AtomicOperation::AtomicAdd;
                using capu::os::AtomicOperation::AtomicSub;
                using capu::os::AtomicOperation::AtomicInc;
                using capu::os::AtomicOperation::AtomicDec;
            };
        }
    }
}
#endif // CAPU_ANDROID_ARM_V7L_ATOMICOPERATION_H
