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

#ifndef environ
extern char** environ;
#endif

namespace capu
{
    /**
     * Class for accessing environment variables of the operating system.
     */
    class EnvironmentVariables
    {
    public:
        EnvironmentVariables();
        /**
         * Obtain all environment variables
         * @return A table containing all enviroment variables
         */
        const HashTable<String, String>& getAll() const;

        /**
         * Obtain the value of an environment variable
         * @param key the key of the environment variable to query
         * @param value Reference to string which will contain the requested value
         * @return CAPU_OK if variable was successfully queried
         */
        bool_t get(const String& key, String& value) const;

    private:
        HashTable<String, String> m_env;
    };

    inline EnvironmentVariables::EnvironmentVariables()
    {
        uint_t i = 0;
        char_t* envvar = 0;
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
                    m_env.put(name, value);
                }
                i++;
            }
        }
        while (envvar);
    }

    inline bool_t EnvironmentVariables::get(const String& key, String& value) const
    {
        HashTable<String, String>::Iterator it = m_env.find(key);
        if (it != m_env.end())
        {
            value = (*it).value;
            return true;
        }
        return false;
    }

    inline const HashTable<String, String>& EnvironmentVariables::getAll() const
    {
        return m_env;
    }
}

#endif // CAPU_ENVIRONMENTVARIABLES_H
