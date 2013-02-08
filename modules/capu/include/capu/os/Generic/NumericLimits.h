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
        capu::int64_t
        NumericLimits::Max<capu::int64_t>()
        {
            return 0x7FFFFFFFFFFFFFFFULL;
        }

        template<>
        inline
        capu::int64_t
        NumericLimits::Min<capu::int64_t>()
        {
            return 0x8000000000000000LL;
        }

        template<>
        inline
        capu::uint64_t
        NumericLimits::Max<capu::uint64_t>()
        {
            return 0xFFFFFFFFFFFFFFFFULL;
        }

        template<>
        inline
        capu::uint64_t
        NumericLimits::Min<capu::uint64_t>()
        {
            return 0x0;
        }

        template<>
        inline
        capu::int32_t
        NumericLimits::Max<capu::int32_t>()
        {
            return 0x7fffffff;
        }

        template<>
        inline
        capu::int32_t
        NumericLimits::Min<capu::int32_t>()
        {
            return 0x80000000;
        }

        template<>
        inline
        capu::uint32_t
        NumericLimits::Max<capu::uint32_t>()
        {
            return 0xFFFFFFFF;
        }

        template<>
        inline
        capu::uint32_t
        NumericLimits::Min<capu::uint32_t>()
        {
            return 0x0;
        }

        template<>
        inline
        capu::int16_t
        NumericLimits::Min<capu::int16_t>()
        {
            return -32768;
        }

        template<>
        inline
        capu::int16_t
        NumericLimits::Max<capu::int16_t>()
        {
            return 32767;
        }

        template<>
        inline
        capu::uint16_t
        NumericLimits::Min<capu::uint16_t>()
        {
            return 0;
        }

        template<>
        inline
        capu::uint16_t
        NumericLimits::Max<capu::uint16_t>()
        {
            return 0xFFFF;
        }

        template<>
        inline
        capu::int8_t
        NumericLimits::Min<capu::int8_t>()
        {
            return -128;
        }

        template<>
        inline
        capu::int8_t
        NumericLimits::Max<capu::int8_t>()
        {
            return 127;
        }

        template<>
        inline
        capu::uint8_t
        NumericLimits::Min<capu::uint8_t>()
        {
            return 0;
        }

        template<>
        inline
        capu::uint8_t
        NumericLimits::Max<capu::uint8_t>()
        {
            return 0xFF;
        }

        template<>
        inline
        capu::float_t
        NumericLimits::Min<capu::float_t>()
        {
            return FLT_MIN;
        }

        template<>
        inline
        capu::float_t
        NumericLimits::Max<capu::float_t>()
        {
            return FLT_MAX;
        }
    }
}

#endif //CAPU_GENERIC_NUMERIC_LIMITS_H

