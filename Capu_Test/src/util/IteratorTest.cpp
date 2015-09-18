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

#include "capu/util/Iterator.h"
#include "capu/container/Vector.h"
#include "capu/Config.h"
#include "BidirectionalTestContainer.h"
#include "gmock/gmock.h"

namespace capu
{
    TEST(IteratorTest, DistanceOnPointer)
    {
        uint_t a[4] = { 1, 2, 3, 4 };
        int_t result = distance(a, a + 4);
        EXPECT_EQ(4, result);
    }

    TEST(IteratorTest, DistanceOnSamePointer)
    {
        uint_t a[1] = { 1 };
        int_t result = distance(a, a);
        EXPECT_EQ(0, result);
    }

    TEST(IteratorTest, DistanceOnPointerNegative)
    {
        uint_t a[4] = { 1, 2, 3, 4 };
        int_t result = distance(a + 4, a);
        EXPECT_EQ(-4, result);
    }

    TEST(IteratorTest, DistanceOnVector)
    {
        Vector<uint_t> v(3, 0);
        int_t result = distance(v.begin(), v.end());
        EXPECT_EQ(3, result);
    }

    TEST(IteratorTest, DistanceOnVectorSameIterator)
    {
        Vector<uint_t> v(3, 0);
        int_t result = distance(v.begin(), v.begin());
        EXPECT_EQ(0, result);
    }

    TEST(IteratorTest, DistanceOnVectorNegative)
    {
        Vector<uint_t> v(3, 0);
        int_t result = distance(Vector<uint_t>::Iterator(v.begin() + 2u), v.begin());
        EXPECT_EQ(-2, result);
    }

    TEST(IteratorTest, DistanceOnBidirectionalIterator)
    {
        BidirectionalTestContainer<uint_t> c(5);
        int_t result = distance(c.begin(), c.end());
        EXPECT_EQ(5, result);
    }

    TEST(IteratorTest, DistanceOnBidirectionalIteratorSameIterator)
    {
        BidirectionalTestContainer<uint_t> c(5);
        int_t result = distance(c.end(), c.end());
        EXPECT_EQ(0, result);
    }

    TEST(IteratorTest, AdvanceOnPointer)
    {
        uint_t a[4] = { 1, 2, 3, 4 };
        uint_t *p = a;
        advance(p, 2);
        EXPECT_EQ(3u, *p);
    }

    TEST(IteratorTest, AdvanceZeroOnPointer)
    {
        uint_t a[2] = { 1, 2 };
        uint_t *p = a;
        advance(p, 0);
        EXPECT_EQ(1u, *p);
    }

    TEST(IteratorTest, AdvanceNegativeOnPointer)
    {
        uint_t a[4] = { 1, 2, 3, 4 };
        uint_t *p = &a[3];
        advance(p, -3);
        EXPECT_EQ(1u, *p);
    }

    TEST(IteratorTest, AdvanceOnVector)
    {
        Vector<uint_t> v(4, 0);
        v[0] = 1;
        v[1] = 2;
        v[2] = 3;
        v[3] = 4;
        Vector<uint_t>::Iterator it = v.begin();
        advance(it, 2);
        EXPECT_EQ(3u, *it);
    }

    TEST(IteratorTest, AdvanceZeroOnVector)
    {
        Vector<uint_t> v(2, 0);
        v[0] = 1;
        v[1] = 2;
        Vector<uint_t>::Iterator it = v.begin();
        advance(it, 0);
        EXPECT_EQ(1u, *it);
    }

    TEST(IteratorTest, AdvanceNegativeOnVector)
    {
        Vector<uint_t> v(4, 0);
        v[0] = 1;
        v[1] = 2;
        v[2] = 3;
        v[3] = 4;
        Vector<uint_t>::Iterator it = v.begin() + 3u;
        advance(it, -3);
        EXPECT_EQ(1u, *it);
    }

    TEST(IteratorTest, AdvanceOnBidirectionalContainer)
    {
        BidirectionalTestContainer<uint_t> c(4);
        c[0] = 1;
        c[1] = 2;
        c[2] = 3;
        c[3] = 4;
        BidirectionalTestContainer<uint_t>::Iterator it = c.begin();
        advance(it, 2);
        EXPECT_EQ(3u, *it);
    }

    TEST(IteratorTest, AdvanceZeroOnBidirectionalContainer)
    {
        BidirectionalTestContainer<uint_t> c(2);
        c[0] = 1;
        c[1] = 2;
        BidirectionalTestContainer<uint_t>::Iterator it = c.begin();
        advance(it, 0);
        EXPECT_EQ(1u, *it);
    }

    TEST(IteratorTest, AdvanceNegativeOnBidirectionalContainer)
    {
        BidirectionalTestContainer<uint_t> c(4);
        c[0] = 1;
        c[1] = 2;
        c[2] = 3;
        c[3] = 4;
        BidirectionalTestContainer<uint_t>::Iterator it = c.end();
        advance(it, -3);
        EXPECT_EQ(2u, *it);
    }
}
