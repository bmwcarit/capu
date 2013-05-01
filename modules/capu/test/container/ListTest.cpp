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
#include "capu/container/List.h"
#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/String.h"

struct MyStruct
{
    capu::uint32_t a;

    capu::uint32_t getA()
    {
        return a;
    }
};

TEST(List, Constructor_Default)
{
    //create an empty linked list
    capu::List<capu::int32_t*>* list = NULL;
    list = new capu::List<capu::int32_t*>();
    EXPECT_TRUE(list != NULL);
    delete list;
}

TEST(List, insertTest)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::status_t result;

    //append element to the linked list
    EXPECT_EQ(capu::CAPU_OK, list->insert(data1));

    //append another element to linked list
    EXPECT_EQ(capu::CAPU_OK, list->insert(data2));

    //Get added elements to compare that if they are correctly added or not
    EXPECT_EQ(data1, list->get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    EXPECT_EQ(data2, list->get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    list->get(5, &result);
    EXPECT_EQ(capu::CAPU_EINVAL, result);

    delete list;
}

TEST(List, insertIndexTest)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;

    EXPECT_EQ(capu::CAPU_EINVAL, list->insert(10, data1));

    EXPECT_EQ(capu::CAPU_OK, list->insert(0, data2));

    EXPECT_EQ(capu::CAPU_OK, list->insert(1, data2));

    EXPECT_EQ(capu::CAPU_OK, list->insert(0, data2));

    EXPECT_EQ(capu::CAPU_OK, list->insert(1, data1));

    //Get added elements to compare that if they are correctly added or not

    EXPECT_EQ(data2, list->get(0));
    EXPECT_EQ(data1, list->get(1));

    delete list;
}

TEST(List, erase)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 56;

    //add some elements to linked list
    EXPECT_EQ(capu::CAPU_OK, list->insert(data1));

    EXPECT_EQ(capu::CAPU_OK, list->insert(data3));

    EXPECT_EQ(capu::CAPU_OK, list->insert(data2));

    //removing element at index 1
    EXPECT_EQ(capu::CAPU_OK, list->erase(1));

    //removing element at index 1
    EXPECT_EQ(capu::CAPU_OK, list->erase(1));

    //removing element at index 0 (HEAD)
    EXPECT_EQ(capu::CAPU_OK, list->erase(0, &data1));
    EXPECT_EQ(32, data1);

    //remove element from out of index
    EXPECT_EQ(capu::CAPU_EINVAL, list->erase(1000));

    //check size of list
    EXPECT_EQ(0u, list->size());

    delete list;
}

TEST(List, equalsWithString)
{
    capu::List<capu::String> list1;
    capu::List<capu::String> list2;

    EXPECT_TRUE(list1 == list2); // empty lists are equal

    list1.push_back("hello");
    list1.push_back(" world");

    EXPECT_TRUE(list1 != list2); // not the same size

    list2.push_back("hello");
    list2.push_back(" moon");

    EXPECT_TRUE(list1 != list2); // same size, but different elements

    list2.pop_back();
    list2.push_back(" world");

    EXPECT_TRUE(list1 == list2); // equal lists
}

TEST(List, equals)
{
    capu::List<capu::int32_t> list1;
    capu::List<capu::int32_t> list2;

    EXPECT_TRUE(list1 == list2); // empty lists are equal

    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);

    list2.push_back(1);
    list2.push_back(2);
    list2.push_back(3);

    EXPECT_TRUE(list1 == list2); // equal lists

    list2.pop_front();

    EXPECT_TRUE(list1 != list2); // not the same size

    list1.pop_front();
    list1.pop_front();
    list1.push_back(7);

    EXPECT_TRUE(list1 != list2); // same size, but different elements

    list1.pop_front();

    EXPECT_TRUE(list1 != list2); // not the same size

    list2.pop_front();

    EXPECT_TRUE(list1 != list2); // same size, but different elements

    list2.push_front(7);
    list1.push_back(3);

    EXPECT_TRUE(list1 == list2); // equal lists

    list1.clear();
    list2.clear();

    EXPECT_TRUE(list1 == list2); // empty lists are equal
}

