/*
 * Copyright (C) 2014 jambit GmbH
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

#ifndef CAPU_IPHONEOS_STRINGUTILS_H
#define CAPU_IPHONEOS_STRINGUTILS_H

#include <capu/os/Posix/StringUtils.h>

namespace capu
{
    namespace iphoneos
    {
        class StringUtils: private capu::posix::StringUtils
        {
        public:
            using capu::posix::StringUtils::Strncpy;
            using capu::posix::StringUtils::Sprintf;
            using capu::posix::StringUtils::Vsprintf;
            using capu::posix::StringUtils::Vscprintf;
            using capu::posix::StringUtils::Strlen;
            using capu::posix::StringUtils::Strcmp;
            using capu::posix::StringUtils::LastIndexOf;
            using capu::posix::StringUtils::IndexOf;
            using capu::posix::StringUtils::StartsWith;
        };
    }
}
#endif // CAPU_IPHONEOS_STRINGUTILS_H
