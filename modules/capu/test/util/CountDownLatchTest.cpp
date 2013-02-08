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
#include "capu/util/CountDownLatch.h"
#include "capu/os/Thread.h"
#include "capu/util/Runnable.h"

using namespace capu;

class LatchCounter: public capu::Runnable
{
public:
    CountDownLatch& mLatch;

    LatchCounter(CountDownLatch& latch)
        : mLatch(latch)
    {
    }

    inline void run()
    {
        capu::Thread::Sleep(100);
        mLatch.countDown();
    }
};

class LatchConsumer: public capu::Runnable
{
public:
    CountDownLatch& mLatch;

    LatchConsumer(CountDownLatch& latch)
        : mLatch(latch)
    {
    }

    inline void run()
    {
        mLatch.await();
    }
};

TEST(CountDownLatch, AwaitTimeout)
{
    CountDownLatch l(1);
    EXPECT_EQ(CAPU_ETIMEOUT, l.await(100));
}

TEST(CountDownLatch, ZeroCountsInConstructor)
{
    CountDownLatch l(0);
    EXPECT_EQ(CAPU_ERROR, l.countDown());
    EXPECT_EQ(CAPU_ERROR, l.countDown());
    EXPECT_EQ(CAPU_ERROR, l.countDown());
    EXPECT_EQ(CAPU_OK, l.await());
}

TEST(CountDownLatch, TooMuchCountDowns)
{
    CountDownLatch l(1);
    EXPECT_EQ(CAPU_OK, l.countDown());
    EXPECT_EQ(CAPU_ERROR, l.countDown());
    EXPECT_EQ(CAPU_ERROR, l.countDown());
    EXPECT_EQ(CAPU_OK, l.await());
}

TEST(CountDownLatch, SingleThreaded)
{
    CountDownLatch l(3);
    l.countDown();
    l.countDown();
    l.countDown();
    EXPECT_EQ(CAPU_OK, l.await());
}

TEST(CountDownLatch, MultiThreaded)
{
    CountDownLatch l(3);

    LatchConsumer c1(l);
    LatchConsumer c2(l);
    LatchConsumer c3(l);

    LatchCounter co1(l);
    LatchCounter co2(l);
    LatchCounter co3(l);

    Thread t1;
    t1.start(c1);

    Thread t2;
    t2.start(c2);

    Thread t3;
    t3.start(c3);

    Thread t4;
    t4.start(co1);

    Thread t5;
    t5.start(co2);

    Thread t6;
    t6.start(co3);
}

