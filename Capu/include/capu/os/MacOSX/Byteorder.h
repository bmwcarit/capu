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

#ifndef CAPU_MACOSX_BYTEORDER_H
#define CAPU_MACOSX_BYTEORDER_H

#include <capu/os/Posix/Byteorder.h>

namespace capu
{
    namespace os
    {
        class Byteorder : private capu::posix::Byteorder
        {
        public:
            using capu::posix::Byteorder::NtoHl;
            using capu::posix::Byteorder::NtoHs;
            using capu::posix::Byteorder::HtoNl;
            using capu::posix::Byteorder::HtoNs;
        };
    }
}
#endif // CAPU_MACOSX_BYTEORDER_H