TEST(List, getElementInConstList)
{
    capu::List<capu::int32_t> normalList;
    const capu::List<capu::int32_t>& constantList = normalList;
    capu::int32_t data1;

    capu::status_t result;

    data1 = 32;

    EXPECT_EQ(capu::CAPU_OK, normalList.insert(data1));

    constantList.get(0, &result);
    EXPECT_EQ(capu::CAPU_OK, result);
}

TEST(List, loopThroughConstList)
{
    capu::List<capu::int32_t> normalList;
    const capu::List<capu::int32_t>& constantList = normalList;
    capu::int32_t data1;

    capu::status_t result;

    data1 = 32;

    result = normalList.insert(data1);
    EXPECT_EQ(capu::CAPU_OK, result);

    capu::List<capu::int32_t>::Iterator iterator = constantList.begin();
    capu::int32_t temp = 0;
    while (iterator != constantList.end())
    {
        iterator++;
        temp += *iterator;
    }
}

TEST(List, get)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 56;

    capu::status_t result;

    //add some element to the linked list
    result = list->insert(data1);
    EXPECT_EQ(capu::CAPU_OK, result);

    result = list->insert(data3);
    EXPECT_EQ(capu::CAPU_OK, result);

    result = list->insert(data2);
    EXPECT_EQ(capu::CAPU_OK, result);

    //get the added elements by using its index and compare the values with inserted elements
    EXPECT_EQ(data1, list->get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    EXPECT_EQ(data3, list->get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    EXPECT_EQ(data2, list->get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    list->get(6, &result);
    EXPECT_EQ(capu::CAPU_EINVAL, result);

    delete list;
}

TEST(List, size)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;

    //size of empty list
    EXPECT_EQ(0u, list->size());

    //add some elements and check the size for each step
    list->insert(data1);
    EXPECT_EQ(1u, list->size());

    list->insert(data1);
    EXPECT_EQ(2u, list->size());

    list->insert(data1);
    EXPECT_EQ(3u, list->size());

    list->erase(0);
    EXPECT_EQ(2u, list->size());

    list->erase(1);
    EXPECT_EQ(1u, list->size());

    delete list;
}

TEST(List, empty)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;

    //check the empty list
    EXPECT_TRUE(list->isEmpty());
    EXPECT_TRUE(list->empty());

    //add some element
    list->insert(data1);
    EXPECT_FALSE(list->isEmpty());
    EXPECT_FALSE(list->empty());

    delete list;
}

TEST(List, find)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;

    //check empty list
    EXPECT_EQ(list->end(), list->find(data1));

    //add some elements
    list->insert(data1);
    list->insert(data2);
    list->insert(data3);

    //find the elements
    EXPECT_EQ(0, list->find(data1).currentIndex());

    EXPECT_EQ(1, list->find(data2).currentIndex());

    EXPECT_EQ(2, list->find(data3).currentIndex());

    delete list;
}

TEST(List, contains)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;

    capu::int32_t check_value = 0;

    //check empty list
    EXPECT_FALSE(list->contains(check_value));

    // fill the linke
    list->insert(data1);
    list->insert(data1);
    list->insert(data2);
    list->insert(data3);

    //check an elements to be contained by linked list or not
    EXPECT_FALSE(list->contains(check_value));

    EXPECT_TRUE(list->contains(data3));

    delete list;
}

TEST(List, copyConstructor1)
{
    capu::List<capu::int32_t> list;
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;

    //add some dummy values to the linked list
    list.insert(data1);
    list.insert(data2);
    list.insert(data3);

    capu::List<capu::int32_t> copy(list);

    EXPECT_EQ(list.size(), copy.size());
    list.clear();

    EXPECT_EQ(0, copy.find(data1).currentIndex());
    EXPECT_EQ(1, copy.find(data2).currentIndex());
    EXPECT_EQ(2, copy.find(data3).currentIndex());
}

TEST(List, copyConstructor2)
{
    // copy empty list and add values afterwards to original list
    capu::List<capu::int32_t> list;
    capu::List<capu::int32_t> copy(list);

    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;
    copy.insert(data1);
    copy.insert(data2);
    copy.insert(data3);

    EXPECT_EQ(0, copy.find(data1).currentIndex());
    EXPECT_EQ(1, copy.find(data2).currentIndex());
    EXPECT_EQ(2, copy.find(data3).currentIndex());
    EXPECT_TRUE(list.isEmpty());
}

