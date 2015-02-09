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

#ifndef CAPU_IPHONEOS_ARM_DYNAMICLIBRARY_H
#define CAPU_IPHONEOS_ARM_DYNAMICLIBRARY_H

#include <capu/os/iPhoneOS/DynamicLibrary.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class DynamicLibrary: private capu::iphoneos::DynamicLibrary
            {
            public:
                DynamicLibrary(String path);
                using capu::iphoneos::DynamicLibrary::load;
                using capu::iphoneos::DynamicLibrary::unload;
                using capu::iphoneos::DynamicLibrary::loadSymbol;
                using capu::iphoneos::DynamicLibrary::getPath;
            };

            inline
            DynamicLibrary::DynamicLibrary(String path)
                : capu::iphoneos::DynamicLibrary(path)
            {
            }
        }
    }
}

#endif // CAPU_IPHONEOS_ARM_DYNAMICLIBRARY_H
