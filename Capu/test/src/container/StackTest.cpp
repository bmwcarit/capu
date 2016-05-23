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
#include "capu/container/Stack.h"
#include "capu/Error.h"
#include "capu/Config.h"

TEST(Stack, PushPop)
{
    capu::Stack<uint32_t> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    uint32_t val;
    stack.pop(&val);
    EXPECT_EQ(4u, val);
    stack.pop(&val);
    EXPECT_EQ(3u, val);
    stack.pop(&val);
    EXPECT_EQ(2u, val);
    stack.pop(&val);
    EXPECT_EQ(1u, val);
}

TEST(Stack, PopOnEmpty)
{
    capu::Stack<uint32_t> stack;
    EXPECT_EQ(capu::CAPU_EINVAL, stack.pop());
}

TEST(Stack, Clear)
{
    capu::Stack<uint32_t> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.clear();
    EXPECT_EQ(capu::CAPU_EINVAL, stack.pop());
}

TEST(Stack, IsEmpty)
{
    capu::Stack<uint32_t> stack;
    EXPECT_TRUE(stack.isEmpty());
    stack.push(1);
    EXPECT_FALSE(stack.isEmpty());
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(Stack, Size)
{
    capu::Stack<uint32_t> stack;
    EXPECT_EQ(0, stack.size());
    stack.push(1);
    EXPECT_EQ(1, stack.size());
    stack.push(2);
    EXPECT_EQ(2, stack.size());
    stack.push(3);
    EXPECT_EQ(3, stack.size());
    stack.push(4);
    EXPECT_EQ(4, stack.size());
    stack.clear();
    EXPECT_EQ(0, stack.size());
}
