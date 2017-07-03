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
#include "capu/container/Queue.h"

TEST(Queue, Constructor_Default)
{
    //create an empty linked Queue
    capu::Queue<int32_t*>* queue = NULL;
    queue = new capu::Queue<int32_t*>();
    EXPECT_TRUE(queue != NULL);
    delete queue;
}

TEST(Queue, push_pop_Test)
{
    capu::Queue<int32_t>* queue = new capu::Queue<int32_t > ();
    int32_t data1;
    int32_t data2;
    int32_t data3;
    int32_t test = 0;
    capu::status_t result;

    data1 = 32;
    data2 = 43;
    data3 = 1211;
    //append element to the linked Queue
    result = queue->push(data1);
    EXPECT_EQ(capu::CAPU_OK, result);

    //append another element to linked Queue
    result = queue->push(data2);
    EXPECT_EQ(capu::CAPU_OK, result);

    result = queue->push(data3);
    EXPECT_EQ(capu::CAPU_OK, result);

    //Empty Queue, and see if it works correctly
    //First Element
    bool isEmpty = queue->empty();
    EXPECT_FALSE(isEmpty);
    EXPECT_EQ(capu::CAPU_OK, queue->pop(&test));
    EXPECT_EQ(test, data1);
    //Second Element
    isEmpty = queue->empty();
    EXPECT_FALSE(isEmpty);
    EXPECT_EQ(capu::CAPU_OK, queue->pop(&test));
    EXPECT_EQ(test, data2);
    //Third Element
    isEmpty = queue->empty();
    EXPECT_FALSE(isEmpty);
    EXPECT_EQ(capu::CAPU_OK, queue->pop(&test));
    EXPECT_EQ(test, data3);
    //Should be Empty
    isEmpty = queue->empty();
    EXPECT_TRUE(isEmpty);

    delete queue;
}

TEST(Queue, popWith0)
{
    capu::Queue<int32_t> queue;
    queue.push(1);
    int32_t* somenullptr = 0;
    EXPECT_EQ(capu::CAPU_OK, queue.pop(somenullptr));
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(capu::CAPU_EINVAL, queue.pop(somenullptr));
}

TEST(Queue, size)
{
    capu::Queue<int32_t>* queue = new capu::Queue<int32_t > ();
    int32_t data1;

    capu::uint_t result;

    data1 = 32;

    //size of empty Queue
    result = queue->size();
    EXPECT_EQ(0u, result);

    //add some elements and check the size for each step
    queue->push(data1);
    result = queue->size();
    EXPECT_EQ(1u, result);

    queue->push(data1);
    result = queue->size();
    EXPECT_EQ(2u, result);

    queue->push(data1);
    result = queue->size();
    EXPECT_EQ(3u, result);

    queue->pop(NULL);
    result = queue->size();
    EXPECT_EQ(2u, result);

    queue->pop();
    result = queue->size();
    EXPECT_EQ(1u, result);

    delete queue;
}

TEST(Queue, empty)
{
    capu::Queue<int32_t>* queue = new capu::Queue<int32_t > ();
    int32_t data1;

    bool result;

    data1 = 32;

    //check the empty Queue
    result = queue->empty();
    EXPECT_TRUE(result);

    //add some element
    queue->push(data1);
    result = queue->empty();
    EXPECT_FALSE(result);

    delete queue;
}

TEST(Queue, clear)
{
    capu::Queue<int32_t>* queue = new capu::Queue<int32_t > ();
    int32_t data1;
    int32_t data2;
    int32_t data3;
    capu::status_t result;

    data1 = 32;
    data2 = 43;
    data3 = 44;

    //add some dummy values to the lQueue
    queue->push(data1);
    queue->push(data1);
    queue->push(data2);
    queue->push(data3);

    //remove all elements from the linked Queue
    queue->clear();
    EXPECT_TRUE(queue->empty());

    result = queue->push(data1);
    EXPECT_EQ(capu::CAPU_OK, result);

    int32_t test = 0;
    EXPECT_EQ(capu::CAPU_OK, queue->pop(&test));
    EXPECT_EQ(data1, test);
    delete queue;
}

TEST(Queue, pop)
{
    capu::Queue<int32_t>* queue = new capu::Queue<int32_t > ();
    int32_t test;
    int32_t data1;
    int32_t data2;

    int cnt = 0;
    EXPECT_TRUE(queue->empty());
    data1 = 32;
    data2 = 43;
    queue->push(data1);
    queue->push(data2);

    while (!queue->empty())
    {
        EXPECT_EQ(capu::CAPU_OK, queue->pop(&test));
        if (cnt == 0)
        {
            EXPECT_EQ(test, data1);
        }
        else
        {
            EXPECT_EQ(test, data2);
        }
        cnt++;
    }
    delete queue;
}

TEST(Queue, swap)
{
    capu::Queue<int32_t> queue1;
    capu::Queue<int32_t> queue2;
    int32_t test;
    int32_t data1;
    int32_t data2;

    EXPECT_TRUE(queue1.empty());
    EXPECT_TRUE(queue2.empty());
    data1 = 32;
    data2 = 43;
    queue1.push(data1);
    queue2.push(data2);
    queue2.push(data2);

    EXPECT_EQ(1u, queue1.size());
    EXPECT_EQ(2u, queue2.size());

    using std::swap;
    swap(queue1, queue2);

    EXPECT_EQ(2u, queue1.size());
    EXPECT_EQ(1u, queue2.size());

    EXPECT_EQ(capu::CAPU_OK, queue2.pop(&test));
    EXPECT_EQ(test, data1);

    EXPECT_EQ(capu::CAPU_OK, queue1.pop(&test));
    EXPECT_EQ(test, data2);

    EXPECT_EQ(capu::CAPU_OK, queue1.pop(&test));
    EXPECT_EQ(test, data2);

    EXPECT_EQ(0u, queue1.size());
    EXPECT_EQ(0u, queue2.size());
}

TEST(Queue, frontBack)
{
    capu::Queue<int32_t> queue;
    queue.push(3);
    EXPECT_EQ(3, queue.front());
    EXPECT_EQ(3, queue.back());

    queue.push(4);
    EXPECT_EQ(3, queue.front());
    EXPECT_EQ(4, queue.back());

    queue.push(5);
    EXPECT_EQ(3, queue.front());
    EXPECT_EQ(5, queue.back());

    queue.pop();
    EXPECT_EQ(4, queue.front());
    EXPECT_EQ(5, queue.back());

    queue.pop();
    EXPECT_EQ(5, queue.front());
    EXPECT_EQ(5, queue.back());
}

TEST(Queue, peek)
{
    capu::Queue<int32_t> queue;
    int32_t val;
    EXPECT_EQ(capu::CAPU_EINVAL, queue.peek(val));

    queue.push(3);
    EXPECT_EQ(capu::CAPU_OK, queue.peek(val));
    EXPECT_EQ(3, val);

    EXPECT_EQ(1u, queue.size());
}

TEST(Queue, popAll)
{
    capu::List<int32_t> list;
    capu::Queue<int32_t> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);

    EXPECT_EQ(capu::CAPU_OK, queue.popAll(list));

    // test that list contains elements in correct order
    EXPECT_EQ(1, list.get(0));
    EXPECT_EQ(2, list.get(1));
    EXPECT_EQ(3, list.get(2));

    // queue must be empty now
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(0u, queue.size());

    // test with empty queue
    list.clear();
    EXPECT_EQ(capu::CAPU_OK, queue.popAll(list));
    EXPECT_TRUE(list.isEmpty());
}
