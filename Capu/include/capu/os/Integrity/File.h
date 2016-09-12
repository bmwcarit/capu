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
#define PATH_MAX __ABS_PATH_MAX // file system max path length
#include <capu/os/Posix/File.h>

#define STREAM_BUFFER_SIZE (2 * 1024 * 1024)

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

        private:
            char* mStreamBuffer;
        };

        inline
        File::File(const String& path)
            : capu::posix::File(path)
            , mStreamBuffer(NULL)
        {
        }

        inline
        File::File(const File& parent, const capu::String& path)
            : capu::posix::File(parent, path)
            , mStreamBuffer(NULL)
        {
        }

        inline
        status_t File::open(const FileMode& mode)
        {
            status_t status = capu::posix::File::open(mode);
            if (status == CAPU_OK && mStreamBuffer == NULL)
            {
                //the stream buffer is a block of data that acts as intermediary between the i/o operations and the physical file associated to the stream
                //activating it here in order to improve file operation performance
                mStreamBuffer = (char*) malloc(STREAM_BUFFER_SIZE);
                int returnCode = setvbuf(mHandle, mStreamBuffer, _IOFBF, STREAM_BUFFER_SIZE);
                if (returnCode != 0)
                {
                    status = CAPU_ERROR;
                }
            }

            return status;
        }

        inline
        status_t File::close()
        {
            status_t status = capu::posix::File::close();
            if (NULL != mStreamBuffer)
            {
                free(mStreamBuffer);
                mStreamBuffer = NULL;
            }
            return status;
        }

    }
}
#endif // CAPU_INTEGRITY_ARM_V7L_FILE_H
