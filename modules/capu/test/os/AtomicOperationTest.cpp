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
#include "capu/os/AtomicOperation.h"
#include "capu/os/Thread.h"
#include "capu/os/Mutex.h"
#include "capu/util/Runnable.h"

struct AtomicGlobals
{
    static capu::uint32_t var;
    static capu::uint32_t sum;
    static capu::uint32_t n;
    static capu::Mutex mutex;

};

capu::uint32_t AtomicGlobals::var = 1;
capu::uint32_t AtomicGlobals::sum = 0;
capu::uint32_t AtomicGlobals::n = 100000;
capu::Mutex AtomicGlobals::mutex;

class AtomicOperationIncrementThread : public capu::Runnable
{
public:
    void run()
    {
        capu::uint32_t oldValue = 0;
        for (capu::uint32_t i = 0; i < AtomicGlobals::n; i++)
        {
            oldValue += capu::AtomicOperation::AtomicInc32(AtomicGlobals::var);
        }

        AtomicGlobals::mutex.lock();
        AtomicGlobals::sum += oldValue;
        AtomicGlobals::mutex.unlock();
    }
};

class AtomicOperationDecrementThread : public capu::Runnable
{
public:
    void run()
    {
        capu::uint32_t oldValue = 0;
        for (capu::uint32_t i = 0; i < AtomicGlobals::n; i++)
        {
            oldValue += capu::AtomicOperation::AtomicDec32(AtomicGlobals::var);
        }

        AtomicGlobals::mutex.lock();
        AtomicGlobals::sum += oldValue;
        AtomicGlobals::mutex.unlock();
    }
};



//include tests depending on architecture
#include "arch/AtomicOperation.inc"

TEST(AtomicOperation, Add)
{
    capu::uint32_t val = 100;
    capu::uint32_t ret = capu::AtomicOperation::AtomicAdd32(val, 3);
    EXPECT_EQ(103u, val);
    EXPECT_EQ(100u, ret);
}

TEST(AtomicOperation, Sub)
{
    capu::uint32_t val = 13;
    capu::uint32_t ret = capu::AtomicOperation::AtomicSub32(val, 5);
    EXPECT_EQ(8u, val);
    EXPECT_EQ(13u, ret);
}

TEST(AtomicOperation, Inc)
{
    capu::uint32_t val = 1;
    capu::uint32_t ret = capu::AtomicOperation::AtomicInc32(val);
    EXPECT_EQ(2u, val);
    EXPECT_EQ(1u, ret);
}

TEST(AtomicOperation, Dec)
{
    capu::uint32_t val = 3;
    capu::uint32_t ret = capu::AtomicOperation::AtomicDec32(val);
    EXPECT_EQ(2u, val);
    EXPECT_EQ(3u, ret);
}

TEST(AtomicOperation, Atomicity)
{
    capu::Thread thread1;
    capu::Thread thread2;
    capu::Thread thread3;
    capu::Thread thread4;
    capu::Thread thread5;
    AtomicOperationIncrementThread runnable1;
    AtomicOperationIncrementThread runnable2;
    AtomicOperationIncrementThread runnable3;
    AtomicOperationIncrementThread runnable4;
    AtomicOperationIncrementThread runnable5;

    thread1.start(runnable1);
    thread2.start(runnable2);
    thread3.start(runnable3);
    thread4.start(runnable4);
    thread5.start(runnable5);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    EXPECT_EQ(AtomicGlobals::n * 5 + 1, AtomicGlobals::var);
    EXPECT_EQ(((AtomicGlobals::n * 5 * AtomicGlobals::n * 5) + AtomicGlobals::n * 5) / 2, AtomicGlobals::sum);

    AtomicOperationDecrementThread runnable6;
    AtomicOperationDecrementThread runnable7;
    AtomicOperationDecrementThread runnable8;
    AtomicOperationDecrementThread runnable9;
    AtomicOperationDecrementThread runnable10;

    AtomicGlobals::var = AtomicGlobals::n * 5;
    AtomicGlobals::sum = 0;

    thread1.start(runnable6);
    thread2.start(runnable7);
    thread3.start(runnable8);
    thread4.start(runnable9);
    thread5.start(runnable10);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    EXPECT_EQ(0, AtomicGlobals::var);
    EXPECT_EQ(((AtomicGlobals::n * 5 * AtomicGlobals::n * 5) + AtomicGlobals::n * 5) / 2, AtomicGlobals::sum);


}
