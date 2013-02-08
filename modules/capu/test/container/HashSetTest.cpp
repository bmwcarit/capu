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
#include "capu/Error.h"
#include "capu/Config.h"

TEST(HashSet, Constructor_Default)
{
    capu::HashSet<capu::int32_t>* list = new capu::HashSet<capu::int32_t > ();
    delete list;
}

TEST(HashSet, copyConstructor)
{
    capu::HashSet<capu::int32_t> list;
    list.put(1);
    list.put(2);
    list.put(3);

    capu::HashSet<capu::int32_t> list2 = list; // copy

    list.clear();

    EXPECT_TRUE(list2.hasElement(1));
    EXPECT_TRUE(list2.hasElement(2));
    EXPECT_TRUE(list2.hasElement(3));
}

TEST(HashSet, put)
{
    capu::int32_t value2 = 10;
    capu::int32_t value = 5;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

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
    capu::int32_t value2 = 10;
    capu::int32_t value = 5;
    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

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
    capu::int32_t value = 5;
    capu::int32_t value2 = 6;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();
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
    capu::int32_t value = 5;
    capu::int32_t value2 = 6;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

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
    capu::int32_t value = 5;
    capu::int32_t value2 = 6;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

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
    capu::int32_t value = 10;
    capu::int32_t value2 = 12;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

    //create iterator
    capu::HashSet<capu::int32_t>::Iterator it = h1->begin();

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
    capu::int32_t value = 10;
    capu::int32_t value2 = 12;

    capu::HashSet<capu::int32_t>* h1 = new capu::HashSet<capu::int32_t > ();

    capu::int32_t check_value = 0;
    capu::int32_t check_value2 = 0;

    //create iterator
    capu::HashSet<capu::int32_t>::Iterator it = h1->begin();

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

#define COUNT 500000

capu::HashSet<capu::uint32_t> set;

TEST(HashSet, performancePut)
{
    //capu::HashSet<capu::uint32_t> set;
    for (capu::uint32_t i = 0; i < COUNT; i++)
    {
        set.put(i);
    }
}

TEST(HashSet, performanceIterate)
{
    capu::HashSet<capu::uint32_t>::Iterator iter = set.begin();
    while (iter != set.end())
    {
        iter++;
    }
}

TEST(HashSet, performanceRemove)
{
    // capu::HashSet<capu::uint32_t> set;
    for (capu::uint32_t i = 0; i < COUNT; i++)
    {
        set.remove(i);
    }
}




