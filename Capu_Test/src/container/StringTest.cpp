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
#include "capu/container/String.h"
#include "capu/Error.h"
#include "capu/Config.h"

TEST(String, TestCTor)
{
    capu::String str;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("", str.c_str()));
}

TEST(String, TestCStr)
{
    capu::String str("asdf");
    EXPECT_EQ(0, capu::StringUtils::Strcmp("asdf", str.c_str()));
    EXPECT_EQ(4u, str.getLength());
}

TEST(String, TestCopyConstructor)
{
    capu::String str("asdf");
    capu::String copy(str);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("asdf", str.c_str()));
    EXPECT_STREQ("asdf", str.c_str());
    EXPECT_EQ(str.getLength(), copy.getLength());
}

TEST(String, InitialSizeConstructor)
{
    capu::String str(5, 'a');

    EXPECT_EQ(5u, str.getLength());
    EXPECT_STREQ("aaaaa", str.c_str());
}

TEST(String, TestAssignOperator1)
{
    capu::String str("asdf");
    capu::String other("other");
    
    EXPECT_EQ(4u, str.getLength());
    EXPECT_EQ(5u, other.getLength());
    str = other;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("other", str.c_str()));
    EXPECT_EQ(5u, str.getLength());
    EXPECT_EQ(5u, other.getLength());

    capu::String str2;
    capu::String other2;
    str2 = other2;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("", str2.c_str()));

    // one string on stack, one on heap
    capu::String stringStack1;
    capu::String stringStack2;
    capu::String *stringHeap1 = new capu::String();
    capu::String *stringHeap2 = new capu::String();


    stringStack1 = *stringHeap1;
    EXPECT_EQ(0u, stringHeap1->getLength());
    EXPECT_EQ(0u, stringStack1.getLength());
    delete stringHeap1;

    *stringHeap2 = stringStack2;
    EXPECT_EQ(0u, stringHeap2->getLength());
    EXPECT_EQ(0u, stringStack2.getLength());
    delete stringHeap2;
}

TEST(String, TestToLowerCase1)
{
    capu::String str("abcDEF");
    str.toLowerCase();
    EXPECT_EQ(capu::String("abcdef"), str);
}

TEST(String, TestToLowerCase2)
{
    capu::String str("");
    str.toLowerCase();
    EXPECT_EQ(capu::String(""), str);
}

TEST(String, TestToLowerCase3)
{
    capu::String str("ABC");
    str.toLowerCase();
    EXPECT_EQ(capu::String("abc"), str);
}

TEST(String, TestToLowerCase4)
{
    capu::String str("AbC");
    str.toLowerCase();
    EXPECT_EQ(capu::String("abc"), str);
}

TEST(String, TestToLowerCase5)
{
    capu::String str("Abc");
    str.toLowerCase();
    EXPECT_EQ(capu::String("abc"), str);
}

TEST(String, TestToUpperCase1)
{
    capu::String str("abcDEF");
    str.toUpperCase();
    EXPECT_EQ(capu::String("ABCDEF"), str);
}

TEST(String, TestToUpperCase2)
{
    capu::String str("");
    str.toUpperCase();
    EXPECT_EQ(capu::String(""), str);
}

TEST(String, TestToUpperCase3)
{
    capu::String str("ABC");
    str.toUpperCase();
    EXPECT_EQ(capu::String("ABC"), str);
}

TEST(String, TestToUpperCase4)
{
    capu::String str("AbC");
    str.toUpperCase();
    EXPECT_EQ(capu::String("ABC"), str);
}

TEST(String, TestToUpperCase5)
{
    capu::String str("Abc");
    str.toUpperCase();
    EXPECT_EQ(capu::String("ABC"), str);
}

TEST(String, TestAssignOperator2)
{
    capu::String str("asdf");
    str = "other";
    EXPECT_EQ(0, capu::StringUtils::Strcmp("other", str.c_str()));
    EXPECT_EQ(5u, str.getLength());
}

