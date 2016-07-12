/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#include "StringOutputStreamTest.h"
#include "capu/os/NumericLimits.h"

namespace capu
{
    StringOutputStreamTest::StringOutputStreamTest()
    {
    }

    StringOutputStreamTest::~StringOutputStreamTest()
    {
    }

    void StringOutputStreamTest::SetUp()
    {
    }

    void StringOutputStreamTest::TearDown()
    {
    }

    TEST_F(StringOutputStreamTest, WriteFloatWithDefaultPrecision)
    {
        outputStream << 47.11f;
        outputStream.flush();
        EXPECT_STREQ("47.110001", outputStream.c_str());
        EXPECT_EQ(9U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteFloatMaximumNegativeWithDefaultPrecision)
    {
        outputStream << -capu::generic::NumericLimits::Max<float>();
        outputStream.flush();
        EXPECT_EQ(47U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteFloatZeroWithMaximumPrecision)
    {
        outputStream.setDecimalDigits(45);
        outputStream << 0.f;
        outputStream.flush();
        EXPECT_EQ(2U + 45U, outputStream.length()); // '0.' + precision
    }

    TEST_F(StringOutputStreamTest, WriteFloatZeroWithCappedToMaximumPrecision)
    {
        outputStream.setDecimalDigits(46);
        outputStream << 0.f;
        outputStream.flush();
        EXPECT_EQ(2U + 45U, outputStream.length()); // '0.' + precision
    }

    TEST_F(StringOutputStreamTest, WriteFloatSmallestNegativeWithMaximumPrecision)
    {
        outputStream.setDecimalDigits(45);
        outputStream << (-1.f / (capu::generic::NumericLimits::Max<float>() - 1));
        outputStream.flush();
        EXPECT_EQ(3U + 45U, outputStream.length()); // '-' + '0.' + precision
    }

    TEST_F(StringOutputStreamTest, WriteInt32)
    {
        outputStream << -1059192060;
        outputStream.flush();
        EXPECT_STREQ("-1059192060", outputStream.c_str());
        EXPECT_EQ(11U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt32)
    {
        outputStream << 4711u;
        outputStream.flush();
        EXPECT_STREQ("4711", outputStream.c_str());
        EXPECT_EQ(4U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt64)
    {
        int64_t value = 0x6464646432323232LL;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("7234017282965516850", outputStream.c_str());
        EXPECT_EQ(19U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt64)
    {
        uint64_t value = 0x6464646432323232uLL;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("7234017282965516850", outputStream.c_str());
        EXPECT_EQ(19U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteString)
    {
        outputStream << String("Hello World");
        outputStream.flush();
        EXPECT_STREQ("Hello World", outputStream.c_str());
        EXPECT_EQ(11U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteBool)
    {
        outputStream << true;
        outputStream.flush();
        EXPECT_STREQ("1", outputStream.c_str());
        EXPECT_EQ(1U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteCharArray)
    {
        outputStream << "Hello World";
        outputStream.flush();
        EXPECT_STREQ("Hello World", outputStream.c_str());
        EXPECT_EQ(11U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteChar)
    {
        char c = 'T';
        outputStream << c;
        outputStream.flush();
        EXPECT_STREQ("T", outputStream.c_str());
        EXPECT_EQ(1U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt16)
    {
        outputStream << static_cast<uint16_t>(4711);
        outputStream.flush();
        EXPECT_STREQ("4711", outputStream.c_str());
        EXPECT_EQ(4U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteGuid)
    {
        outputStream << Guid("AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE");
        outputStream.flush();
        EXPECT_STREQ("AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE", outputStream.c_str());
        EXPECT_EQ(36U, outputStream.length());
    }


    TEST_F(StringOutputStreamTest, WriteEndl)
    {
        outputStream << "test" << endl;
        outputStream.flush();
        EXPECT_STREQ("test\n", outputStream.c_str());
        EXPECT_EQ(5u, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteFixed)
    {
        outputStream << 47.11f;
        outputStream.flush();
        EXPECT_STREQ("47.110001", outputStream.c_str());
        EXPECT_EQ(9u, outputStream.length());
        outputStream.clear();
        outputStream << fixed << 47.11f;
        outputStream.flush();
        EXPECT_STREQ("47.1100", outputStream.c_str());
        EXPECT_EQ(7u, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, Clear)
    {
        outputStream << "Some data";
        outputStream.clear();
        EXPECT_STREQ("", outputStream.c_str());
        EXPECT_EQ(0u, outputStream.length());

        outputStream << "Some data";
        outputStream.flush();
        outputStream.clear();
        EXPECT_STREQ("", outputStream.c_str());
        EXPECT_EQ(0u, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, AutoFlush)
    {
        outputStream << "Some data";
        EXPECT_STREQ("Some data", outputStream.c_str());
        EXPECT_EQ(9u, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteData)
    {
        outputStream.write("Some data to write", 18);
        outputStream.flush();
        EXPECT_STREQ("Some data to write", outputStream.c_str());
        EXPECT_EQ(18U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, Flush)
    {
        outputStream << 4711;
        outputStream.flush();
        EXPECT_EQ(CAPU_OK, outputStream.flush());
        EXPECT_EQ(4U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, MultipleData)
    {
        outputStream << 4711 << " " << 47.11f << " " << "Hello World" << " " << true;
        outputStream.flush();
        EXPECT_STREQ("4711 47.110001 Hello World 1", outputStream.c_str());
        EXPECT_EQ(28U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, Resize)
    {
        outputStream << "Exactly 16 chars";
        outputStream.flush();
        EXPECT_STREQ("Exactly 16 chars", outputStream.c_str());
        EXPECT_EQ(16U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, C_StrIsNullTerminatedForConstStreams)
    {
        outputStream << "a";
        const StringOutputStream& constStream = outputStream;
        EXPECT_STREQ("a", constStream.c_str());
    }

    TEST_F(StringOutputStreamTest, WriteUInt32HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        uint32_t value = 4446222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x43D80E", outputStream.c_str());
        EXPECT_EQ(8U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt32HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        uint32_t value = 4446222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0043D80E", outputStream.c_str());
        EXPECT_EQ(10U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt32HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        int32_t value = 4446222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x43D80E", outputStream.c_str());
        EXPECT_EQ(8U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt32HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        int32_t value = 4446222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0043D80E", outputStream.c_str());
        EXPECT_EQ(10U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt32HexNegativeValue)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        int32_t value = -4446222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0xFFBC27F2", outputStream.c_str());//uint32_max - value
        EXPECT_EQ(10U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt64HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        uint64_t value = 353544462511222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x1418BFC19AC76", outputStream.c_str());
        EXPECT_EQ(15U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt64HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        uint64_t value = 353544462511222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0001418BFC19AC76", outputStream.c_str());
        EXPECT_EQ(18U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt64HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        int64_t value = 353544462511222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x1418BFC19AC76", outputStream.c_str());
        EXPECT_EQ(15U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt64HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        int64_t value = 353544462511222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0001418BFC19AC76", outputStream.c_str());
        EXPECT_EQ(18U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt64HexNegativeValue)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        int64_t value = -353544462511222;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0xFFFEBE7403E6538A", outputStream.c_str());//uint64_max - value
        EXPECT_EQ(18U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt16HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        uint16_t value = 1337;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0539", outputStream.c_str());
        EXPECT_EQ(6U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteUInt16HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        uint16_t value = 1337;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x539", outputStream.c_str());
        EXPECT_EQ(5U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt16HexLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        uint16_t value = 1337;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x0539", outputStream.c_str());
        EXPECT_EQ(6U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt16HexNoLeadingZero)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_NO_LEADING_ZEROS);
        uint16_t value = 1337;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0x539", outputStream.c_str());
        EXPECT_EQ(5U, outputStream.length());
    }

    TEST_F(StringOutputStreamTest, WriteInt16HexNegativeValue)
    {
        outputStream.setHexadecimalOutputFormat(StringOutputStream::HEXADECIMAL_LEADING_ZEROS);
        int16_t value = -1337;
        outputStream << value;
        outputStream.flush();
        EXPECT_STREQ("0xFAC7", outputStream.c_str());//uint16_max - value
        EXPECT_EQ(6U, outputStream.length());
    }
}
