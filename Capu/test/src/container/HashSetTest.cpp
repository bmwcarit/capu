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
#include "capu/container/HashSet.h"
#include "capu/container/vector.h"
#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/util/Guid.h"

TEST(HashSet, Constructor_Default)
{
    capu::HashSet<int32_t>* list = new capu::HashSet<int32_t > ();
    delete list;
}

TEST(HashSet, copyConstructor)
{
    capu::HashSet<int32_t> list;
    list.put(1);
    list.put(2);
    list.put(3);

    capu::HashSet<int32_t> list2 = list; // copy

    list.clear();

    EXPECT_TRUE(list2.hasElement(1));
    EXPECT_TRUE(list2.hasElement(2));
    EXPECT_TRUE(list2.hasElement(3));
}

TEST(HashSet, put)
{
    int32_t value2 = 10;
    int32_t value = 5;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    // add new key
    EXPECT_EQ(capu::CAPU_OK,  h1->put(value));

    // add new key
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    // add existing key
    EXPECT_EQ(capu::CAPU_ERROR, h1->put(value2));

    delete h1;
}

TEST(HashSet, count)
{
    int32_t value2 = 10;
    int32_t value = 5;
    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    //check count
    EXPECT_EQ(0u, h1->count());

    // add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    // add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    EXPECT_EQ(2u, h1->count());

    EXPECT_EQ(capu::CAPU_OK, h1->remove(value2));

    EXPECT_EQ(1u, h1->count());

    delete h1;
}

TEST(HashSet, clear)
{
    int32_t value = 5;
    int32_t value2 = 6;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();
    // add new keys
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    //add new keys
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    // check count
    EXPECT_EQ(2u, h1->count());

    //remove all
    EXPECT_EQ(capu::CAPU_OK, h1->clear());

    //check count
    EXPECT_EQ(0u, h1->count());

    delete h1;
}

TEST(HashSet, remove)
{
    int32_t value = 5;
    int32_t value2 = 6;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    // add new keys
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    //delete a non existing value
    EXPECT_EQ(capu::CAPU_ERANGE, h1->remove(value2));

    //add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    // check count
    EXPECT_EQ(2u, h1->count());

    //delete existing value
    EXPECT_EQ(capu::CAPU_OK, h1->remove(value2));

    //check count
    EXPECT_EQ(1u, h1->count());

    delete h1;
}

TEST(HashSet, hasElement)
{
    int32_t value = 5;
    int32_t value2 = 6;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    // add new keys
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    EXPECT_FALSE(h1->hasElement(value2));

    //add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    //delete existing value
    EXPECT_TRUE(h1->hasElement(value2));

    delete h1;
}

TEST(HashSetIterator, hasNext)
{
    int32_t value = 10;
    int32_t value2 = 12;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    //create iterator
    capu::HashSet<int32_t>::Iterator it = h1->begin();

    //check hasNext
    EXPECT_EQ(it, h1->end());

    // add new values
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    //add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));

    it = h1->begin();
    EXPECT_NE(it, h1->end());

    delete h1;
}

TEST(HashSetIterator, next)
{
    int32_t value = 10;
    int32_t value2 = 12;

    capu::HashSet<int32_t>* h1 = new capu::HashSet<int32_t > ();

    int32_t check_value = 0;
    int32_t check_value2 = 0;

    //create iterator
    capu::HashSet<int32_t>::Iterator it = h1->begin();

    //check hasNext
    EXPECT_TRUE(it == h1->end());

    // add new keys
    EXPECT_EQ(capu::CAPU_OK, h1->put(value));

    //add new value
    EXPECT_EQ(capu::CAPU_OK, h1->put(value2));
    it = h1->begin();

    check_value = *it;
    EXPECT_TRUE(check_value == value || check_value == value2);

    it++;
    check_value = *it;
    EXPECT_TRUE(check_value == value || check_value == value2);

    EXPECT_NE(check_value, check_value2);
    delete h1;
}

