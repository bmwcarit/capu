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

#include "StaticAllocatorTest.h"

namespace capu
{
    StaticAllocatorTest::StaticAllocatorTest()
    {
    }

    StaticAllocatorTest::~StaticAllocatorTest()
    {
    }

    void StaticAllocatorTest::SetUp()
    {
    }

    void StaticAllocatorTest::TearDown()
    {
    }

    TEST_F(StaticAllocatorTest, AllocatePrimitive)
    {
        StaticAllocator<uint32_t, 2> allocator;

        uint32_t* first  = allocator.allocate();
        uint32_t* second = allocator.allocate();
        uint32_t* third  = allocator.allocate();

        (*first)  = 10u;
        (*second) = 20u;

        EXPECT_EQ(10u, *first);
        EXPECT_EQ(20u, *second);
        EXPECT_EQ(0, third);

        allocator.deallocate(first);
        allocator.deallocate(second);
    }

    struct StaticAllocatorTestClass
    {
        uint32_t value1;
        int16_t  value2;

        static int32_t counter;

        StaticAllocatorTestClass()
        {
            ++counter;
        }

        ~StaticAllocatorTestClass()
        {
            --counter;
        }
    };

    int32_t StaticAllocatorTestClass::counter = 0;

    TEST_F(StaticAllocatorTest, AllocateComplex)
    {
        StaticAllocator<StaticAllocatorTestClass, 2> allocator;

        StaticAllocatorTestClass* first  = allocator.allocate();

        EXPECT_EQ(1, StaticAllocatorTestClass::counter);

        StaticAllocatorTestClass* second = allocator.allocate();

        EXPECT_EQ(2, StaticAllocatorTestClass::counter);

        StaticAllocatorTestClass* third  = allocator.allocate();

        EXPECT_EQ(2, StaticAllocatorTestClass::counter);

        first->value1  = 10u;
        first->value2  = 16;

        second->value1 = 20u;
        second->value2 = 32;

        EXPECT_EQ(10u, first->value1);
        EXPECT_EQ(16, first->value2);

        EXPECT_EQ(20u, second->value1);
        EXPECT_EQ(32, second->value2);

        EXPECT_EQ(0, third);

        allocator.deallocate(first);

        EXPECT_EQ(1, StaticAllocatorTestClass::counter);

        allocator.deallocate(second);

        EXPECT_EQ(0, StaticAllocatorTestClass::counter);

    }

    TEST_F(StaticAllocatorTest, DeallocatePrimitive)
    {
        StaticAllocator<uint32_t, 2> allocator;

        uint32_t* first  = allocator.allocate();
        uint32_t* second = allocator.allocate();
        uint32_t* third  = allocator.allocate();

        (*first)  = 1;
        (*second) = 2;

        EXPECT_EQ(0, third);

        allocator.deallocate(first);

        EXPECT_EQ(0 , first);

        third = allocator.allocate();

        (*third) = 3;

        EXPECT_TRUE(third != 0);
        EXPECT_EQ(0, first);
        EXPECT_EQ(2u, *second);
        EXPECT_EQ(3u, *third);

        uint32_t* forth = allocator.allocate();

        EXPECT_EQ(0, forth);

        allocator.deallocate(second);
        allocator.deallocate(third);

        first  = allocator.allocate();
        second = allocator.allocate();
        third  = allocator.allocate();

        EXPECT_TRUE(0 != first);
        EXPECT_TRUE(0 != second);
        EXPECT_TRUE(0 == third);
    }

    TEST_F(StaticAllocatorTest, DeallocateComplex)
    {
        StaticAllocator<StaticAllocatorTestClass, 2> allocator;

        StaticAllocatorTestClass* first  = allocator.allocate();
        StaticAllocatorTestClass* second = allocator.allocate();

        first->value1  = 10u;
        first->value2  = 16;

        second->value1 = 20u;
        second->value2 = 32;

        allocator.deallocate(second);

        EXPECT_EQ(10u, first->value1);
        EXPECT_EQ(16, first->value2);

        EXPECT_EQ(0, second);

        second = allocator.allocate();

        EXPECT_TRUE(0 != second);
    }
}
