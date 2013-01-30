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
#include <gmock/gmock.h>
#include "capu/util/ThreadPool.h"
#include "capu/os/Mutex.h"
#include "capu/os/Semaphore.h"
#include "capu/os/AtomicOperation.h"

class Globals
{
public:
    static capu::uint32_t var;
};
capu::uint32_t Globals::var = 0;

class WorkToDo : public capu::Runnable
{
public:
    WorkToDo()
    {
    }

    void run()
    {
        capu::AtomicOperation::AtomicAdd32(Globals::var, 5);
        capu::Thread::Sleep(10);
    }
};

class WorkToDoCancelable : public capu::Runnable
{
public:
    capu::Semaphore& m_semaphore;

    WorkToDoCancelable(capu::Semaphore& semaphore)
        : m_semaphore(semaphore)
    {
    }

    void run()
    {
        capu::AtomicOperation::AtomicAdd32(Globals::var, 5);
        m_semaphore.release();
        while (!isCancelRequested())
        {
            capu::Thread::Sleep(10);
        }
    }
};

TEST(ThreadPool, ConstructorTest)
{
    capu::ThreadPool* pool = new capu::ThreadPool();
    EXPECT_TRUE(pool != NULL);
    EXPECT_EQ(static_cast<capu::uint32_t>(5), pool->getSize());
    delete pool;
    pool = new capu::ThreadPool(10);
    EXPECT_TRUE(pool != NULL);
    EXPECT_EQ(static_cast<capu::uint32_t>(10), pool->getSize());
    delete pool;
}

TEST(ThreadPool, TooManyThreadsTest)
{
    capu::uint32_t poolSize = 1000000;

    capu::ThreadPool* pool = new capu::ThreadPool(poolSize);
    EXPECT_TRUE(pool->getSize() < poolSize);
    EXPECT_TRUE(pool->getSize() <= capu::ThreadPool::MAX_THREAD_POOL_THREADS);

    delete pool;
}

TEST(ThreadPool, AddCloseTest)
{
    Globals::var = 0;

    capu::ThreadPool* pool = new capu::ThreadPool(capu::ThreadPool::MAX_THREAD_POOL_THREADS);
    for (capu::int32_t i = 0; i < 1000; i++)
    {
        capu::SmartPointer<WorkToDo> w = new WorkToDo();
        EXPECT_EQ(capu::CAPU_OK, pool->add(w));
    }
    EXPECT_EQ(capu::CAPU_OK, pool->close());
    //check if all the work has been done which means that all threads have been executed
    EXPECT_EQ(5000u, Globals::var);

    //make sure adding is no more supported after pool has been closed
    capu::SmartPointer<WorkToDo> w = new WorkToDo();
    EXPECT_EQ(capu::CAPU_ERROR, pool->add(w));

    EXPECT_TRUE(pool->isClosed());

    delete pool;
}

TEST(ThreadPool, AddCloseCancelTest)
{
    Globals::var = 0;
    capu::Semaphore waiter;
    capu::uint32_t poolSize = 5;
    capu::ThreadPool* pool = new capu::ThreadPool(poolSize);

    // add a lot of workers
    // a worker will increase the counter and then stay active
    for (capu::int32_t i = 0; i < 10000; i++)
    {
        capu::SmartPointer<WorkToDoCancelable> w = new WorkToDoCancelable(waiter);
        EXPECT_EQ(capu::CAPU_OK, pool->add(w));
    }

    // wait that all workers that fit in the pool have increased the global counter
    for(capu::uint32_t i = 0; i < poolSize; i++)
    {
        waiter.aquire();
    }

    // closing will cancel all workers and forbid any new worker to start
    EXPECT_EQ(capu::CAPU_OK, pool->close(true));

    // check if all the work has been done which means that all threads have been executed
    EXPECT_EQ(25u, Globals::var);

    // if test runs further closing has worked
    EXPECT_TRUE(pool->isClosed());

    delete pool;
}

TEST(ThreadPool, SizeTest)
{
    capu::ThreadPool pool(3);
    EXPECT_EQ(static_cast<capu::uint32_t>(3), pool.getSize());
    pool.close();
    EXPECT_EQ(static_cast<capu::uint32_t>(3), pool.getSize());
}

TEST(ThreadPool, CloseTest)
{
    Globals::var = 0;
    capu::ThreadPool pool(1);

    pool.add(new WorkToDo());
    pool.add(new WorkToDo());
    pool.add(new WorkToDo());

    EXPECT_EQ(capu::CAPU_OK, pool.close());

    EXPECT_EQ(capu::CAPU_ERROR, pool.add(new WorkToDo()));

    EXPECT_EQ(15u, Globals::var);

    EXPECT_TRUE(pool.isClosed());
}
