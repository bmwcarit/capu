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

#ifndef CAPU_GENERIC_FILE_H
#define CAPU_GENERIC_FILE_H

#include "capu/container/String.h"

namespace capu
{
    namespace generic
    {
        class File
        {
        public:
            File(const String& path);
            File(const File& parent, const capu::String& path);
            const capu::String getFileName() const;
            const capu::String getExtension() const;
            const capu::String& getPath() const;
            ~File();

        protected:
            String mPath;
            FILE* mHandle;
        };

        inline
        File::File(const String& path)
            : mPath(path)
            , mHandle(NULL)
        {
        }

        inline
        File::File(const File& parent, const capu::String& path)
            : mPath(parent.getPath())
            , mHandle(NULL)
        {
            mPath.append("/").append(path);
        }

        inline
        File::~File()
        {
        }

        inline
        const capu::String& File::getPath() const
        {
            return mPath;
        }

        inline
        const capu::String File::getExtension() const
        {
            capu::int_t position = capu::StringUtils::LastIndexOf(mPath.c_str(), '.');
            if (position < 0)
            {
                // index not found
                return capu::String("");
            }
            return capu::String(mPath.c_str(), position + 1);
        }

        inline
        const capu::String File::getFileName() const
        {
            capu::int_t lastSeparator = capu::StringUtils::LastIndexOf(mPath.c_str(), '/');
            if (lastSeparator == -1)
            {
                lastSeparator = capu::StringUtils::LastIndexOf(mPath.c_str(), '\\');
            }
            if (lastSeparator != -1)
            {
                return capu::String(mPath.c_str(), lastSeparator + 1);
            }
            else
            {
                return mPath;
            }
        }
    }
}

#endif //CAPU_GENERIC_FILE_H