TEST(List, clear)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;
    capu::status_t result;

    //add some dummy values to the linked list
    list->insert(data1);
    list->insert(data1);
    list->insert(data2);
    list->insert(data3);

    //remove all elements from the linked list
    list->clear();
    EXPECT_TRUE(list->isEmpty());

    result = list->insert(data1);
    EXPECT_EQ(capu::CAPU_OK, result);

    EXPECT_EQ(data1, list->get(0, &result));
    delete list;
}

TEST(List, set)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 56;

    EXPECT_EQ(capu::CAPU_OK, list->insert(data1));
    EXPECT_EQ(capu::CAPU_OK, list->insert(data2));
    EXPECT_EQ(capu::CAPU_OK, list->set(1, data3));
    EXPECT_EQ(capu::CAPU_OK, list->set(0, data3));
    EXPECT_EQ(data3, list->get(0));
    EXPECT_EQ(data3, list->get(1));
    EXPECT_EQ(capu::CAPU_EINVAL, list->set(2, data1));
    delete list;
}

TEST(List, assignOperator)
{
    capu::List<capu::String> l1;
    capu::List<capu::String> l2;
    l1.insert("hello");
    l1.insert("world");
    l2.insert("will");
    l2.insert("get");
    l2.insert("removed");

    EXPECT_EQ(3u, l2.size());

    l2 = l1; // assign op

    EXPECT_EQ(2u, l2.size());
    EXPECT_EQ(2u, l1.size());

    EXPECT_TRUE(l1.contains("hello"));
    EXPECT_TRUE(l1.contains("world"));

    l1.remove("hello");

    EXPECT_FALSE(l1.contains("hello"));
    EXPECT_TRUE(l1.contains("world"));

    EXPECT_TRUE(l2.contains("hello"));
    EXPECT_TRUE(l2.contains("world"));
}

TEST(List, front)
{
    capu::List<capu::uint32_t> list;

    list.push_back(1);
    EXPECT_EQ(1u, list.front());

    list.push_back(2);
    EXPECT_EQ(1u, list.front());

    list.push_back(3);
    EXPECT_EQ(1u, list.front());

    list.push_back(4);
    EXPECT_EQ(1u, list.front());

    list.push_back(5);
    EXPECT_EQ(1u, list.front());
}

TEST(List, back)
{
    capu::List<capu::uint32_t> list;

    list.push_back(1);
    EXPECT_EQ(1u, list.back());
    EXPECT_EQ(1u, list.front());

    list.push_back(2);
    EXPECT_EQ(2u, list.back());
    EXPECT_EQ(1u, list.front());

    list.push_back(3);
    EXPECT_EQ(3u, list.back());
    EXPECT_EQ(1u, list.front());

    list.push_back(4);
    EXPECT_EQ(4u, list.back());
    EXPECT_EQ(1u, list.front());

    list.push_back(5);
    EXPECT_EQ(5u, list.back());
    EXPECT_EQ(1u, list.front());
}

TEST(List, StaticMemory)
{
    capu::StaticList<capu::uint32_t, 3u> list;
	EXPECT_EQ(capu::CAPU_OK, list.push_front(1));
    EXPECT_EQ(capu::CAPU_OK, list.push_front(2));
    EXPECT_EQ(capu::CAPU_OK, list.push_front(3));
    EXPECT_EQ(capu::CAPU_ENO_MEMORY, list.push_front(4));
    EXPECT_EQ(3, list.size());

    list.clear();
    EXPECT_EQ(0, list.size());

    EXPECT_EQ(capu::CAPU_OK, list.push_front(1));
    EXPECT_EQ(capu::CAPU_OK, list.push_front(2));
    EXPECT_EQ(capu::CAPU_OK, list.push_front(3));
    EXPECT_EQ(capu::CAPU_ENO_MEMORY, list.push_front(4));
    EXPECT_EQ(3, list.size());
}

TEST(ListIterator, hasNext)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::List<capu::int32_t>::Iterator it = list->begin();
    EXPECT_EQ(list->end(), it);

    list->insert(data1);
    list->insert(data2);

    it = list->begin();
    EXPECT_NE(list->end(), it);
    delete list;
}

