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

#ifndef CAPU_IPHONEOS_DYNAMICLIBRARY_H
#define CAPU_IPHONEOS_DYNAMICLIBRARY_H

#include <capu/os/Posix/DynamicLibrary.h>

namespace capu
{
    namespace iphoneos
    {
        class DynamicLibrary: private capu::posix::DynamicLibrary
        {
        public:
            DynamicLibrary(String path);
            using capu::posix::DynamicLibrary::load;
            using capu::posix::DynamicLibrary::unload;
            using capu::posix::DynamicLibrary::loadSymbol;
            using capu::posix::DynamicLibrary::getPath;
        };

        inline
        DynamicLibrary::DynamicLibrary(String path)
            : capu::posix::DynamicLibrary(path)
        {
        }
    }
}

#endif // CAPU_IPHONEOS_DYNAMICLIBRARY_H
