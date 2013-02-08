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

#ifndef CAPU_COMPARATOR_H
#define CAPU_COMPARATOR_H

#include "capu/Config.h"
#include "capu/os/StringUtils.h"

namespace capu
{
    /**
     * Comparison class used in container classes.
     */
    class Comparator
    {

    public:
        /**
         * Return result of comparison.
         */
        template <class T>
        bool_t operator()(const T& x, const T& y) const
        {
            return x == y;
        }

        /**
         * Return result of string comparison.
         */
        bool_t operator()(const char_t* x, const char_t* y) const
        {
            return (StringUtils::Strcmp(x, y) == 0);
        }

        /**
         * Return result of string comparison.
         */
        bool_t operator()(char_t* x, char_t* y) const
        {
            return (StringUtils::Strcmp(x, y) == 0);
        }
    };
}

#endif /* CAPU_COMPARATOR_H */

