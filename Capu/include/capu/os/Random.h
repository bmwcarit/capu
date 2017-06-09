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

#include "capu/Config.h"
#include <random>

namespace capu
{
    /**
     * Represents a random number generator.
     */
    class Random
    {
    public:
        Random();

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

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<uint32_t> dist;
    };

    inline Random::Random()
        : gen(rd())
    {
    }

    inline uint8_t Random::nextUInt8()
    {
        return dist(gen) & 0xFF;
    }

    inline uint16_t Random::nextUInt16()
    {
        return dist(gen) & 0xFFFF;
    }

    inline uint32_t Random::nextUInt32()
    {
        return dist(gen);
    }
}

#endif // CAPU_RANDOM_H