TEST(String, TestAssignOperator3)
{
    capu::String str("asdf");
    str = '\0';
    EXPECT_EQ(0, capu::StringUtils::Strcmp("", str.c_str()));
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestAssignOperator4)
{
    capu::String str("asdf");
    capu::String other;
    str = other;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("", str.c_str()));
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestAssignOperator5)
{
    capu::String str("asdf");
    str = 'z';
    EXPECT_STREQ("z", str.c_str());
    EXPECT_EQ(1u, str.getLength());
}

TEST(String, TestAssignOperator6)
{
    capu::String str("asdf");
    str = '\0';
    EXPECT_STREQ("", str.c_str());
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestAppend1)
{
    capu::String str("hello");
    capu::String other("world");
    str.append(other);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str.c_str()));
    EXPECT_EQ(10u, str.getLength());
}

TEST(String, TestAppend2)
{
    capu::String str("hello");
    capu::String other;
    str.append(other);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str.c_str()));
    EXPECT_EQ(5u, str.getLength());
}

TEST(String, TestAppend3)
{
    capu::String str;
    capu::String other("world");
    str.append(other);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str.c_str()));
    EXPECT_EQ(5u, str.getLength());
}

TEST(String, TestAppend4)
{
    capu::String str("hello");
    str.append("world");
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str.c_str()));
    EXPECT_EQ(10u, str.getLength());
}

TEST(String, TestPlusOperator1)
{
    capu::String str1("hello");
    capu::String str2("world");
    capu::String str3 = str1 + str2;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str1.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str2.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str3.c_str()));
}

TEST(String, TestPlusOperator2)
{
    capu::String str1("hello");
    capu::String str2;
    capu::String str3 = str1 + str2;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str1.c_str()));
    EXPECT_EQ(0u, str2.getLength());
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str3.c_str()));
}

TEST(String, TestPlusOperator3)
{
    capu::String str1;
    capu::String str2("world");
    capu::String str3 = str1 + str2;
    EXPECT_EQ(0u, str1.getLength());
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str2.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str3.c_str()));
}

TEST(String, TestPlusOperator4)
{
    capu::String str1("hello");
    capu::String str2 = str1 + "world";
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str1.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str2.c_str()));
}

TEST(String, TestPlusOperator5)
{
    capu::String str1("world");
    capu::String str2 = "hello" + str1;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str1.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str2.c_str()));
}

TEST(String, TestAddition)
{
    capu::String str1("hello");
    capu::String str2("world");
    capu::String str3 = str1 + str2;
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str1.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("world", str2.c_str()));
    EXPECT_EQ(0, capu::StringUtils::Strcmp("helloworld", str3.c_str()));
}

TEST(String, TestAddAssignString)
{
    capu::String str ("abc");
    str += "def";

    EXPECT_EQ(6u, str.getLength());
    EXPECT_STREQ("abcdef", str.c_str());
}

TEST(String, TestAddAssignChar)
{
    capu::String str ("abc");
    str += 'd';

    EXPECT_EQ(4u, str.getLength());
    EXPECT_STREQ("abcd", str.c_str());
}

TEST(String, TestSubStringCTor1)
{
    capu::String str("0123456789", 4, 6);
    EXPECT_STREQ("456", str.c_str());
    EXPECT_EQ(3u, str.getLength());
}

TEST(String, TestSubStringCTor2)
{
    capu::String str("0123456789", 4);
    EXPECT_STREQ("456789", str.c_str());
    EXPECT_EQ(6u, str.getLength());
}

TEST(String, TestSubStringCTor3)
{
    capu::String str(static_cast<const char*>(0), 4);
    EXPECT_STREQ("", str.c_str());
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestSubStringCTor4)
{
    capu::String str(0, 4, 9);
    EXPECT_STREQ("", str.c_str());
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestSubStringCTor5)
{
    capu::String str(0, 9, 4);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("", str.c_str()));
    EXPECT_EQ(0u, str.getLength());
}

TEST(String, TestSubStringCTor6)
{
    capu::String str("hello", 0, 100);
    EXPECT_EQ(0, capu::StringUtils::Strcmp("hello", str.c_str()));
    EXPECT_EQ(5u, str.getLength());
}

