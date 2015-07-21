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

#ifndef CAPU_GENERIC_NUMERIC_LIMITS_H
#define CAPU_GENERIC_NUMERIC_LIMITS_H

#include "capu/Config.h"

namespace capu
{
    namespace generic
    {
        class NumericLimits
        {
        public:
            template<typename T> static T Max();
            template<typename T> static T Min();
        };

        template<>
        inline
        int64_t
        NumericLimits::Max<int64_t>()
        {
            return 0x7FFFFFFFFFFFFFFFULL;
        }

        template<>
        inline
        int64_t
        NumericLimits::Min<int64_t>()
        {
            return 0x8000000000000000LL;
        }

        template<>
        inline
        uint64_t
        NumericLimits::Max<uint64_t>()
        {
            return 0xFFFFFFFFFFFFFFFFULL;
        }

        template<>
        inline
        uint64_t
        NumericLimits::Min<uint64_t>()
        {
            return 0x0;
        }

        template<>
        inline
        int32_t
        NumericLimits::Max<int32_t>()
        {
            return 0x7fffffff;
        }

        template<>
        inline
        int32_t
        NumericLimits::Min<int32_t>()
        {
            return 0x80000000;
        }

        template<>
        inline
        uint32_t
        NumericLimits::Max<uint32_t>()
        {
            return 0xFFFFFFFF;
        }

        template<>
        inline
        uint32_t
        NumericLimits::Min<uint32_t>()
        {
            return 0x0;
        }

        template<>
        inline
        int16_t
        NumericLimits::Min<int16_t>()
        {
            return -32768;
        }

        template<>
        inline
        int16_t
        NumericLimits::Max<int16_t>()
        {
            return 32767;
        }

        template<>
        inline
        uint16_t
        NumericLimits::Min<uint16_t>()
        {
            return 0;
        }

        template<>
        inline
        uint16_t
        NumericLimits::Max<uint16_t>()
        {
            return 0xFFFF;
        }

        template<>
        inline
        int8_t
        NumericLimits::Min<int8_t>()
        {
            return -128;
        }

        template<>
        inline
        int8_t
        NumericLimits::Max<int8_t>()
        {
            return 127;
        }

        template<>
        inline
        uint8_t
        NumericLimits::Min<uint8_t>()
        {
            return 0;
        }

        template<>
        inline
        uint8_t
        NumericLimits::Max<uint8_t>()
        {
            return 0xFF;
        }

        template<>
        inline
        float
        NumericLimits::Min<float>()
        {
            return FLT_MIN;
        }

        template<>
        inline
        float
        NumericLimits::Max<float>()
        {
            return FLT_MAX;
        }
    }
}

#endif //CAPU_GENERIC_NUMERIC_LIMITS_H

