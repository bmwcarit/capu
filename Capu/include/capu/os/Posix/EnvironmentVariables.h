/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_UNIXBASED_ENVIRONMENTVARIABLES_H
#define CAPU_UNIXBASED_ENVIRONMENTVARIABLES_H

#include <stdlib.h>
#include <capu/os/Generic/EnvironmentVariables.h>

namespace capu
{
    namespace posix
    {
        class EnvironmentVariables: private capu::generic::EnvironmentVariables
        {
        public:
            using capu::generic::EnvironmentVariables::getAll;
            static bool get(const String& key, String& value);
        };

        inline
        bool EnvironmentVariables::get(const String& key, String& value)
        {
            char * env = getenv(key.c_str());
            value = env;
            return (0 != env);
        }
    }
}


#endif // CAPU_UNIXBASED_ENVIRONMENTVARIABLES_H
