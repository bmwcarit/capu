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

#ifndef CAPU_DYNAMICLIBRARY_H
#define CAPU_DYNAMICLIBRARY_H

#include "capu/Config.h"
#include "capu/Error.h"
#include "capu/container/String.h"
#include "capu/os/PlatformInclude.h"

#include CAPU_PLATFORM_INCLUDE(DynamicLibrary)

//make symbols visible from extern
#if defined(_MSC_VER) // Visual Studio Compiler
#define EXPORT_FUNCTION __declspec(dllexport)
#elif defined(__GNUC__) // GCC
#define EXPORT_FUNCTION __attribute__ ((visibility("default")))
#elif defined(__ghs__) // Integrity
#define EXPORT_FUNCTION __attribute__((externally_visible))
#else
#warning EXPORT_FUNCTION is not defined for your compiler. Adjust capu/os/DynamicLibrary.h to get rid of the error message for your compiler.
#endif

namespace capu
{
    /**
     * Abstracts an OS dynamic shared library.
     */
    class DynamicLibrary: private os::arch::DynamicLibrary
    {
    public:
        /**
         * Constructor.
         * @param path Filename of the library to use
         */
        DynamicLibrary(String path);

        virtual ~DynamicLibrary();

        /**
         * Load the library, ie. create OS specific connection to library, load into address space
         * @return CAPU_OK on success, CAPU_ENOT_EXIST if file could not be found or opened, CAPU_ERROR on other erros.
         */
        virtual status_t load();

        /**
         * Unload library, remove from address space etc.
         * @return CAPU_OK on success, CAPU_ERROR if library could not be unloaded.
         */
        virtual status_t unload();

        /**
         * Loads an exported symbol out of the library.
         * @param name the name of the symbol.
         * @param symbol The pointer which is filled with the symbol.
         */
        virtual status_t loadSymbol(const String& name, void*& symbol);

        /**
         * Get path/filename of the OS library.
         * @return the filename or path.
         */
        virtual String getPath() const;

        /**
         * The default prefix of filenames of dynamic libraries (Platform dependent)
         */
        static const String defaultLibraryPrefix;

        /**
         * The default postfix of filenames of dynamic libraries (Platform dependent)
         */
        static const String defaultLibraryPostfix;
    };

    inline DynamicLibrary::DynamicLibrary(String path) : os::arch::DynamicLibrary(path)
    {
    }

    inline DynamicLibrary::~DynamicLibrary()
    {
    }

    inline status_t DynamicLibrary::load()
    {
        return os::arch::DynamicLibrary::load();
    }

    inline status_t DynamicLibrary::unload()
    {
        return os::arch::DynamicLibrary::unload();
    }

    inline status_t DynamicLibrary::loadSymbol(const String& name, void*& symbol)
    {
        return os::arch::DynamicLibrary::loadSymbol(name, symbol);
    }

    inline String DynamicLibrary::getPath() const
    {
        return os::arch::DynamicLibrary::getPath();
    }
}

#endif // CAPU_DYNAMICLIBRARY_H
