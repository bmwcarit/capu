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

#ifndef CAPU_IPHONEOS_ARM_STRINGUTILS_H
#define CAPU_IPHONEOS_ARM_STRINGUTILS_H

#include <capu/os/iPhoneOS/StringUtils.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class StringUtils: private capu::iphoneos::StringUtils
            {
            public:
                using capu::iphoneos::StringUtils::Strncpy;
                using capu::iphoneos::StringUtils::Sprintf;
                using capu::iphoneos::StringUtils::Vsprintf;
                using capu::iphoneos::StringUtils::Vscprintf;
                using capu::iphoneos::StringUtils::Strlen;
                using capu::iphoneos::StringUtils::Strcmp;
                using capu::iphoneos::StringUtils::LastIndexOf;
                using capu::iphoneos::StringUtils::IndexOf;
                using capu::iphoneos::StringUtils::StartsWith;
            };
        }
    }
}
#endif // CAPU_IPHONEOS_ARM_STRINGUTILS_H
