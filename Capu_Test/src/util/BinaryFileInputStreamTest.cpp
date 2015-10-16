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

#include "BinaryFileInputStreamTest.h"
#include <capu/util/BinaryFileInputStream.h>
namespace capu
{
    BinaryFileInputStreamTest::BinaryFileInputStreamTest()
        : mFile("TestInputFile.bin")
    {


    }

    BinaryFileInputStreamTest::~BinaryFileInputStreamTest()
    {

    }

    void BinaryFileInputStreamTest::SetUp()
    {
        mFile.open(WRITE_NEW_BINARY);

        int32_t intVal = 10;
        float floatVal = 20.f;
        String  stringVal = "Dies ist ein Text";
        uint32_t strlen = static_cast<uint32_t>(stringVal.getLength());

        mFile.write(reinterpret_cast<char*>(&intVal), sizeof(int32_t));
        mFile.write(reinterpret_cast<char*>(&floatVal), sizeof(float));
        mFile.write(reinterpret_cast<char*>(&strlen), sizeof(uint32_t));
        mFile.write(stringVal.c_str(), stringVal.getLength());

        mFile.close();
    }

    void BinaryFileInputStreamTest::TearDown()
    {
        mFile.close();
        mFile.remove();
    }


    TEST_F(BinaryFileInputStreamTest, ReadSomeData)
    {

        capu::BinaryFileInputStream inputStream(mFile);

        int32_t intVal = 0;
        float floatVal = 0.f;
        capu::String  stringVal = "";

        inputStream >> intVal;

        EXPECT_EQ(CAPU_OK, inputStream.getState());

        inputStream >> floatVal;

        EXPECT_EQ(CAPU_OK, inputStream.getState());

        inputStream >> stringVal;

        EXPECT_EQ(CAPU_OK, inputStream.getState());

        int32_t errorIntVal = 0;
        inputStream >> errorIntVal;

        EXPECT_EQ(CAPU_EOF, inputStream.getState());

        EXPECT_EQ(10, intVal);
        EXPECT_EQ(20.0f, floatVal);
        EXPECT_STREQ("Dies ist ein Text", stringVal.c_str());
    }

    TEST_F(BinaryFileInputStreamTest, BadFileObj)
    {
        capu::File file("some/non/existing/path");
        capu::BinaryFileInputStream inputStream(file);

        char data;
        int size = 1;

        inputStream.read(&data, size);

        EXPECT_NE(CAPU_OK, inputStream.getState());
    }
}
