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

#ifndef CAPU_MACOSX_X86_32_FILE_H
#define CAPU_MACOSX_X86_32_FILE_H

#include <capu/os/MacOSX/File.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class File: private capu::os::File
            {
            public:
                File(const String& path);
                File(const File& parent, const capu::String& path);
                using capu::os::File::open;
                using capu::os::File::isOpen;
                using capu::os::File::isEof;
                using capu::os::File::read;
                using capu::os::File::write;
                using capu::os::File::flush;
                using capu::os::File::close;
                using capu::os::File::createFile;
                using capu::os::File::createDirectory;
                using capu::os::File::remove;
                using capu::os::File::exists;
                using capu::os::File::renameTo;
                using capu::os::File::copyTo;
                using capu::os::File::getFileName;
                using capu::os::File::getExtension;
                using capu::os::File::getPath;
                using capu::os::File::getParentPath;
                using capu::os::File::getSizeInBytes;
                using capu::os::File::isDirectory;
            };

            inline
            File::File(const String& path)
                : capu::os::File(path)
            {
            }

            inline
            File::File(const File& parent, const capu::String& path)
                : capu::os::File(parent, path)
            {
            }
        }
    }
}
#endif
