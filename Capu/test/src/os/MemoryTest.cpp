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
    char string1[30] = "This is a String";
    char string2[30] = "This is a String";
    char string3[30] = "This is another String";

    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, 30));
    EXPECT_FALSE(0 == capu::Memory::Compare(string2, string3, 30));
    EXPECT_EQ(0, capu::Memory::Compare("", "", 0));
}

TEST(Memory, set)
{
    char string[] = "Hello World";
    capu::Memory::Set(string, 'm', 5);
    EXPECT_EQ(0, capu::Memory::Compare("mmmmm World", string, strlen(string)));
    
    capu::Memory::Set(string, 'a', 0);
    EXPECT_EQ(0, capu::Memory::Compare("mmmmm World", string, strlen(string)));
}

TEST(Memory, copy)
{
    char string1[30] = "This is a String";
    char string2[30];
    capu::Memory::Copy(string2, string1, strlen(string1) + 1);
    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, strlen(string1)));

    capu::Memory::Copy(string2, string1, 0);
    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, strlen(string1)));

    capu::Memory::Copy(string2, 0, 0);
    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, strlen(string1)));

    capu::Memory::Copy(0, string1, 0);
    EXPECT_EQ(0, capu::Memory::Compare(string1, string2, strlen(string1)));
}

TEST(Memory, move)
{
    char string1[35] = "This is a boring String";
    capu::Memory::Move(string1 + 17, string1 + 10, 13);
    char string2[35] = "This is a boring boring String";
    EXPECT_EQ(0, capu::Memory::Compare(string2, string1, 35));

    capu::Memory::Move(string1 + 17, string1 + 10, 0);
    EXPECT_EQ(0, capu::Memory::Compare(string2, string1, 35));
}

TEST(Memory, copyObjectWithPrimitiveType)
{
    uint32_t vals[42];
    vals[5] = 10;
    uint32_t othervals[42];

    capu::Memory::CopyObject(othervals, vals, 42);
    EXPECT_EQ(static_cast<uint32_t>(10), othervals[5]);

    capu::Memory::CopyObject(othervals, vals, 0);
    EXPECT_EQ(static_cast<uint32_t>(10), othervals[5]);
}

class SomeTestClass
{
public:
    uint32_t i;
    bool assignmentOperatorCalled;

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
    EXPECT_EQ(static_cast<uint32_t>(10), othervals[5].i); // test that assignment operator was called
    for (int i = 0; i < 100; i++)
    {
        EXPECT_TRUE(othervals[i].assignmentOperatorCalled);
    }

    capu::Memory::CopyObject(othervals, vals, 0);
    EXPECT_EQ(static_cast<uint32_t>(10), othervals[5].i); // test that assignment operator was called
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
    EXPECT_EQ("hello world", othervals[5]);

    capu::Memory::CopyObject(othervals, vals, 0);
    EXPECT_EQ("hello world", othervals[5]);

    delete[] vals; // no problem should occur!
}

TEST(Memory, moveOjectWithClassTypeNoOverlap)
{
    SomeTestClass vals[10];
    for (uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[5], &vals[0], 3);
    for (uint32_t i = 5; i < 5 + 3; ++i)
    {
        EXPECT_EQ(i - 5, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }

    capu::Memory::MoveObject(&vals[5], &vals[0], 0);
    for (uint32_t i = 5; i < 5 + 3; ++i)
    {
        EXPECT_EQ(i - 5, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, moveOjectWithClassTypeOverlapForward)
{
    SomeTestClass vals[10];
    for (uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[3], &vals[0], 6);
    for (uint32_t i = 3; i < 3 + 6; ++i)
    {
        EXPECT_EQ(i - 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }

    capu::Memory::MoveObject(&vals[3], &vals[0], 0);
    for (uint32_t i = 3; i < 3 + 6; ++i)
    {
        EXPECT_EQ(i - 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}

TEST(Memory, moveOjectWithClassTypeOverlapBackward)
{
    SomeTestClass vals[10];
    for (uint32_t i = 0; i < 10; ++i)
    {
        vals[i].i = i;
    }

    capu::Memory::MoveObject(&vals[0], &vals[3], 6);
    for (uint32_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(i + 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }

    capu::Memory::MoveObject(&vals[0], &vals[3], 0);
    for (uint32_t i = 0; i < 6; ++i)
    {
        EXPECT_EQ(i + 3, vals[i].i);
        EXPECT_TRUE(vals[i].assignmentOperatorCalled);
    }
}
