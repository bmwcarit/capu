/*
* Copyright (C) 2015 BMW Car IT GmbH
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

#include "capu/util/Algorithm.h"
#include "capu/container/Vector.h"
#include "capu/container/String.h"
#include "capu/Config.h"
#include "gmock/gmock.h"
#include "BidirectionalTestContainer.h"
#include "ComplexTestType.h"

namespace capu
{
    TEST(AlgorithmTest, CopyPointers)
    {
        uint32_t src[3] = {2, 3, 4};
        uint32_t dst[4] = { 0 };

        uint32_t* result = copy(src, src + 3, dst);
        EXPECT_EQ(dst + 3, result);
        EXPECT_EQ(2u, dst[0]);
        EXPECT_EQ(3u, dst[1]);
        EXPECT_EQ(4u, dst[2]);
        EXPECT_EQ(0u, dst[3]);
    }

    TEST(AlgorithmTest, CopyVector)
    {
        Vector<uint_t> src(3, 0);
        src[0] = 2;
        src[1] = 3;
        src[2] = 4;
        Vector<uint_t> dst(4, 0);

        const Vector<uint_t>::Iterator result = copy(src.begin(), src.end(), dst.begin());
        EXPECT_EQ(dst.begin() + 3u, result);
        EXPECT_EQ(2u, dst[0]);
        EXPECT_EQ(3u, dst[1]);
        EXPECT_EQ(4u, dst[2]);
        EXPECT_EQ(0u, dst[3]);
    }

    TEST(AlgorithmTest, CopyZeroElements)
    {
        uint32_t src[2] = { 2, 3 };
        uint32_t dst[1] = { 0 };

        uint32_t* result = copy(src, src + 0, dst);
        EXPECT_EQ(dst, result);
        EXPECT_EQ(0u, dst[0]);
    }

    TEST(AlgorithmTest, CopyEmptyVector)
    {
        Vector<uint_t> src;
        Vector<uint_t> dst(2, 0);

        Vector<uint_t>::Iterator result = copy(src.begin(), src.end(), dst.begin());
        EXPECT_EQ(dst.begin(), result);
        EXPECT_EQ(0u, dst[0]);
    }

    TEST(AlgorithmTest, CopyBackwardsPointers)
    {
        uint32_t arr[4] = {2, 3, 4, 99};

        uint32_t* result = copy_backward(arr, arr + 3, arr + 4);
        EXPECT_EQ(arr + 1, result);
        EXPECT_EQ(2u, arr[0]);
        EXPECT_EQ(2u, arr[1]);
        EXPECT_EQ(3u, arr[2]);
        EXPECT_EQ(4u, arr[3]);
    }

    TEST(AlgorithmTest, CopyBackwardIntegralType)
    {
        BidirectionalTestContainer<uint32_t> src(3);
        src[0] = 2;
        src[1] = 3;
        src[2] = 4;
        Vector<uint_t> dst(4, 0);

        Vector<uint_t>::Iterator result = copy_backward(src.begin(), src.end(), dst.end()-1u);
        EXPECT_EQ(dst.begin(), result);
        EXPECT_EQ(2u, dst[0]);
        EXPECT_EQ(3u, dst[1]);
        EXPECT_EQ(4u, dst[2]);
        EXPECT_EQ(0u, dst[3]);
    }

    TEST(AlgorithmTest, CopyBackwardComplexType)
    {
        BidirectionalTestContainer<ComplexTestType> src(3);
        src[0] = 2;
        src[1] = 3;
        src[2] = 4;
        Vector<ComplexTestType> dst(4, 0);

        Vector<ComplexTestType>::Iterator result = copy_backward(src.begin(), src.end(), dst.end() - 1u);
        EXPECT_EQ(dst.begin(), result);
        EXPECT_EQ(ComplexTestType(2u), dst[0]);
        EXPECT_EQ(ComplexTestType(3u), dst[1]);
        EXPECT_EQ(ComplexTestType(4u), dst[2]);
        EXPECT_EQ(ComplexTestType(0u), dst[3]);
    }

    TEST(AlgorithmTest, FillArray)
    {
        uint_t a[3] = { 0 };
        uint_t* result = fill_n(a, 2, 1u);
        EXPECT_EQ(a + 2, result);
        EXPECT_EQ(1u, a[0]);
        EXPECT_EQ(1u, a[1]);
        EXPECT_EQ(0u, a[2]);
    }

    TEST(AlgorithmTest, FillArrayWithZeroElements)
    {
        uint_t a[3] = { 99, 99, 99 };
        uint_t* result = fill_n(a, 0, 1u);
        EXPECT_EQ(a, result);
        EXPECT_EQ(99u, a[0]);
        EXPECT_EQ(99u, a[1]);
        EXPECT_EQ(99u, a[2]);
    }

    TEST(AlgorithmTest, FillVector)
    {
        Vector<uint_t> v(3, 0);
        Vector<uint_t>::Iterator result = fill_n(v.begin(), 2, 1u);
        EXPECT_EQ(v.begin() + 2u, result);
        EXPECT_EQ(1u, v[0]);
        EXPECT_EQ(1u, v[1]);
        EXPECT_EQ(0u, v[2]);
    }

    TEST(AlgorithmTest, FillVectorWithZeroElements)
    {
        Vector<uint_t> v(3, 99);
        Vector<uint_t>::Iterator result = fill_n(v.begin(), 0, 1u);
        EXPECT_EQ(v.begin(), result);
        EXPECT_EQ(99u, v[0]);
        EXPECT_EQ(99u, v[1]);
        EXPECT_EQ(99u, v[2]);
    }

    TEST(AlgorithmTest, EqualSame)
    {
        uint_t a[3] = { 1, 2, 3 };
        uint_t b[3] = { 1, 2, 3 };
        EXPECT_TRUE(equal(a, a+3, b));
    }

    TEST(AlgorithmTest, EqualSameComplexType)
    {
        String a[3] = { "1", "2", "3" };
        String b[3] = { "1", "2", "3" };
        EXPECT_TRUE(equal(a, a + 3, b));
    }

    TEST(AlgorithmTest, EqualNotSame)
    {
        uint_t a[3] = { 1, 2, 3 };
        uint_t b[3] = { 1, 5, 3 };
        EXPECT_FALSE(equal(a, a + 3, b));
    }

    TEST(AlgorithmTest, EqualNotSameComplexType)
    {
        String a[3] = { "1", "2", "3" };
        String b[3] = { "1", "5", "3" };
        EXPECT_FALSE(equal(a, a + 3, b));
    }

    TEST(AlgorithmTest, EqualOnEmpty)
    {
        uint_t a[1] = { 1 };
        uint_t b[1] = { 2 };
        EXPECT_TRUE(equal(a, a + 0, b));
    }
}
