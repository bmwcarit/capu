/*
 * Copyright 2013 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "BinaryFileOutputStreamTest.h"
#include <capu/util/BinaryFileOutputStream.h>

namespace capu
{
    BinaryFileOutputStreamTest::BinaryFileOutputStreamTest()
        : mFile("TestFile.bin")
    {
    }

    BinaryFileOutputStreamTest::~BinaryFileOutputStreamTest()
    {
        mFile.close();
        mFile.remove();
    }

    void BinaryFileOutputStreamTest::SetUp()
    {
        capu::BinaryFileOutputStream outputStream(mFile);

        mTestString = "Dies ist ein Text";

        outputStream << 10 << 20.0f << mTestString;
        outputStream.flush();
        mFile.close();

    }

    void BinaryFileOutputStreamTest::TearDown()
    {
    }

    TEST_F(BinaryFileOutputStreamTest, WriteSomeData)
    {
        EXPECT_EQ(capu::CAPU_OK, mFile.open(READ_EXISTING_BINARY));

        capu::char_t buffer[64];
        capu::uint_t numBytes = 0;

        EXPECT_EQ(capu::CAPU_OK, mFile.read(buffer, sizeof(capu::int32_t), numBytes));
        EXPECT_EQ(sizeof(capu::int32_t), numBytes);
        EXPECT_EQ(10, *reinterpret_cast<capu::int32_t*>(buffer));

        EXPECT_EQ(capu::CAPU_OK, mFile.read(buffer, sizeof(capu::float_t), numBytes));
        EXPECT_EQ(sizeof(capu::float_t), numBytes);
        EXPECT_EQ(20.0f, *reinterpret_cast<capu::float_t*>(buffer));

        EXPECT_EQ(CAPU_OK, mFile.read(buffer, sizeof(uint32_t), numBytes));
        EXPECT_EQ(CAPU_OK, mFile.read(buffer, sizeof(capu::char_t) * mTestString.getLength(), numBytes));
        buffer[mTestString.getLength()] = 0;
        EXPECT_EQ(sizeof(capu::char_t) * mTestString.getLength(), numBytes);
        EXPECT_STREQ("Dies ist ein Text", buffer);
    }
}
