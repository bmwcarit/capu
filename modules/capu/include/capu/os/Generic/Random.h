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

#ifndef CAPU_GENERIC_RANDOM_H
#define CAPU_GENERIC_RANDOM_H

#include "capu/os/Time.h"

namespace capu
{
    namespace generic
    {
        class Random
        {
        public:
            Random();
            uint8_t nextUInt8();
            uint16_t nextUInt16();
            uint32_t nextUInt32();
        };

        inline Random::Random()
        {
            srand(static_cast<uint32_t>(Time::GetMilliseconds()));
        }

        inline uint8_t Random::nextUInt8()
        {
            return static_cast<uint8_t>(nextUInt16());
        }

        inline uint16_t Random::nextUInt16()
        {
            // RAND_MAX is at least 32767, which are 15 bit, so we need to shift here already
            return static_cast<uint16_t>((rand() << 8) | rand()); // avoid compiler warnings.
        }

        inline uint32_t Random::nextUInt32()
        {
            // because RAND_MAX is not guarantueed to be bigger than 32767, we do the distribution on ourself.
            return nextUInt16() | (nextUInt16() << 16);
        }
    }
}

#endif // CAPU_GENERIC_RANDOM_H
