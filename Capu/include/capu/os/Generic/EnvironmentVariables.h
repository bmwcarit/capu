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

#ifndef CAPU_GENERIC_ENVIRONMENTVARIABLES_H
#define CAPU_GENERIC_ENVIRONMENTVARIABLES_H

#include <assert.h>
#include "capu/Config.h"
#include "capu/container/String.h"
#include "capu/container/HashTable.h"

#ifndef environ
extern "C" char** environ;
#endif

namespace capu
{
    namespace generic
    {
        class EnvironmentVariables
        {
        public:
            static HashTable<String, String> getAll();
        };

        inline
        HashTable<String, String>
        EnvironmentVariables::getAll()
        {
            HashTable<String, String> env;
            uint_t i = 0;
            char* envvar = 0;
            do
            {
                // "environ" is a globally defined symbol which holds the current environment variables
                envvar = *(environ + i);
                if (envvar)
                {
                    const uint32_t index = static_cast<uint32_t>(StringUtils::IndexOf(envvar, '='));
                    if (index > 0)
                    {
                        const String name(envvar, 0, index - 1);
                        const String value(envvar, index + 1);
                        env.put(name, value);
                    }
                    i++;
                }
            } while (envvar);
            return env;
        }
    }
}


#endif //CAPU_GENERIC_ENVIRONMENTVARIABLES_H