TEST(String, AccessOperator)
{
    capu::String str("abc");

    EXPECT_EQ('b', str[1]);
    
    str[1] = 'z';
    EXPECT_STREQ("azc", str.c_str());
}


TEST(String, AutoCast)
{
    capu::String string = "TestString";
    EXPECT_STREQ("TestString", string.c_str());
    EXPECT_EQ(10u, string.getLength());
}

TEST(String, Equals1)
{
    capu::String str1;
    capu::String str2;
    EXPECT_TRUE(str1 == str2);
    EXPECT_TRUE(str2 == str1);
}

TEST(String, Equals2)
{
    capu::String str1;
    capu::String str2("nonnull");
    EXPECT_FALSE(str1 == str2);
    EXPECT_FALSE(str2 == str1);
}

TEST(String, NotEquals1)
{
    capu::String str1;
    capu::String str2;
    EXPECT_FALSE(str1 != str2);
    EXPECT_FALSE(str2 != str1);

    str2 = "";
    EXPECT_FALSE(str1 != str2);
    EXPECT_FALSE(str2 != str1);

    str2 = "test";
    EXPECT_TRUE(str1 != str2);
    EXPECT_TRUE(str2 != str1);
}

TEST(String, NotEquals2)
{
    capu::String str1("hello");
    capu::String str2("world");
    capu::String str3("world");
    EXPECT_TRUE(str1 != str2);
    EXPECT_FALSE(str2 != str3);
}

TEST(String, FindChar)
{
    capu::String str1;
    capu::String str2("hello world");
    capu::String str3("");

    EXPECT_EQ(-1, str1.find('a'));
    EXPECT_EQ(0, str1.find(0)); // indexof(0) = strlen
    EXPECT_EQ(4, str2.find('o'));
    EXPECT_EQ(11, str2.find(0)); // indexof(0) = strlen
    EXPECT_EQ(-1, str2.find('O'));
    EXPECT_EQ(-1, str3.find('o'));
    EXPECT_EQ(0, str3.find(0)); // indexof(0) = strlen
}

TEST(String, FindCharOffset)
{
    capu::String str1;
    capu::String str2("hello world");
    capu::String str3("");

    EXPECT_EQ(-1, str1.find('o', 12));
    EXPECT_EQ(-1, str1.find('o', 0));

    EXPECT_EQ(4, str2.find('o',  1));
    EXPECT_EQ(5, str2.find(' ', 0));
    EXPECT_EQ(7, str2.find('o', 5));

    EXPECT_EQ(-1, str2.find('o', 20));

}

TEST(String, FindStringOffset)
{
    capu::String str("hello world I am your old man");

    EXPECT_EQ(5, str.find(" ", 0));
    EXPECT_EQ(11, str.find(" ", 6));
    EXPECT_EQ(13, str.find(" ", 12));
    EXPECT_EQ(16, str.find(" ", 14));
    EXPECT_EQ(21, str.find(" ", 17));
    EXPECT_EQ(25, str.find(" ", 22));

    EXPECT_EQ(9, str.find("ld",  0));
    EXPECT_EQ(23, str.find("ld", 10));

    EXPECT_EQ(-1, str.find("ld", 25));

    EXPECT_EQ(-1, str.find("ld", 29));
    EXPECT_EQ(-1, str.find("ld", 30));

}

TEST(String, RFindChar)
{
    capu::String str1;
    capu::String str2("hello world");
    capu::String str3("");

    EXPECT_EQ(-1, str1.rfind('a'));
    EXPECT_EQ(0, str1.rfind(0)); // indexof(0) = strlen
    EXPECT_EQ(7, str2.rfind('o'));
    EXPECT_EQ(11, str2.rfind(0)); // indexof(0) = strlen
    EXPECT_EQ(-1, str2.rfind('O'));
    EXPECT_EQ(-1, str3.rfind('o'));
    EXPECT_EQ(0, str3.rfind(0)); // indexof(0) = strlen
}

