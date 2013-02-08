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
#include "capu/os/Memory.h"
#include "capu/container/String.h"

TEST(Memory, compare)
{
    capu::char_t string1[30] = "This is a String";
    capu::char_t string2[30] = "This is a String";
    capu::char_t string3[30] = "This is another String";

    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, 30));
    EXPECT_FALSE(0 == capu::Memory::Compare(string2, string3, 30));
}

TEST(Memory, set)
{
    capu::char_t string[] = "Hello World";
    capu::Memory::Set(string, 'm', 5);
    EXPECT_EQ(0, capu::Memory::Compare("mmmmm World", string, strlen(string)));
}

TEST(Memory, copy)
{
    capu::char_t string1[30] = "This is a String";
    capu::char_t string2[30];
    capu::Memory::Copy(string2, string1, strlen(string1) + 1);
    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, strlen(string1)));
}

TEST(Memory, move)
{
    capu::char_t string1[35] = "This is a boring String";
    capu::Memory::Move(string1 + 17, string1 + 10, 13);
    capu::char_t string2[35] = "This is a boring boring String";
    EXPECT_EQ(0, capu::Memory::Compare(string2, string1, 35));
}

TEST(Memory, copyObjectWithPrimitiveType)
{
    capu::uint32_t vals[42];
    vals[5] = 10;
    capu::uint32_t othervals[42];

    capu::Memory::CopyObject(othervals, vals, 42);
    EXPECT_EQ(static_cast<capu::uint32_t>(10), othervals[5]);
}

class SomeTestClass
{
public:
    capu::uint32_t i;
    capu::bool_t assignmentOperatorCalled;

    SomeTestClass()
        : i(0)
        , assignmentOperatorCalled(false)
    {
    }

    SomeTestClass(int val)
        : i(val)
        , assignmentOperatorCalled(false)
    {
    }

    SomeTestClass& operator=(const SomeTestClass& other)
    {
        i = other.i;
        assignmentOperatorCalled = true;
        return *this;
    }
};

TEST(Memory, copyObjectWithClassType)
{
    SomeTestClass vals[100];
    vals[5].i = 10;
    SomeTestClass othervals[100];

    for (int i = 0; i < 100; i++)
    {
        EXPECT_FALSE(othervals[i].assignmentOperatorCalled);
    }

    capu::Memory::CopyObject(othervals, vals, 100);
    EXPECT_EQ(static_cast<capu::uint32_t>(10), othervals[5].i); // test that assignment operator was called
    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(othervals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, copyWithStrings)
{
    std::string* vals = new std::string[42];
    std::string& val = vals[5];
    val = "hello world";
    std::string othervals[42];

    capu::Memory::CopyObject(othervals, vals, 42);
    delete[] vals; // no problem should occur!
    EXPECT_EQ("hello world", othervals[5]);
}

TEST(Memory, moveOjectWithClassTypeNoOverlap)
{
    SomeTestClass vals[10];
    for (capu::uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[5], &vals[0], 3);
    for (capu::uint32_t i = 5; i < 5 + 3; ++i)
    {
        EXPECT_EQ(i - 5, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, moveOjectWithClassTypeOverlapForward)
{
    SomeTestClass vals[10];
    for (capu::uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[3], &vals[0], 6);
    for (capu::uint32_t i = 3; i < 3 + 6; ++i)
    {
        EXPECT_EQ(i - 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, moveOjectWithClassTypeOverlapBackward)
{
    SomeTestClass vals[10];
    for (capu::uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[0], &vals[3], 6);
    for (capu::uint32_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(i + 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, usedMemoryTest)
{
    capu::uint_t memUsage = capu::Memory::CurrentMemoryUsage();
    capu::char_t* someMem = new capu::char_t[1000000];
    capu::uint_t memUsageAfter = capu::Memory::CurrentMemoryUsage();
    EXPECT_GE(memUsageAfter, memUsage);
    delete[] someMem;
}
