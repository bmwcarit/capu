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

#ifndef CAPU_INTEGRITY_FILE_H
#define CAPU_INTEGRITY_FILE_H

#include <limits>
#ifdef PATH_MAX
#undef PATH_MAX
#endif
#define PATH_MAX __ABS_PATH_MAX // file system max path length
#include <capu/os/Posix/File.h>

namespace capu
{
    namespace os
    {
        class File: private capu::posix::File
        {
        public:
            File(const String& path);
            File(const File& parent, const capu::String& path);

            status_t open(const FileMode& mode);
            status_t close();

            using capu::posix::File::isOpen;
            using capu::posix::File::isEof;
            using capu::posix::File::read;
            using capu::posix::File::write;
            using capu::posix::File::seek;
            using capu::posix::File::getCurrentPosition;
            using capu::posix::File::flush;
            using capu::posix::File::renameTo;
            using capu::posix::File::createFile;
            using capu::posix::File::createDirectory;
            using capu::posix::File::remove;
            using capu::posix::File::exists;
            using capu::posix::File::getFileName;
            using capu::posix::File::getExtension;
            using capu::posix::File::getPath;
            using capu::posix::File::getParentPath;
            using capu::posix::File::getSizeInBytes;
            using capu::posix::File::isDirectory;
            using capu::posix::File::copyTo;

        };

        inline
        File::File(const String& path)
            : capu::posix::File(path)
        {
        }

        inline
        File::File(const File& parent, const capu::String& path)
            : capu::posix::File(parent, path)
        {
        }

        inline
        status_t File::open(const FileMode& mode)
        {
            return capu::posix::File::open(mode);
        }

        inline
        status_t File::close()
        {
            return capu::posix::File::close();
        }

    }
}
#endif // CAPU_INTEGRITY_ARM_V7L_FILE_H
