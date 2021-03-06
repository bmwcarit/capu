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

#ifndef CAPU_NUMERIC_LIMITS_H
#define CAPU_NUMERIC_LIMITS_H

// remove MSVC macros
#undef min
#undef max

#include <capu/Config.h>
#include <limits>

namespace capu
{
    /**
     * Numerical system limits.
     */
    template <typename T>
    class NumericLimits // no inheritance is needed because all methods are static
    {
    public:
        /**
         * Return the maximum value for the given type
         */
        static T Max()
        {
            return std::numeric_limits<T>::max();
        }

        /**
         * Return the minimum value for the given type
         */
        static T Min()
        {
            return std::numeric_limits<T>::min();
        }

        /**
         * Return the epsilon value for the given type
         */
        static T Epsilon()
        {
            return std::numeric_limits<T>::epsilon();
        }
    };
}

#endif //CAPU_NUMERIC_LIMITS_H
