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

    TEST_F(VectorTest, ConstructorWithCapacity)
    {
        Vector<uint32_t> vector(3);
        EXPECT_EQ(0u, vector.size());
    }

    TEST_F(VectorTest, ConstructorWithCapacityAndValue)
    {
        Vector<uint32_t> vector(3, 5);

        EXPECT_EQ(5u, vector[0]);
        EXPECT_EQ(5u, vector[1]);
        EXPECT_EQ(5u, vector[2]);
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


    TEST_F(VectorTest, IteratorInc)
    {
        Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator current = vector.begin();

        EXPECT_EQ(1u, *current);
        ++current;
        EXPECT_EQ(2u, *current);
        ++current;
        EXPECT_EQ(3u, *current);
        ++current;
        EXPECT_EQ(4u, *current);
    }


    TEST_F(VectorTest, IteratorNotEqual)
    {
        Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator start = vector.begin();
        Vector<uint32_t>::Iterator end = vector.end();

        EXPECT_TRUE(start != end);
        end = start;
        EXPECT_FALSE(start != end);

    }

    TEST_F(VectorTest, IteratorSmaller)
    {
        Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator start = vector.begin();
        Vector<uint32_t>::Iterator end = vector.end();

        EXPECT_TRUE(start < end);
        EXPECT_FALSE(end < start);
    }

    TEST_F(VectorTest, IteratorBigger)
    {
        Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator start = vector.begin();
        Vector<uint32_t>::Iterator end = vector.end();

        EXPECT_TRUE(end > start);
        EXPECT_FALSE(start > end);
    }

    TEST_F(VectorTest, IteratorAddValue)
    {
        Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator start = vector.begin();

        EXPECT_EQ(1u, *(start + 0u));
        EXPECT_EQ(2u, *(start + 1u));
        EXPECT_EQ(3u, *(start + 2u));
        EXPECT_EQ(4u, *(start + 3u));
    }

    TEST_F(VectorTest, IteratorSubValue)
    {
         Vector<uint32_t> vector;

        vector.push_back(1u);
        vector.push_back(2u);
        vector.push_back(3u);
        vector.push_back(4u);

        Vector<uint32_t>::Iterator end = vector.end();

        EXPECT_EQ(4u, *(end - 1u));
        EXPECT_EQ(3u, *(end - 2u));
        EXPECT_EQ(2u, *(end - 3u));
        EXPECT_EQ(1u, *(end - 4u));
    }

    TEST_F(VectorTest, Iterator)
    {
        Vector<uint32_t> vector;

        vector.push_back(42u);
        vector.push_back(47u);

        Vector<uint32_t> vector2;

        for (Vector<uint32_t>::Iterator iter = vector.begin(); iter != vector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(42u, vector2[0]);
        EXPECT_EQ(47u, vector2[1]);
    }

    TEST_F(VectorTest, IteratorOnConstVector)
    {
        Vector<uint32_t> vector;

        const Vector<uint32_t>& constVector = vector;

        vector.push_back(42u);
        vector.push_back(47u);

        Vector<uint32_t> vector2;

        for (Vector<uint32_t>::ConstIterator iter = constVector.begin(); iter != constVector.end(); ++iter)
        {
            vector2.push_back(*iter);
        }

        EXPECT_EQ(2u, vector2.size());
        EXPECT_EQ(42u, vector2[0]);
        EXPECT_EQ(47u, vector2[1]);
    }

    TEST_F(VectorTest, AccessOperator)
    {
        Vector<uint32_t> vector;

        vector.push_back(42u);
        vector.push_back(47u);

        vector[0] = 47u;
        vector[1] = 42u;

        EXPECT_EQ(47u, vector[0]);
        EXPECT_EQ(42u, vector[1]);
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

        for (Vector<TestStruct>::Iterator iter = vector.begin(); iter != vector.end(); ++iter)
        {
            vector2.push_back(iter->value1);
        }

        EXPECT_EQ(47u, vector2[0]);
        EXPECT_EQ(8u, vector2[1]);
    }

    TEST_F(VectorTest, Resize)
    {
        Vector<uint32_t> vector(2);

        vector.push_back(1);
        vector.push_back(2);

        vector.resize(19);

        EXPECT_EQ(1u, vector[0]);
        EXPECT_EQ(2u, vector[1]);

        vector.resize(1);

        EXPECT_EQ(1u, vector[0]);
    }

    TEST_F(VectorTest, Clear)
    {
        TestStruct struct1;
        struct1.value1 = 47;
        struct1.value2   = 11.f;

        TestStruct struct2;
        struct2.value1 = 8;
        struct2.value2 = 15.f;


        Vector<TestStruct> vector;

        vector.push_back(struct1);
        vector.push_back(struct2);

        vector.clear();

        vector.push_back(struct2);
        vector.push_back(struct1);

        EXPECT_EQ(struct2.value1, vector[0].value1);
        EXPECT_EQ(struct2.value2, vector[0].value2);
        EXPECT_EQ(struct1.value1, vector[1].value1);
        EXPECT_EQ(struct1.value2, vector[1].value2);


    }
}
