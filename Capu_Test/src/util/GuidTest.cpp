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

#include "gmock/gmock.h"
#include "capu/util/Guid.h"

TEST(GuidTest, TestNewAreNotEqual)
{
    capu::Guid id1;
    capu::Guid id2;
    EXPECT_FALSE(id1.equals(id2));
}

TEST(GuidTest, TestParseAreEqual1)
{
    capu::Guid id1;
    capu::Guid id2(id1.toString());
    EXPECT_TRUE(id1.equals(id2));
}

TEST(GuidTest, TestParseAreEqual2)
{
    capu::Guid id1;
    capu::Guid id2(id1.toString());
    EXPECT_TRUE(id1 == id2); // test == operator
}

TEST(GuidTest, TestGuidIsV4)
{
    capu::Guid id1;
    capu::String str = id1.toString();
    EXPECT_EQ('4', str.c_str()[14]);
}

TEST(GuidTest, TestParseInvalidGuid1)
{
    capu::Guid id1("THIS_IS_NO_GUID");
    EXPECT_TRUE(id1.isInvalid());
}

TEST(GuidTest, TestParseInvalidGuid2)
{
    capu::Guid id1("");
    EXPECT_TRUE(id1.isInvalid());
}

TEST(GuidTest, TestParseToString1)
{
    capu::Guid id1("D92BB305-D8B2-4B60-A3A3-A3CFDA57678F");
    EXPECT_EQ(capu::String("D92BB305-D8B2-4B60-A3A3-A3CFDA57678F"), id1.toString());
}

TEST(GuidTest, TestParseToString2)
{
    capu::Guid id1("BA28BA67-E777-4014-B8DD-6B928BDF57B1");
    EXPECT_EQ(capu::String("BA28BA67-E777-4014-B8DD-6B928BDF57B1"), id1.toString());
}

TEST(GuidTest, TestCopyConstructor)
{
    capu::Guid id1("BA28BA67-E777-4014-B8DD-6B928BDF57B1");
    capu::Guid id2(id1);
    EXPECT_TRUE(id1.equals(id2));
}

TEST(GuidTest, TestAssignmentOperator1)
{
    capu::Guid id1("BA28BA67-E777-4014-B8DD-6B928BDF57B1");
    capu::Guid id2;
    EXPECT_FALSE(id1.equals(id2));
    id2 = id1;
    EXPECT_TRUE(id1.equals(id2));
    EXPECT_EQ(capu::String("BA28BA67-E777-4014-B8DD-6B928BDF57B1"), id2.toString());
}

TEST(GuidTest, TestAssignmentOperator2)
{
    capu::Guid id1("BA28BA67-E777-4014-B8DD-6B928BDF57B1");
    capu::Guid id2;
    EXPECT_FALSE(id1.equals(id2));
    id2 = id1.getGuidData();
    EXPECT_TRUE(id1.equals(id2));
    EXPECT_EQ(capu::String("BA28BA67-E777-4014-B8DD-6B928BDF57B1"), id2.toString());
}

TEST(GuidTest, TestParse1)
{
    capu::Guid id1("BA28BA67-E777-4014-B8DD-6B928BDF57B1");
    capu::Guid id2;
    EXPECT_FALSE(id1.equals(id2));
    id2.parse(id1.toString());
    EXPECT_TRUE(id1.equals(id2));
    EXPECT_EQ(capu::String("BA28BA67-E777-4014-B8DD-6B928BDF57B1"), id2.toString());
}

TEST(GuidTest, TestParse2)
{
    capu::Guid id1;
    EXPECT_FALSE(id1.isInvalid());
    id1.parse("some strange guid string");
    EXPECT_TRUE(id1.isInvalid());
    EXPECT_EQ(capu::String("00000000-0000-0000-0000-000000000000"), id1.toString());
}

TEST(GuidTest, CopyConstructor)
{
    capu::Guid id1;
    capu::Guid id2(id1);

    EXPECT_STREQ(id1.toString().c_str(), id2.toString().c_str());
}
