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
#include "capu/os/CondVar.h"
#include "capu/os/Thread.h"

class GlobalVariables
{
public:
    static int32_t loops;
    static int32_t buffer;
    static capu::CondVar cv;
    static capu::CondVar pv;
    static capu::Mutex mutex;
};

int32_t GlobalVariables::loops = 10000;
capu::CondVar GlobalVariables::cv;
capu::CondVar GlobalVariables::pv;
capu::Mutex GlobalVariables::mutex;
int32_t GlobalVariables::buffer = 0;

class ThreadCondVarConsumer : public capu::Runnable
{
public:
    void run()
    {
        for (int32_t i = 0; i < GlobalVariables::loops; i++)
        {
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.lock());
            while (GlobalVariables::buffer == 0)
            {
                EXPECT_EQ(capu::CAPU_OK, GlobalVariables::cv.wait(GlobalVariables::mutex));
            }
            GlobalVariables::buffer--;
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::pv.signal());
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.unlock());
        }
    }
};

class ThreadCondVarProducerBroadcast : public capu::Runnable
{
public:
    void run()
    {
        for (int32_t i = 0; i < GlobalVariables::loops * 1; i++)
        {
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.lock());
            while (GlobalVariables::buffer == 100)
            {
                EXPECT_EQ(capu::CAPU_OK, GlobalVariables::pv.wait(GlobalVariables::mutex));
            }
            GlobalVariables::buffer++;
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::cv.broadcast());
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.unlock());
        }
    }
};

class ThreadCondVarProducerSignal : public capu::Runnable
{
public:
    void run()
    {
        for (int32_t i = 0; i < GlobalVariables::loops; i++)
        {
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.lock());
            while (GlobalVariables::buffer == 100)
            {
                EXPECT_EQ(capu::CAPU_OK, GlobalVariables::pv.wait(GlobalVariables::mutex));
            }
            GlobalVariables::buffer++;
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::cv.signal());
            EXPECT_EQ(capu::CAPU_OK, GlobalVariables::mutex.unlock());
        }
    }
};

TEST(CondVar, ConstructorTest)
{
    capu::CondVar* _cv = new capu::CondVar();
    EXPECT_TRUE(_cv != NULL);
    delete _cv;
}

TEST(CondVar, WaitAndBroadcastTest)
{
    ThreadCondVarConsumer consumer;
    ThreadCondVarProducerBroadcast producer;
    //create two thread
    capu::Thread* CAPU_thread = new capu::Thread();
    CAPU_thread->start(consumer);

    capu::Thread* CAPU_thread4 = new capu::Thread();
    CAPU_thread4->start(producer);

    //wait for them to finish
    CAPU_thread->join();

    CAPU_thread4->join();

    //expected value
    EXPECT_EQ(0, GlobalVariables::buffer);
    delete CAPU_thread;

    delete CAPU_thread4;
}

TEST(CondVar, WaitAndSignalTest)
{
    ThreadCondVarConsumer consumer;
    ThreadCondVarProducerSignal producer;
    //create two thread
    capu::Thread* CAPU_thread = new capu::Thread();
    CAPU_thread->start(consumer);
    capu::Thread* CAPU_thread2 = new capu::Thread();
    CAPU_thread2->start(producer);
    //wait for them to finish
    CAPU_thread->join();
    CAPU_thread2->join();
    //expected value
    EXPECT_EQ(0, GlobalVariables::buffer);
    delete CAPU_thread;
    delete CAPU_thread2;
}

TEST(CondVar, TimedWaitTestWithMutex)
{
    capu::Mutex mutex;
    capu::CondVar condvar;
    mutex.lock();
    // 100ms wait and timeout
    EXPECT_EQ(capu::CAPU_ETIMEOUT, condvar.wait(mutex, 100));
    mutex.unlock();
}

TEST(CondVar, TimedWaitTestWithLightweightMutex)
{
    capu::LightweightMutex mutex;
    capu::CondVar condvar;
    mutex.lock();
    // 100ms wait and timeout
    EXPECT_EQ(capu::CAPU_ETIMEOUT, condvar.wait(mutex, 100));
    mutex.unlock();
}

//TODO add recursive mutex condvar test
