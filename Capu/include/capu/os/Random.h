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

#ifndef CAPU_RANDOM_H
#define CAPU_RANDOM_H

#include "capu/os/PlatformInclude.h"
#include CAPU_PLATFORM_INCLUDE(Random)

namespace capu
{
    /**
     * Represents a random number generator.
     */
    class Random: private capu::os::arch::Random
    {
    public:
        /**
         * Creates a new random number.
         * @return A new random number.
         */
        uint8_t nextUInt8();

        /**
         * Creates a new random number.
         * @return A new random number.
         */
        uint16_t nextUInt16();

        /**
         * Creates a new random number.
         * @return A new random number.
         */
        uint32_t nextUInt32();
    };

    inline uint8_t Random::nextUInt8()
    {
        return capu::os::arch::Random::nextUInt8();
    }

    inline uint16_t Random::nextUInt16()
    {
        return capu::os::arch::Random::nextUInt16();
    }

    inline uint32_t Random::nextUInt32()
    {
        return capu::os::arch::Random::nextUInt32();
    }
}

#endif // CAPU_RANDOM_H