TEST(ListIterator, next)
{
    capu::List<capu::int32_t>* list = new capu::List<capu::int32_t > ();
    capu::int32_t data1;
    capu::int32_t data2;

    capu::List<capu::int32_t>::Iterator it = list->begin();
    capu::int32_t cnt = 0;
    EXPECT_EQ(list->end(), it);
    data1 = 32;
    data2 = 43;
    list->insert(data1);
    list->insert(data2);

    it = list->begin();

    EXPECT_NE(list->end(), it);
    it++;
    EXPECT_NE(list->end(), it);
    it++;
    EXPECT_EQ(list->end(), it);

    it = list->begin();

    while (it != list->end())
    {
        if (cnt == 0)
        {
            EXPECT_EQ(*it, data1);
        }
        else
        {
            EXPECT_EQ(*it, data2);
        }
        cnt++;
        it++;
    }
    delete list;
}

TEST(ListIterator, beginEnd)
{
    const capu::List<MyStruct> constList;
    capu::List<MyStruct> list;

    const capu::List<MyStruct>::Iterator constListIterBegin = list.begin();
    capu::List<MyStruct>::Iterator listIterBegin = list.begin();
    const capu::List<MyStruct>::Iterator constListIterEnd = list.end();
    capu::List<MyStruct>::Iterator listIterEnd = list.end();

    EXPECT_EQ(listIterBegin, listIterEnd);
    EXPECT_EQ(constListIterBegin, constListIterEnd);
}

TEST(ListIterator, IteratorMethodCall)
{
    MyStruct mystruct;
    mystruct.a = 5;

    capu::List<MyStruct> list;
    list.insert(mystruct);

    capu::List<MyStruct>::Iterator listIter = list.begin();

    EXPECT_EQ(5u, listIter->getA());
}

TEST(ListIterator, IteratorPtrMethodCall)
{
    MyStruct mystruct;
    mystruct.a = 5;

    capu::List<MyStruct*> list;
    list.insert(&mystruct);

    capu::List<MyStruct*>::Iterator listIter = list.begin();

    EXPECT_EQ(5u, (*listIter)->getA());
}

TEST(ListIterator, current)
{
    capu::List<capu::int32_t> list;
    capu::List<capu::int32_t>::Iterator it = list.begin();

    list.insert(1);
    it = list.begin();
    EXPECT_EQ(1, *it);
    list.insert(2);
    list.insert(3);

    capu::int32_t index = 1;
    for (it = list.begin(); it != list.end(); it++)
    {
        EXPECT_EQ(*it, index);
        ++index;
    }
}

TEST(ListIterator, erase)
{
    capu::List<capu::int32_t> list;
    capu::int32_t data1 = 0;
    capu::List<capu::int32_t>::Iterator it = list.begin();

    EXPECT_EQ(capu::CAPU_EINVAL, list.erase(it));
    list.insert(1);
    it = list.begin();
    EXPECT_EQ(0, it.currentIndex());
    EXPECT_EQ(capu::CAPU_OK, list.erase(it, &data1));
    EXPECT_EQ(list.end(), it);
    EXPECT_EQ(0u, list.size());
    EXPECT_EQ(1, data1);

    list.insert(1);
    list.insert(2);
    list.insert(3);

    it = list.begin();
    EXPECT_EQ(0, it.currentIndex());
    it++;
    EXPECT_EQ(1, it.currentIndex());
    list.erase(it, &data1);
    EXPECT_EQ(2, data1);
    EXPECT_EQ(0, it.currentIndex());

    capu::int32_t index = 1;

    for (it = list.begin(); it != list.end(); it++)
    {
        EXPECT_EQ(*it, index);
        index += 2;
    }

    list.insert(1, 2);

    index = 1;

    it = list.begin();
    EXPECT_EQ(0, it.currentIndex()); // always pointing to first element of list

    while (it != list.end())
    {
        list.erase(it, &data1);
        EXPECT_EQ(data1, index);
        ++index;
    }

    EXPECT_EQ(0, list.size());
}

