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
#include <cstdlib>

#include "capu/container/BlockingQueue.h"
#include "capu/util/ThreadPool.h"
#include "capu/os/AtomicOperation.h"

using namespace std;

class Producer: public capu::Runnable
{
public:
    capu::BlockingQueue<capu::uint32_t>& mQueue;
    capu::int32_t mCount;
    capu::uint32_t mId;

    static capu::uint32_t mSum;

    Producer(capu::BlockingQueue<capu::uint32_t>& queue, capu::int32_t count, capu::uint32_t id)
        : mQueue(queue), mCount(count), mId(id)
    {
    }

    inline void run()
    {
        for (capu::int32_t i = 0; i < mCount; ++i)
        {
            capu::uint32_t val = i + mId * 100 + 100;
            mQueue.push(val);
            capu::uint32_t sleepTime = rand() % 100;
            capu::Thread::Sleep(sleepTime);
            capu::AtomicOperation::AtomicAdd32(mSum, val);
        }
    }
};

capu::uint32_t Producer::mSum = 0;

class Consumer: public capu::Runnable
{
public:
    capu::BlockingQueue<capu::uint32_t>& mQueue;
    capu::int32_t mCount;
    capu::uint32_t mId;

    static capu::uint32_t mSum;

    Consumer(capu::BlockingQueue<capu::uint32_t>& queue, capu::int32_t count, capu::uint32_t id)
        : mQueue(queue), mCount(count), mId(id)
    {
    }

    inline void run()
    {
        for (capu::int32_t i = 0; i < mCount; ++i)
        {
            capu::uint32_t tmp = 0;
            EXPECT_EQ(capu::CAPU_OK, mQueue.pop(&tmp));
            capu::AtomicOperation::AtomicAdd32(mSum, tmp);
        }
    }
};
capu::uint32_t Consumer::mSum = 0;

TEST(BlockingQueue, PushPopSingleThread)
{
    capu::BlockingQueue<capu::int32_t> queue;
    queue.push(3);
    capu::int32_t val = 0;
    EXPECT_EQ(capu::CAPU_OK, queue.pop(&val));
    EXPECT_EQ(3, val);
}

TEST(BlockingQueue, TestPopTimeout)
{
    capu::BlockingQueue<capu::int32_t> queue;
    capu::int32_t val;
    EXPECT_EQ(capu::CAPU_ETIMEOUT, queue.pop(&val, 10));
}

TEST(BlockingQueue, TestEmpty)
{
    capu::BlockingQueue<capu::int32_t> queue;
    EXPECT_TRUE(queue.empty());
    queue.push(3);
    EXPECT_FALSE(queue.empty());
    capu::int32_t val;
    queue.pop(&val);
    EXPECT_TRUE(queue.empty());
}

TEST(BlockingQueue, TestPeek)
{
    capu::int32_t val;
    capu::BlockingQueue<capu::int32_t> queue;
    EXPECT_EQ(capu::CAPU_EINVAL, queue.peek(val));
    queue.push(3);
    EXPECT_EQ(capu::CAPU_OK, queue.peek(val));
    EXPECT_EQ(3, val);
    queue.pop(&val);
    EXPECT_EQ(capu::CAPU_EINVAL, queue.peek(val));
}

TEST(BlockingQueue, PushPopMultiThread)
{
    capu::BlockingQueue<capu::uint32_t> queue;

    // countProducer * countPerProducer % countConsumer == 0 must be ensured!!!
    capu::int32_t countProducer = 20;
    capu::int32_t countConsumer = 100;
    capu::int32_t countPerProducer = 10;
    capu::int32_t count = countProducer * countPerProducer;
    capu::int32_t countPerConsumer = count / countConsumer;

    // this ensures correct test configuration
    ASSERT_EQ(0, countProducer * countPerProducer % countConsumer);

    capu::ThreadPool consumer(countConsumer);

    for (capu::int32_t i = 0; i < countConsumer; ++i)
    {
        consumer.add(new Consumer(queue, countPerConsumer, i + 20));
    }

    capu::ThreadPool producer(countProducer);
    for (capu::int32_t i = 0; i < countProducer; ++i)
    {
        producer.add(new Producer(queue, countPerProducer, i + 10));
    }

    producer.close();
    consumer.close();

    EXPECT_EQ(Producer::mSum, Consumer::mSum);
}

TEST(BlockingQueue, StaticMemory)
{
    capu::StaticBlockingQueue<capu::uint32_t, 3u> queue;
    EXPECT_EQ(capu::CAPU_OK, queue.push(1));
    EXPECT_EQ(capu::CAPU_OK, queue.push(2));
    EXPECT_EQ(capu::CAPU_OK, queue.push(3));
    EXPECT_EQ(capu::CAPU_ENO_MEMORY, queue.push(4));
    EXPECT_EQ(3, queue.size());

    queue.clear();
    EXPECT_EQ(0, queue.size());

    EXPECT_EQ(capu::CAPU_OK, queue.push(1));
    EXPECT_EQ(capu::CAPU_OK, queue.push(2));
    EXPECT_EQ(capu::CAPU_OK, queue.push(3));
    EXPECT_EQ(capu::CAPU_ENO_MEMORY, queue.push(4));
    EXPECT_EQ(3, queue.size());
}