TEST(HashSetIterator, ForEach)
{
    capu::HashSet<int32_t> hashSet;

    hashSet.put(32);
    hashSet.put(43);
    hashSet.put(44);

    capu::HashSet<int32_t> testHashSet;

    capu_foreach(capu::HashSet<int32_t>, hashSet, iter)
    {
        testHashSet.put(*iter);
    }

    EXPECT_TRUE(testHashSet.hasElement(32));
    EXPECT_TRUE(testHashSet.hasElement(43));
    EXPECT_TRUE(testHashSet.hasElement(44));

}


class HashSetPerformance : public ::testing::Test
{
public:
    HashSetPerformance()
        : count(500000)
    {}

    static capu::HashSet<uint32_t> set;
    uint32_t count;
};

capu::HashSet<uint32_t> HashSetPerformance::set;

TEST_F(HashSetPerformance, performancePut)
{
    for (uint32_t i = 0; i < count; i++)
    {
        set.put(i);
    }
}

TEST_F(HashSetPerformance, performanceIterate)
{
    capu::HashSet<uint32_t>::Iterator iter = set.begin();
    while (iter != set.end())
    {
        iter++;
    }
}

TEST_F(HashSetPerformance, performanceConstIterate)
{
    const capu::HashSet<uint32_t>& setConstRef = set;
    capu::HashSet<uint32_t>::ConstIterator iter = setConstRef.begin();
    while (iter != setConstRef.end())
    {
        iter++;
    }
}

TEST_F(HashSetPerformance, performanceRemove)
{
    for (uint32_t i = 0; i < count; i++)
    {
        set.remove(i);
    }
}

TEST(HashSet, iteratorPointsToNextElementAfterDeletion)
{
    capu::HashSet<uint32_t> set;

    set.put(1);
    set.put(2);
    set.put(3);

    // point to middle element
    capu::HashSet<uint32_t>::Iterator i1 = set.begin();
    capu::HashSet<uint32_t>::Iterator i2 = set.begin(); ++i2;
    capu::HashSet<uint32_t>::Iterator i3 = set.begin(); ++i3; ++i3;

    // all iterators point to different elements
    // no assumption which value each iterator points to, because no order is defined
    ASSERT_NE(*i1, *i2);
    ASSERT_NE(*i2, *i3);
    ASSERT_NE(*i1, *i3);

    set.removeAt(i2);

    // i2 now points at next element -> i3
    EXPECT_EQ(i2, i3);

    set.removeAt(i1);
    EXPECT_EQ(i1, i3);
}

TEST(HashSet, canRemoveElementsDuringCycle)
{
    capu::HashSet<uint32_t> set;

    set.put(1);
    set.put(2);
    set.put(3);

    capu::HashSet<uint32_t>::Iterator iter = set.begin();
    while (iter != set.end())
    {
        set.removeAt(iter);
    }

    EXPECT_EQ(0u, set.count());
}

TEST(HashSet, HashSetWithGuids)
{
    typedef capu::HashSet<capu::Guid> GuidSet;
    GuidSet set;
    capu::vector<capu::Guid> vec;
    for (capu::uint_t i = 0; i < 20; ++i)
    {
        capu::Guid guid = capu::Guid(true);
        set.put(guid);
        vec.push_back(guid);
    }

    for (capu::uint_t i = 0; i < vec.size(); ++i)
    {
        const capu::Guid& guid = vec[i];
        EXPECT_TRUE(set.hasElement(guid));
    }
}

TEST(HashSet, swapMemberFunction)
{
    capu::HashSet<uint32_t> first;
    capu::HashSet<uint32_t> second;

    first.put(1);
    first.put(2);
    second.put(3);

    first.swap(second);
    EXPECT_EQ(2u, second.count());
    EXPECT_EQ(1u, first.count());

    EXPECT_TRUE(second.hasElement(1));
    EXPECT_TRUE(second.hasElement(2));
    EXPECT_TRUE(first.hasElement(3));
}

TEST(HashSet, swapGlobal)
{
    capu::HashSet<uint32_t> first;
    capu::HashSet<uint32_t> second;

    first.put(1);
    first.put(2);
    second.put(3);

    using std::swap;
    swap(first, second);
    EXPECT_EQ(2u, second.count());
    EXPECT_EQ(1u, first.count());
}
