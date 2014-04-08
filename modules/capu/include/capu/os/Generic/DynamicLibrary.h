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

#ifndef CAPU_GENERIC_DYNAMICLIBRARY_H
#define CAPU_GENERIC_DYNAMICLIBRARY_H

#include "capu/container/String.h"

namespace capu
{
    namespace generic
    {
        class DynamicLibrary
        {
        public:
            DynamicLibrary(String path);
            ~DynamicLibrary();
            String getPath() const;
        protected:
            const String mPath;
        private:
            DynamicLibrary operator=(const DynamicLibrary dl);
        };

        inline
        DynamicLibrary::DynamicLibrary(String path)
            : mPath(path)
        {
        }

        inline
        DynamicLibrary::~DynamicLibrary()
        {
        }

        inline
        String DynamicLibrary::getPath() const
        {
            return mPath;
        }
    }
}

#endif //CAPU_GENERIC_DYNAMICLIBRARY_H
