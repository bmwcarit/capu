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

#ifndef CAPU_IPHONESIMULATOROS_X86_64_FILE_H
#define CAPU_IPHONESIMULATOROS_X86_64_FILE_H

#include <capu/os/iPhoneOS/File.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class File: private capu::iphoneos::File
            {
            public:
                File(const String& path);
                File(const File& parent, const capu::String& path);
                using capu::iphoneos::File::open;
                using capu::iphoneos::File::isOpen;
                using capu::iphoneos::File::isEof;
                using capu::iphoneos::File::read;
                using capu::iphoneos::File::write;
                using capu::iphoneos::File::flush;
                using capu::iphoneos::File::close;
                using capu::iphoneos::File::createFile;
                using capu::iphoneos::File::createDirectory;
                using capu::iphoneos::File::remove;
                using capu::iphoneos::File::exists;
                using capu::iphoneos::File::renameTo;
                using capu::iphoneos::File::copyTo;
                using capu::iphoneos::File::getFileName;
                using capu::iphoneos::File::getExtension;
                using capu::iphoneos::File::getPath;
                using capu::iphoneos::File::getParentPath;
                using capu::iphoneos::File::getSizeInBytes;
                using capu::iphoneos::File::isDirectory;
            };

            inline
            File::File(const String& path)
                : capu::iphoneos::File(path)
            {
            }

            inline
            File::File(const File& parent, const capu::String& path)
                : capu::iphoneos::File(parent, path)
            {
            }
        }
    }
}
#endif
