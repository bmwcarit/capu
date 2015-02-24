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

#include "BinaryOutputStreamTest.h"
#include "capu/util/Guid.h"
#include "capu/util/BinaryInputStream.h"
#include "capu/os/NumericLimits.h"

namespace capu
{
    BinaryOutputStreamTest::BinaryOutputStreamTest()
    {
    }

    BinaryOutputStreamTest::~BinaryOutputStreamTest()
    {
    }

    void BinaryOutputStreamTest::SetUp()
    {
    }

    void BinaryOutputStreamTest::TearDown()
    {
    }

    TEST_F(BinaryOutputStreamTest, Constructor)
    {
        BinaryOutputStream outStream;

        EXPECT_EQ(0u,  outStream.getSize());
        EXPECT_EQ(16u, outStream.getCapacity());
    }

    TEST_F(BinaryOutputStreamTest, ConstructorWithCapacity)
    {
        BinaryOutputStream outStream(32);

        EXPECT_EQ(0u,  outStream.getSize());
        EXPECT_EQ(32u, outStream.getCapacity());
    }

    TEST_F(BinaryOutputStreamTest, InsertUInt16)
    {
        BinaryOutputStream outStream;

        outStream << static_cast<uint16_t>(5) << static_cast<uint16_t>(6) << static_cast<uint16_t>(7);

        const char_t* data = outStream.getData();
        EXPECT_EQ(5, *reinterpret_cast<const uint16_t*>(data));
        data += sizeof(uint16_t);
        EXPECT_EQ(6, *reinterpret_cast<const uint16_t*>(data));
        data += sizeof(uint16_t);
        EXPECT_EQ(7, *reinterpret_cast<const uint16_t*>(data));
    }


    TEST_F(BinaryOutputStreamTest, InsertInt)
    {
        BinaryOutputStream outStream;

        outStream << 5 << 6 << 7;

        const char_t* data = outStream.getData();
        EXPECT_EQ(5, *reinterpret_cast<const int32_t*>(data));
        data += sizeof(int32_t);
        EXPECT_EQ(6, *reinterpret_cast<const int32_t*>(data));
        data += sizeof(int32_t);
        EXPECT_EQ(7, *reinterpret_cast<const int32_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, InsertUInt)
    {
        BinaryOutputStream outStream;
        outStream << 0u << 5u << 6u << 7u << NumericLimits::Max<uint32_t>();

        const char_t* data = outStream.getData();
        EXPECT_EQ(0u, *reinterpret_cast<const uint32_t*>(data));
        data += sizeof(uint32_t);
        EXPECT_EQ(5u, *reinterpret_cast<const uint32_t*>(data));
        data += sizeof(uint32_t);
        EXPECT_EQ(6u, *reinterpret_cast<const uint32_t*>(data));
        data += sizeof(uint32_t);
        EXPECT_EQ(7u, *reinterpret_cast<const uint32_t*>(data));
        data += sizeof(uint32_t);
        EXPECT_EQ(NumericLimits::Max<uint32_t>(), *reinterpret_cast<const uint32_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, InsertInt64)
    {
        BinaryOutputStream outStream;

        outStream << (int64_t)5 << (int64_t)6 << (int64_t)7;

        const char_t* data = outStream.getData();
        EXPECT_EQ((int64_t)5, *reinterpret_cast<const int64_t*>(data));
        data += sizeof(int64_t);
        EXPECT_EQ((int64_t)6, *reinterpret_cast<const int64_t*>(data));
        data += sizeof(int64_t);
        EXPECT_EQ((int64_t)7, *reinterpret_cast<const int64_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, InsertUInt64)
    {
        BinaryOutputStream outStream;
        outStream << (uint64_t)0u << (uint64_t)5u << (uint64_t)6u << (uint64_t)7u << NumericLimits::Max<uint64_t>();

        const char_t* data = outStream.getData();
        EXPECT_EQ((uint64_t)0u, *reinterpret_cast<const uint64_t*>(data));
        data += sizeof(uint64_t);
        EXPECT_EQ((uint64_t)5u, *reinterpret_cast<const uint64_t*>(data));
        data += sizeof(uint64_t);
        EXPECT_EQ((uint64_t)6u, *reinterpret_cast<const uint64_t*>(data));
        data += sizeof(uint64_t);
        EXPECT_EQ((uint64_t)7u, *reinterpret_cast<const uint64_t*>(data));
        data += sizeof(uint64_t);
        EXPECT_EQ(NumericLimits::Max<uint64_t>(), *reinterpret_cast<const uint64_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, InsertFloat)
    {
        BinaryOutputStream outStream;
        outStream << 5.0f << 6.0f << 7.0f;

        const char_t* data = outStream.getData();
        EXPECT_EQ(5.0f, *reinterpret_cast<const float_t*>(data));
        data += sizeof(int32_t);
        EXPECT_EQ(6.0f, *reinterpret_cast<const float_t*>(data));
        data += sizeof(int32_t);
        EXPECT_EQ(7.0f, *reinterpret_cast<const float_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, InsertGuid)
    {
        BinaryOutputStream outStream;
        Guid guid1;
        Guid guid2;
        outStream << guid1 << guid2;

        const char_t* data = outStream.getData();

        BinaryInputStream in(data);
        Guid fromStreamGuid1;
        Guid fromStreamGuid2;
        in >> fromStreamGuid1 >> fromStreamGuid2;

        EXPECT_EQ(guid1, fromStreamGuid1);
        EXPECT_EQ(guid2, fromStreamGuid2);
    }

    TEST_F(BinaryOutputStreamTest, InsertMultipleData)
    {
        BinaryOutputStream outStream;

        outStream << 5 << true << 7.0f;

        const char_t* data = outStream.getData();
        EXPECT_EQ(5, *reinterpret_cast<const int32_t*>(data));
        data += sizeof(int32_t);
        EXPECT_EQ(true, *reinterpret_cast<const bool_t*>(data));
        data += sizeof(bool_t);
        EXPECT_EQ(7.0f, *reinterpret_cast<const float_t*>(data));
    }

    TEST_F(BinaryOutputStreamTest, ClearStream)
    {
        BinaryOutputStream outStream;

        outStream << 5 << "A test string";

        outStream.clear();

        EXPECT_EQ(0u , outStream.getSize());

        outStream << "Another test string";

        const uint32_t strlen = *reinterpret_cast<const uint32_t*>(outStream.getData());

        char_t* buffer = new char_t[strlen + 1];

        capu::Memory::Copy(buffer, outStream.getData() + sizeof(uint32_t), strlen);
        buffer[strlen] = 0;

        EXPECT_STREQ("Another test string", buffer);

        delete[] buffer;
    }

    TEST_F(BinaryOutputStreamTest, InsertString)
    {
        BinaryOutputStream outStream;

        outStream << "Hello World with a lot of characters";

        const uint32_t strlen = *reinterpret_cast<const uint32_t*>(outStream.getData());

        char_t* buffer = new char_t[strlen + 1];

        capu::Memory::Copy(buffer, outStream.getData() + sizeof(uint32_t), strlen);
        buffer[strlen] = 0;

        EXPECT_STREQ("Hello World with a lot of characters", buffer);

        delete[] buffer;
    }
}

