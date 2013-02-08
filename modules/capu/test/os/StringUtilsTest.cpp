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
#include "capu/os/StringUtils.h"
#include "capu/os/Memory.h"

TEST(StringUtils, Strcmp)
{
    capu::char_t string1[] = "My String";
    capu::char_t string2[] = "My String";
    capu::char_t string3[] = "Another String";
    EXPECT_EQ(0, capu::StringUtils::Strcmp(string1, string2));
    EXPECT_TRUE(0 <= capu::StringUtils::Strcmp(string1, string3));
}

TEST(StringUtils, Strncpy)
{
    capu::char_t string1[20] = "My String";
    capu::char_t string2[30];
    capu::StringUtils::Strncpy(string2, sizeof(string2), string1);
    EXPECT_STREQ(string1, string2);
}

TEST(StringUtils, Strncpy2)
{
    capu::char_t string1[20] = "My String";
    capu::char_t string2[20];
    capu::Memory::Set(string2, 42, sizeof(string2));
    capu::StringUtils::Strncpy(string2, 4, string1);
    EXPECT_STREQ("My ", string2);

    // check that only 4 bytes are written, an no more
    for (capu::uint32_t i = 4; i < sizeof(string2); ++i)
    {
        EXPECT_EQ(42, string2[i]);
    }
}

TEST(StringUtils, Strlen1)
{
    capu::char_t string1[20] = "My String";
    EXPECT_EQ(9u, capu::StringUtils::Strlen(string1));
}

TEST(StringUtils, Strlen2)
{
    EXPECT_EQ(static_cast<capu::uint_t>(0),  capu::StringUtils::Strlen(0));
}

TEST(StringUtils, Sprintf)
{
    capu::char_t string1[20];
    capu::StringUtils::Sprintf(string1, 20, "%d", 12345);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("12345", string1));
    capu::StringUtils::Sprintf(string1, 20, "%d %f", 12345, 12.45);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("12345 12.450000", string1));
}

class VscprintfTest
{
public:
    static capu::int32_t Vscprintf(const capu::char_t* format, ...)
    {
        capu::int32_t length = 0;
        va_list args;
        va_start(args, format);
        length = capu::StringUtils::Vscprintf("This is a test! %d", args);
        va_end(args);
        return length;
    }
};

TEST(StringUtils, Vscprintf)
{
    capu::int32_t length = VscprintfTest::Vscprintf("This is a test! %d", 12345);
    EXPECT_EQ(21, length);
}

TEST(StringUtils, LastIndexOf)
{
    EXPECT_EQ(5, capu::StringUtils::LastIndexOf("012345", '5'));
    EXPECT_EQ(8, capu::StringUtils::LastIndexOf("___512345", '5'));
    EXPECT_EQ(11, capu::StringUtils::LastIndexOf("______555555asdfasdf", '5'));
    EXPECT_EQ(-1, capu::StringUtils::LastIndexOf("111", '5'));
    EXPECT_EQ(-1, capu::StringUtils::LastIndexOf("", '5'));
    EXPECT_EQ(-1, capu::StringUtils::LastIndexOf(0, '5'));
}

TEST(StringUtils, IndexOf)
{
    EXPECT_EQ(5, capu::StringUtils::IndexOf("012345", '5'));
    EXPECT_EQ(0, capu::StringUtils::IndexOf("512345", '5'));
    EXPECT_EQ(0, capu::StringUtils::IndexOf("555555asdfasdf", '5'));
    EXPECT_EQ(-1, capu::StringUtils::IndexOf("111", '5'));
    EXPECT_EQ(-1, capu::StringUtils::IndexOf("", '5'));
    EXPECT_EQ(-1, capu::StringUtils::IndexOf(0, '5'));
}

TEST(StringUtils, StartsWith1)
{
    EXPECT_TRUE(capu::StringUtils::StartsWith("prefix", "prefix"));
    EXPECT_TRUE(capu::StringUtils::StartsWith("prefixwithsuffix", "prefix"));
    EXPECT_FALSE(capu::StringUtils::StartsWith("prefixwithsuffix", "noprefix"));

    EXPECT_TRUE(capu::StringUtils::StartsWith("prefix with spaces AND SOME TEXT", "prefix with spaces"));

    EXPECT_TRUE(capu::StringUtils::StartsWith("testing empty prefix", ""));

    EXPECT_FALSE(capu::StringUtils::StartsWith("DifferentCase", "differentCase"));
}

TEST(StringUtils, StartsWith2)
{
    EXPECT_FALSE(capu::StringUtils::StartsWith("prefixwithsuffix", 0));
    EXPECT_FALSE(capu::StringUtils::StartsWith(0, "prefix"));
}
