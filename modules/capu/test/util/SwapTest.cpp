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

#include <gmock/gmock.h>
#include <capu/Config.h>
#include <capu/util/Swap.h>
#include <capu/container/String.h>

TEST(SwapTest, UInt32)
{
    capu::uint32_t first = 5;
    capu::uint32_t second = 50;

    capu::swap(first, second);

    EXPECT_EQ(5u, second);
    EXPECT_EQ(50u, first);
}

TEST(SwapTest, String)
{
    capu::String first = "first string";
    capu::String second = "second string";

    capu::swap(first, second);

    EXPECT_STREQ("first string", second.c_str());
    EXPECT_STREQ("second string", first.c_str());
}


