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
#include "capu/os/NumericLimits.h"

TEST(NumericLimits, ensureAvailableForAlltypes)
{
    (void)capu::NumericLimits<uint64_t>::Max();
    (void)capu::NumericLimits<int64_t>::Max();

    (void)capu::NumericLimits<uint32_t>::Max();
    (void)capu::NumericLimits<int32_t>::Max();

    (void)capu::NumericLimits<uint16_t>::Max();
    (void)capu::NumericLimits<int16_t>::Max();

    (void)capu::NumericLimits<uint8_t>::Max();
    (void)capu::NumericLimits<int8_t>::Max();

    (void)capu::NumericLimits<float>::Max();
    (void)capu::NumericLimits<double>::Max();

    (void)capu::NumericLimits<capu::uint_t>::Max();
    (void)capu::NumericLimits<capu::int_t>::Max();
}

TEST(NumericLimits, testLimits)
{
    EXPECT_EQ(capu::NumericLimits<int64_t>::Max(), static_cast<int64_t>(0x7fffffffffffffffLL));
    EXPECT_EQ(capu::NumericLimits<int64_t>::Min(), static_cast<int64_t>(0x8000000000000000LL));
    EXPECT_EQ(capu::NumericLimits<uint64_t>::Max(), static_cast<uint64_t>(0xffffffffffffffffULL));
    EXPECT_EQ(capu::NumericLimits<uint64_t>::Min(), static_cast<uint64_t>(0x0));
    EXPECT_EQ(capu::NumericLimits<int32_t>::Max(), static_cast<int32_t>(0x7fffffff));
    EXPECT_EQ(capu::NumericLimits<int32_t>::Min(), static_cast<int32_t>(0x80000000));
    EXPECT_EQ(capu::NumericLimits<uint32_t>::Max(), static_cast<uint32_t>(0xffffffff));
    EXPECT_EQ(capu::NumericLimits<uint32_t>::Min(), static_cast<uint32_t>(0x0));
    EXPECT_EQ(capu::NumericLimits<int16_t>::Max(), static_cast<int32_t>(32767));
    EXPECT_EQ(capu::NumericLimits<int16_t>::Min(), static_cast<int32_t>(-32768));
    EXPECT_EQ(capu::NumericLimits<uint16_t>::Max(), static_cast<int32_t>(65535));
    EXPECT_EQ(capu::NumericLimits<uint16_t>::Min(), static_cast<int32_t>(0));
    EXPECT_EQ(capu::NumericLimits<int8_t>::Max(), static_cast<int32_t>(127));
    EXPECT_EQ(capu::NumericLimits<int8_t>::Min(), static_cast<int32_t>(-128));
    EXPECT_EQ(capu::NumericLimits<uint8_t>::Max(), static_cast<int32_t>(255));
    EXPECT_EQ(capu::NumericLimits<uint8_t>::Min(), static_cast<int32_t>(0));
}
