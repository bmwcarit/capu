/*
 * Copyright (C) 2017 BMW Car IT GmbH
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

#ifndef CAPU_MACOSX_X86_32_BYTEORDER_H
#define CAPU_MACOSX_X86_32_BYTEORDER_H

#include <capu/os/MacOSX/Byteorder.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Byteorder : private os::Byteorder
            {
            public:
                using capu::os::Byteorder::NtoHl;
                using capu::os::Byteorder::NtoHs;
                using capu::os::Byteorder::HtoNl;
                using capu::os::Byteorder::HtoNs;
            };
        }
    }
}
#endif // CAPU_MACOSX_X86_32_BYTEORDER_H
