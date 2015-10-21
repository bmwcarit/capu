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

#ifndef CAPU_ENVIRONMENTVARIABLES_H
#define CAPU_ENVIRONMENTVARIABLES_H

#include "capu/os/StringUtils.h"
#include "capu/container/String.h"
#include "capu/container/Pair.h"
#include "capu/container/HashTable.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(EnvironmentVariables)

namespace capu
{
    /**
     * Class for accessing environment variables of the operating system.
     */
    class EnvironmentVariables : private capu::os::arch::EnvironmentVariables
    {
    public:
        /**
         * Obtain all environment variables
         * @return A table containing all environment variables
         */
        static HashTable<String, String> getAll();

        /**
         * Obtain the value of an environment variable
         * @param key the key of the environment variable to query
         * @param value Reference to string which will contain the requested value
         * @return CAPU_OK if variable was successfully queried
         */
        static bool get(const String& key, String& value);
    };

    inline bool EnvironmentVariables::get(const String& key, String& value)
    {
        return capu::os::arch::EnvironmentVariables::get(key, value);
    }

    inline HashTable<String, String> EnvironmentVariables::getAll()
    {
        return capu::os::arch::EnvironmentVariables::getAll();
    }
}

#endif // CAPU_ENVIRONMENTVARIABLES_H
