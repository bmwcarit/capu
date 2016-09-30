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

#ifndef CAPU_WINDOWS_DYNAMICLIBRARY_H
#define CAPU_WINDOWS_DYNAMICLIBRARY_H

#include "capu/os/Windows/MinimalWindowsH.h"
#include "capu/os/Generic/DynamicLibrary.h"
#include "capu/Error.h"

namespace capu
{
    namespace os
    {
        class DynamicLibrary : private generic::DynamicLibrary
        {
        public:
            DynamicLibrary(String path);
            using generic::DynamicLibrary::getPath;
            status_t load();
            status_t unload();
            status_t loadSymbol(const String& name, void*& symbol);
            ~DynamicLibrary();
        private:
            HMODULE mHandle;
        };

        inline DynamicLibrary::DynamicLibrary(String path) : generic::DynamicLibrary(path), mHandle(0)
        {
        }

        inline DynamicLibrary::~DynamicLibrary()
        {
        }

        inline status_t DynamicLibrary::load()
        {
            if (mPath.getLength() == 0)
            {
                return CAPU_EINVAL;
            }

            if (mHandle != 0)
            {
                // already loaded
                return CAPU_ERROR;
            }

            mHandle = LoadLibraryA(mPath.c_str());

            if (mHandle == 0)
            {
                DWORD errorCode = GetLastError();
                if (errorCode == ERROR_MOD_NOT_FOUND)
                {
                    return CAPU_ENOT_EXIST;
                }
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline status_t DynamicLibrary::unload()
        {
            if (FreeLibrary(mHandle))
            {
                mHandle = 0;
            }
            else
            {
                // library was not unloaded (still references ?)
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline status_t DynamicLibrary::loadSymbol(const String& name, void*& symbol)
        {
            if (mHandle == 0)
            {
                // lib is not loaded, we cannot load a symbol!
                return CAPU_ERROR;
            }

            symbol = (void*) GetProcAddress(mHandle, name.c_str());
            if (symbol == 0)
            {
                return CAPU_ENOT_EXIST;
            }
            return CAPU_OK;
        }
    }
}

#endif // CAPU_WINDOWS_DYNAMICLIBRARY_H