TEST(ListIterator, push_back)
{
    capu::List<capu::int32_t> list;
    capu::status_t result = 0;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(1, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
}

TEST(ListIterator, push_front)
{
    capu::List<capu::int32_t> list;
    capu::status_t result = 0;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    EXPECT_EQ(3, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(1, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(1, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
}

TEST(ListIterator, pop_front)
{
    capu::List<capu::int32_t> list;

    EXPECT_EQ(capu::CAPU_EINVAL, list.pop_front());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(capu::CAPU_OK, list.pop_front());
    EXPECT_EQ(2u, list.size());
    EXPECT_EQ(2, list.get(0));
    EXPECT_EQ(3, list.get(1));

    EXPECT_EQ(capu::CAPU_OK, list.pop_front());
    EXPECT_EQ(1u, list.size());
    EXPECT_EQ(3, list.get(0));

    EXPECT_EQ(capu::CAPU_OK, list.pop_front());
    EXPECT_EQ(0u, list.size());

    EXPECT_EQ(capu::CAPU_EINVAL, list.pop_front());
}

TEST(ListIterator, pop_back)
{
    capu::List<capu::int32_t> list;

    EXPECT_EQ(capu::CAPU_EINVAL, list.pop_back());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(capu::CAPU_OK, list.pop_back());
    EXPECT_EQ(2u, list.size());
    EXPECT_EQ(1, list.get(0));
    EXPECT_EQ(2, list.get(1));

    EXPECT_EQ(capu::CAPU_OK, list.pop_back());
    EXPECT_EQ(1u, list.size());
    EXPECT_EQ(1, list.get(0));

    EXPECT_EQ(capu::CAPU_OK, list.pop_back());
    EXPECT_EQ(0u, list.size());

    EXPECT_EQ(capu::CAPU_EINVAL, list.pop_back());
}

TEST(ListIterator, insert)
{
    capu::List<capu::int32_t> list;
    capu::List<capu::int32_t>::Iterator iter = list.begin();
    capu::status_t result = 0;

    list.insert(iter, 1);
    EXPECT_EQ(0, iter.currentIndex());
    list.insert(iter, 2);
    EXPECT_EQ(1, iter.currentIndex());
    list.insert(iter, 3);
    EXPECT_EQ(2, iter.currentIndex());

    EXPECT_EQ(1, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    iter = list.begin();
    iter++;
    EXPECT_EQ(1, iter.currentIndex());
    list.insert(iter, 4); // inserts before 2
    EXPECT_EQ(2, iter.currentIndex());
    EXPECT_EQ(2, *iter); // iter still points to 2

    EXPECT_EQ(1, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(4, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(3, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    iter++;
    EXPECT_EQ(3, iter.currentIndex());
    EXPECT_EQ(3, *iter);
    iter++;
    list.insert(iter, 5);
    EXPECT_EQ(1, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(4, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(3, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(5, list.get(4, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    iter = list.begin();
    list.insert(iter, 6);
    EXPECT_EQ(6, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(1, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(4, list.get(2, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(3, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(3, list.get(4, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(5, list.get(5, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
}

TEST(ListIterator, currentIndex)
{
    capu::List<capu::int32_t> list;
    capu::int32_t data1 = 32;
    capu::int32_t data2 = 43;
    capu::int32_t data3 = 44;

    //insert some dummy values to the linked list
    list.insert(data1);
    list.insert(data2);
    list.insert(data3);

    capu::List<capu::int32_t>::Iterator iter = list.begin();

    EXPECT_EQ(0u, iter.currentIndex());
    iter++;
    EXPECT_EQ(1u, iter.currentIndex());
    iter++;
    EXPECT_EQ(2u, iter.currentIndex());
}

TEST(ListIterator, loopInserting)
{
    capu::List<capu::int32_t> list;
    capu::List<capu::int32_t>::Iterator iter = list.begin();
    capu::status_t result = 0;

    list.insert(iter, 1);

    iter = list.begin();
    while (iter != list.end())
    {
        iter++; // point to last element
    }

    list.insert(iter, 2);

    EXPECT_EQ(1, list.get(0, &result));
    EXPECT_EQ(capu::CAPU_OK, result);
    EXPECT_EQ(2, list.get(1, &result));
    EXPECT_EQ(capu::CAPU_OK, result);

    iter++;
    iter++;
    iter++;
    list.insert(iter, 3);
    iter = list.begin();
    capu::int32_t i = 1;
    while (i <= 3)
    {
        EXPECT_EQ(i - 1, iter.currentIndex());
        EXPECT_EQ(i, *iter);
        iter++;
        i++;
    }
}
