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

#include "BinaryInputStreamTest.h"
#include <capu/util/BinaryOutputStream.h>
#include <capu/os/Memory.h>

namespace capu
{
    BinaryInputStreamTest::BinaryInputStreamTest()
    {
    }

    BinaryInputStreamTest::~BinaryInputStreamTest()
    {
    }

    void BinaryInputStreamTest::SetUp()
    {
    }

    void BinaryInputStreamTest::TearDown()
    {
    }

    TEST_F(BinaryInputStreamTest, ReadInt32Value)
    {
        char buffer[4];
        int32_t value = 5;
        Memory::Copy(buffer, &value, sizeof(int32_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(5, value);
    }

    TEST_F(BinaryInputStreamTest, ReadUInt32Value)
    {
        char buffer[4];
        uint32_t value = 5;
        Memory::Copy(buffer, &value, sizeof(uint32_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(5u, value);
    }

    TEST_F(BinaryInputStreamTest, ReadInt64Value)
    {
        char buffer[8];
        int64_t value = 5;
        Memory::Copy(buffer, &value, sizeof(int64_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(5, value);
    }

    TEST_F(BinaryInputStreamTest, ReadUInt64Value)
    {
        char buffer[8];

        uint64_t value = 5;
        Memory::Copy(buffer, &value, sizeof(uint64_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(5u, value);
    }

    TEST_F(BinaryInputStreamTest, ReadUInt16Value)
    {
        char buffer[2];
        uint16_t value = 5;
        Memory::Copy(buffer, &value, sizeof(uint16_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(5, value);
    }

    TEST_F(BinaryInputStreamTest, ReadStringValue)
    {
        char buffer[16];
        String value = "Hello World";
        uint32_t strlen = static_cast<uint32_t>(value.getLength());

        Memory::Copy(buffer, reinterpret_cast<char_t*>(&strlen), sizeof(uint32_t));
        Memory::Copy(buffer + sizeof(uint32_t), value.c_str(), value.getLength() + 1);

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_STREQ("Hello World", value.c_str());
    }

    TEST_F(BinaryInputStreamTest, ReadBoolValue)
    {
        char buffer[4];
        bool value = true;
        Memory::Copy(buffer, &value, sizeof(bool));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(true, value);
    }

    TEST_F(BinaryInputStreamTest, ReadFloatValue)
    {
        char buffer[4];
        float_t value = 0.002f;
        Memory::Copy(buffer, &value, sizeof(float_t));

        BinaryInputStream inStream(buffer);

        inStream >> value;

        EXPECT_EQ(0.002f, value);
    }

    TEST_F(BinaryInputStreamTest, ReadMultipleData)
    {
        char buffer[1024];
        int32_t intVal = 5;
        float_t floatVal = 4.3f;
        String stringVal = "Hello World";
        uint32_t strlen = static_cast<uint32_t>(stringVal.getLength());
        bool boolVal = true;

        uint_t offset = 0;

        Memory::Copy(buffer, &intVal, sizeof(int32_t));
        offset += sizeof(int32_t);
        Memory::Copy(buffer + offset, &floatVal, sizeof(float_t));
        offset += sizeof(float_t);
        Memory::Copy(buffer + offset, &strlen, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        Memory::Copy(buffer + offset , stringVal.c_str(), stringVal.getLength() + 1);
        offset += stringVal.getLength();
        Memory::Copy(buffer + offset, &boolVal, sizeof(bool));

        BinaryInputStream inStream(buffer);

        inStream >> intVal >> floatVal >> stringVal >> boolVal;

        EXPECT_EQ(5, intVal);
        EXPECT_EQ(4.3f, floatVal);
        EXPECT_STREQ("Hello World", stringVal.c_str());
        EXPECT_EQ(true, boolVal);
    }

    TEST_F(BinaryInputStreamTest, Reset)
    {
        char buffer[8];
        int32_t value1 = 5;
        int32_t value2 = 10;
        Memory::Copy(buffer, &value1, sizeof(int32_t));
        Memory::Copy(buffer + sizeof(int32_t), &value2, sizeof(int32_t));

        BinaryInputStream inStream(buffer);

        int32_t result1;
        inStream >> result1;
        EXPECT_EQ(value1, result1);

        int32_t result2;
        inStream >> result2;
        EXPECT_EQ(value2, result2);

        int32_t result3;
        inStream.reset();
        inStream >> result3;
        EXPECT_EQ(value1, result3);
    }

}