TEST(String, Truncate)
{
    capu::String str1("hello world");
    str1.truncate(5);
    EXPECT_STREQ("hello", str1.c_str());
    EXPECT_EQ(5u, str1.getLength());

    // test chaining
    capu::String str2("hello world");
    str2.truncate(7).truncate(5);
    EXPECT_STREQ("hello", str2.c_str());
    EXPECT_EQ(5u, str2.getLength());

    // truncate to size of string
    capu::String str3("hello");
    str3.truncate(5);
    EXPECT_STREQ("hello", str3.c_str());
    EXPECT_EQ(5u, str3.getLength());

    // truncate to greater size of string
    capu::String str4("hello");
    str4.truncate(7);
    EXPECT_STREQ("hello", str4.c_str());
    EXPECT_EQ(5u, str4.getLength());

    // truncate with 0 makes empty string
    capu::String str5("hello");
    str5.truncate(0);
    EXPECT_STREQ("", str5.c_str());
    EXPECT_EQ(0u, str5.getLength());
}

TEST(String, FindSubstring)
{
    capu::String str1("hello c++ world.");
    EXPECT_EQ(6, str1.find("c++"));

    // test when the substring is at the start of the string
    capu::String str2("hello c++ world.");
    EXPECT_EQ(0, str2.find("hello"));

    // test when the substring is at the end of the string
    capu::String str3("hello c++ world.");
    EXPECT_EQ(10, str3.find("world."));

    // test substring not found
    capu::String str4("hello c++ world.");
    EXPECT_EQ(-1, str4.find("nosubstring"));

    // test substring is empty
    capu::String str5("hello c++ world.");
    EXPECT_EQ(0, str5.find(""));

    // test string is empty
    capu::String str6("");
    EXPECT_EQ(-1, str6.find("hello"));

    capu::String str7("");
    EXPECT_EQ(0, str7.find(""));
}

TEST(String, GetSubstring)
{
    capu::String str1("hello c++ world.");
    capu::String substr = str1.substr(0, 5);
    EXPECT_EQ(5u, substr.getLength());
    EXPECT_STREQ("hello", substr.c_str());

    // test when start is out of bounds
    capu::String substr2 = str1.substr(25, 5);
    EXPECT_EQ(0u, substr2.getLength());

    // test negative length
    capu::String substr3 = str1.substr(6, -1);
    EXPECT_STREQ("c++ world.", substr3.c_str());

    // test when length is too large
    capu::String substr4 = str1.substr(6, 500);
    EXPECT_STREQ("c++ world.", substr4.c_str());

    // take only the last character of the string
    capu::String substr5 = str1.substr(15, 5);
    EXPECT_STREQ(".", substr5.c_str());

    // length is 0
    capu::String substr6 = str1.substr(2, 0);
    EXPECT_STREQ("", substr6.c_str());

    // start is exactly end of string
    capu::String substr7 = str1.substr(str1.getLength(), 4);
    EXPECT_STREQ("", substr7.c_str());
}

TEST(String, StartsWith)
{
    capu::String str("hello c++ world.");


    EXPECT_TRUE(str.startsWith("hello"));
    EXPECT_TRUE(str.startsWith("h"));
    EXPECT_TRUE(str.startsWith("hello c++ world."));
    EXPECT_FALSE(str.startsWith("c++"));

}

TEST(String, EndsWith)
{
    capu::String str("hello c++ world.");

    EXPECT_TRUE(str.endsWith("."));
    EXPECT_TRUE(str.endsWith("world."));
    EXPECT_TRUE(str.endsWith("hello c++ world."));
    EXPECT_FALSE(str.endsWith("c++"));

    capu::String path("D:");
    EXPECT_FALSE(path.endsWith("\\"));

    capu::String path2("D:\\dir1\\dir2");
    EXPECT_FALSE(path2.endsWith("\\"));

    capu::String path3("D:\\dir1\\dir2\\");
    EXPECT_TRUE(path3.endsWith("\\"));
}
