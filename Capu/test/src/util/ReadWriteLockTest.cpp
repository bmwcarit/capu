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
#include "capu/util/ReadWriteLock.h"
#include "capu/util/Runnable.h"
#include "capu/os/Thread.h"

class Reader: public capu::Runnable
{
public:
    bool& m_var;
    capu::ReadWriteLock& m_lock;

    Reader(bool& var, capu::ReadWriteLock& lock)
        : m_var(var)
        , m_lock(lock)
    {
    }

    inline void run()
    {
        m_lock.lockRead();
        m_var = true;
        capu::Thread::Sleep(500);
        m_var = false;
        m_lock.unlockRead();
    }
};

class Writer: public capu::Runnable
{
public:
    bool& m_var;
    capu::ReadWriteLock& m_lock;

    Writer(bool& var, capu::ReadWriteLock& lock)
        : m_var(var)
        , m_lock(lock)
    {
    }

    inline void run()
    {
        m_lock.lockWrite();
        EXPECT_FALSE(m_var); // no reader during write lock!
        m_lock.unlockWrite();
    }
};

TEST(ReadWriteLock, ReaderWriterLockTest)
{
    capu::ReadWriteLock lock;
    bool readerIsInside = false;

    Reader r1(readerIsInside, lock);
    Reader r2(readerIsInside, lock);
    Reader r3(readerIsInside, lock);
    Writer w1(readerIsInside, lock);
    Writer w2(readerIsInside, lock);

    capu::Thread t1;
    capu::Thread t2;
    capu::Thread t3;
    capu::Thread t4;
    capu::Thread t5;

    t1.start(r1);
    t2.start(r2);
    t3.start(r3);
    t4.start(w1);
    t5.start(w2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
}
