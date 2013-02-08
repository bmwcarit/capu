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
#include "capu/os/Mutex.h"
#include "capu/os/Thread.h"

class ThreadLockTest : public capu::Runnable
{
public:
    static capu::Mutex lock;
    static capu::int32_t variable;

    ThreadLockTest(capu::int32_t sleepTime)
    {
        mSleepTime = sleepTime;
    }

    void run()
    {
        for (capu::int32_t i = 0; i < 20; i++)
        {
            lock.lock();
            capu::int32_t temp = variable;
            capu::Thread::Sleep(mSleepTime) ;
            variable = ++temp;
            lock.unlock();
        }
    }

private:
    capu::int32_t mSleepTime;
};
class ThreadNoLockTest : public capu::Runnable
{
public:
    static capu::int32_t variable2;
    static capu::Mutex lock2;

    void operator()(void* param)
    {
        for (capu::int32_t i = 0; i < 100; i++)
        {
            capu::int32_t* sleepTime = (capu::int32_t*) param;
            capu::int32_t temp = variable2;
            lock2.lock();
            capu::Thread::Sleep(*sleepTime) ;
            variable2 = ++temp;
            lock2.unlock();
        }
    }
};
capu::Mutex ThreadLockTest::lock;
capu::Mutex ThreadNoLockTest::lock2;
capu::int32_t ThreadLockTest::variable = 0;
capu::int32_t ThreadNoLockTest::variable2 = 0;

TEST(Mutex, ConstructorTest)
{
    capu::Mutex* lock = new capu::Mutex();
    EXPECT_TRUE(lock != NULL);
    delete lock;
}

TEST(Mutex, TrylockTest)
{
    capu::Mutex lock;
    //lock the mutex
    EXPECT_EQ(capu::CAPU_OK, lock.lock());
    //use trylock to lock the recursive mutex
    EXPECT_TRUE(lock.trylock());
    //release the lock twice
    EXPECT_EQ(capu::CAPU_OK, lock.unlock());
    EXPECT_EQ(capu::CAPU_OK, lock.unlock());
    //lock the mutex by using trylock
    EXPECT_TRUE(lock.trylock());
    //unlock the mutex
    EXPECT_EQ(capu::CAPU_OK, lock.unlock());
}

TEST(Mutex, lockAndUnlockTest)
{
    ThreadLockTest _testLock(5);
    ThreadLockTest _testLock2(10);
    ThreadLockTest _testLock3(15);
    ThreadLockTest _testNoLock(5);
    ThreadLockTest _testNoLock2(10);
    ThreadLockTest _testNoLock3(15);

    //EXECUTE 3 THREAD WITH A RACE CONDITION
    capu::Thread _thread;
    _thread.start(_testLock);
    capu::Thread _thread2;
    _thread2.start(_testLock2);
    capu::Thread _thread3;
    _thread3.start(_testLock3);

    _thread.join();
    _thread2.join();
    _thread3.join();
    EXPECT_EQ(60, ThreadLockTest::variable);

    //EXECUTE 3 THREAD WITH A RACE CONDITION
    capu::Thread _thread4;
    _thread4.start(_testNoLock);
    capu::Thread _thread5;
    _thread5.start(_testNoLock2);
    capu::Thread _thread6;
    _thread6.start(_testNoLock3);

    _thread4.join();
    _thread5.join();
    _thread6.join();
    EXPECT_NE(60, ThreadNoLockTest::variable2);
}
