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

#include <gtest/gtest.h>
#include "capu/Config.h"
#include "capu/os/Random.h"
#include "capu/os/Memory.h"
#include "capu/os/NumericLimits.h"

TEST(Random, TestUInt8)
{
    capu::Random rand;

    uint32_t max = capu::NumericLimits::Max<uint8_t>();

    uint32_t* counts = new uint32_t[max + 1];
    capu::Memory::Set(counts, 0, (max + 1) * sizeof(uint32_t));

    for (uint32_t i = 0; i < 1000000; ++i)
    {
        counts[rand.nextUInt8()]++;
    }

    // expect each number at least once
    for (uint32_t i = 0; i < max + 1; ++i)
    {
        EXPECT_LT(0u, counts[i]);
    }
    delete[] counts;
}

TEST(Random, TestUInt16)
{
    capu::Random rand;
    uint16_t i1 = rand.nextUInt16();
    uint16_t i2 = rand.nextUInt16();
    EXPECT_TRUE(i1 != i2);
}

TEST(Random, TestUInt32)
{
    capu::Random rand;
    uint32_t i1 = rand.nextUInt32();
    uint32_t i2 = rand.nextUInt32();
    EXPECT_TRUE(i1 != i2);
}

