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
#include "capu/os/Thread.h"

class ThreadTest : public capu::Runnable
{
public:
    static capu::int32_t variable;

    ThreadTest(capu::int32_t val)
    {
        variable = 0;
        mVal = val;
    }

    void run()
    {
        //0.1 second delay
        EXPECT_EQ(capu::CAPU_OK, capu::Thread::Sleep(100));
        variable = mVal;
    }
private:
    capu::int32_t mVal;
};

class ThreadTest2 : public capu::Runnable
{
public:

    ThreadTest2()
    {
    }

    void run()
    {
        capu::Thread::Sleep(1000);
        return;
    }
};

class ThreadTestCancel : public capu::Runnable
{
public:
    ThreadTestCancel()
    {
    }

    void run()
    {
        while (!isCancelRequested())
        {
            capu::Thread::Sleep(10);
        }
    }
};


capu::int32_t ThreadTest::variable;

TEST(Thread, startAndJoinTest)
{
    ThreadTest _test(6);
    ThreadTest2 _test2;

    //CREATE THREAD
    capu::Thread* CAPU_thread = new capu::Thread();
    CAPU_thread->start(_test);

    //WAIT UNTIL IT FINISH
    EXPECT_EQ(capu::CAPU_OK, CAPU_thread->join());

    //CHECK THE VALUE CALCULATED IN THREAD
    EXPECT_EQ(6, ThreadTest::variable);
    delete CAPU_thread;

    capu::Thread* CAPU_thread2 = new capu::Thread();
    CAPU_thread2->start(_test2);
    EXPECT_EQ(capu::CAPU_OK, CAPU_thread2->join());
    // multiple join and join without having started is ok
    EXPECT_EQ(capu::CAPU_OK, CAPU_thread2->join());
    delete CAPU_thread2;
}

TEST(Thread, reuseThread)
{
    ThreadTest runnable1(1);
    ThreadTest runnable2(42);

    {
        capu::Thread thread;
        EXPECT_EQ(capu::TS_NEW, thread.getState());
        EXPECT_EQ(capu::CAPU_OK, thread.start(runnable1));

        // start with another runnable should fail until joined
        EXPECT_NE(capu::CAPU_OK, thread.start(runnable2));

        EXPECT_EQ(capu::CAPU_OK, thread.join());
        EXPECT_EQ(capu::TS_TERMINATED, thread.getState());
        EXPECT_EQ(1, ThreadTest::variable);
        EXPECT_EQ(capu::CAPU_OK, thread.start(runnable2));
        EXPECT_NE(capu::TS_TERMINATED, thread.getState());
    }

    // thread should be joined
    EXPECT_EQ(42, ThreadTest::variable);
}

TEST(Thread, startAndCancelTest)
{
    ThreadTestCancel runnable;

    capu::Thread* thread = new capu::Thread();
    thread->start(runnable);


    thread->cancel();

    // test blocks forever if cancel didn't work
    EXPECT_EQ(capu::CAPU_OK, thread->join());

    delete thread;
}

TEST(Thread, joinWithoutStartingIsOK)
{
    ThreadTestCancel runnable;

    capu::Thread* thread = new capu::Thread();
    // join without having started
    EXPECT_EQ(capu::CAPU_OK, thread->join());

    delete thread;
}

TEST(Thread, startAndDestructorTest)
{
    capu::int32_t newval = 6;
    ThreadTest _test(newval);

    capu::Thread* CAPU_thread = new capu::Thread();
    CAPU_thread->start(_test);
    delete CAPU_thread;

    EXPECT_EQ(newval, ThreadTest::variable);
}

TEST(Thread, sleepTest)
{
    capu::int32_t newval = 5;
    ThreadTest _test(newval);
    //CREATE THREAD
    capu::Thread* CAPU_thread = new capu::Thread();
    CAPU_thread->start(_test);
    //WAIT UNTIL IT FINISH
    EXPECT_EQ(capu::CAPU_OK, CAPU_thread->join());
    EXPECT_EQ(newval, ThreadTest::variable);
    delete CAPU_thread;
}

TEST(Thread, contextTest)
{
    capu::int32_t newval = 4;
    ThreadTest _test(newval);
    {
        capu::Thread thread;
        thread.start(_test);
    }
    EXPECT_EQ(newval, ThreadTest::variable);
}

TEST(Thread, getState)
{
    ThreadTestCancel r1;
    capu::Thread thread;
    capu::ThreadState state = thread.getState();
    EXPECT_EQ(capu::TS_NEW, state);
    thread.start(r1);
    state = thread.getState();
    EXPECT_NE(capu::TS_TERMINATED, state);
    thread.cancel();
    thread.join();
    state = thread.getState();
    EXPECT_EQ(capu::TS_TERMINATED, state);
}

class IdTester : public capu::Runnable
{
public:
    capu::uint_t m_id;

    void run()
    {
        capu::Thread::Sleep(500);
        m_id = capu::Thread::CurrentThreadId();
    }
};

TEST(Thread, getId)
{
    capu::uint_t id1 = capu::Thread::CurrentThreadId();
    IdTester idTester1;
    IdTester idTester2;
    capu::Thread t1;
    t1.start(idTester1);
    capu::Thread t2;
    t2.start(idTester2);
    t1.join();
    t2.join();
    capu::uint_t id2 = idTester1.m_id;
    capu::uint_t id3 = idTester2.m_id;
    EXPECT_NE(id1, id2);
    EXPECT_NE(id2, id3);
    EXPECT_NE(id1, id3);
}

TEST(Thread, cancelWithoutRunnable)
{
    capu::Thread thread;
    thread.cancel();

    // no test as just no crash is expected
}
