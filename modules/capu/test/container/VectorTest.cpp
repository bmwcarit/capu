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

#include <container/VectorTest.h>

namespace capu
{
    VectorTest::VectorTest()
    {

    }

    VectorTest::~VectorTest()
    {

    }

    void VectorTest::SetUp()
    {

    }

    void VectorTest::TearDown()
    {

    }

    TEST_F(VectorTest, Constructor)
    {
        Vector<uint32_t> vector;
        EXPECT_EQ(0U, vector.size());
    }

    TEST_F(VectorTest, PushBack)
    {
        Vector<uint32_t> vector;

        vector.push_back(42u);
        vector.push_back(47u);

        EXPECT_EQ(42u, vector[0]);
        EXPECT_EQ(47u, vector[1]);
    }

    TEST_F(VectorTest, PushBack2)
    {
        Vector<uint32_t> vector(1);

        vector.push_back(42u);
        vector.push_back(47u);

        EXPECT_EQ(42u, vector[0]);
        EXPECT_EQ(47u, vector[1]);
    }

    TEST_F(VectorTest, Iterator)
    {
        Vector<uint32_t> vector;

        vector.push_back(42u);
        vector.push_back(47u);

        Vector<uint32_t> vector2;

        for(Vector<uint32_t>::Iterator iter = vector.begin(); iter != vector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(42u, vector2[0]);
        EXPECT_EQ(47u, vector2[1]);
    }

    struct TestStruct
    {
        uint32_t value1;
        float_t  value2;
    };

    TEST_F(VectorTest, Object)
    {
        Vector<TestStruct> vector;

        TestStruct struct1;
        struct1.value1 = 47;
        struct1.value2   = 11.f;

        TestStruct struct2;
        struct2.value1 = 8;
        struct2.value2 = 15.f;

        vector.push_back(struct1);
        vector.push_back(struct2);

        Vector<uint32_t> vector2;

        for(Vector<TestStruct>::Iterator iter = vector.begin(); iter != vector.end(); ++iter)
        {
            vector2.push_back(iter->value1);
        }

        EXPECT_EQ(47u, vector2[0]);
        EXPECT_EQ(8u, vector2[1]);
    }
}