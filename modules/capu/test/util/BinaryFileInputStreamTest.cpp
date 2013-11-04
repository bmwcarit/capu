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
        mFile.open(WRITE_EXISTING_BINARY);

        int32_t intVal = 10;
        float_t floatVal = 20.f;
        String  stringVal = "Dies ist ein Text";
        uint32_t strlen = static_cast<uint32_t>(stringVal.getLength());

        mFile.write(reinterpret_cast<char_t*>(&intVal), sizeof(capu::int32_t));
        mFile.write(reinterpret_cast<char_t*>(&floatVal), sizeof(capu::float_t));
        mFile.write(reinterpret_cast<char_t*>(&strlen), sizeof(uint32_t));
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

        capu::int32_t intVal = 0;
        capu::float_t floatVal = 0.f;
        capu::String  stringVal = "";

        inputStream >> intVal >> floatVal >> stringVal;

        EXPECT_EQ(10, intVal);
        EXPECT_EQ(20.0f, floatVal);
        EXPECT_STREQ("Dies ist ein Text", stringVal.c_str());

    }

    TEST_F(BinaryFileInputStreamTest, ReadclosedFile)
    {
        capu::BinaryFileInputStream inputStream(mFile);

        // remove the file to make the after read fails
        mFile.close();
        mFile.remove();



        capu::char_t data[1];
        capu::uint32_t len = 1;
        data[0] = 3;
        inputStream.read(data, len);

        EXPECT_EQ(3, data[0]);
    }

    TEST_F(BinaryFileInputStreamTest, ReadNonexistingFile)
    {
    	capu::File nonexistingFile("./IdontExist");
        capu::BinaryFileInputStream inputStream(nonexistingFile);

        capu::char_t data[1];
        capu::uint32_t len = 1;
        data[0] = 3;
        inputStream.read(data, len);

        EXPECT_EQ(3, data[0]);
    }
}
