/*
 * Copyright (C) 2016 BMW Car IT GmbH
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
#include "capu/util/TimerManager.h"
#include "gmock/gmock.h"
#include "capu/util/Timer.h"
#include "capu/os/Thread.h"
#include "capu/util/ScopedLock.h"


/**
 * Class providing mocked functions.
 * The mocked function 'paramMethod' can be called by calling paramMethodCaller(). The parameter of
 * the function will be 'm_param', which can be set by the test before.
 * The pointer 'm_timer' can be initialized and then when the function deleteTimerAndCallMethod1() is
 * called, it will be destroyed and method1() is called.
 * In the constructor, the expected (minimal) numbers of calls to mutexFunc1() and mutexFunc2() can be defined.
 * When the expected number of calls is reached, m_expectationReachedConditionVariable will be signaled and
 * expectationReached() will return true.
 */
class TimerCallMock
{
public:
    /*
     * expectFunc1 and expectFunc2 are the expected (minimal) number of calls to
     * mutexFunc1() and mutexFunc2()
     */
    TimerCallMock(int expectFunc1 = 0, int expectFunc2 = 0)
        : m_param(0)
        , m_timer(NULL)
        , m_expectFunc1(expectFunc1)
        , m_expectFunc2(expectFunc2)
        , m_funcCalled1(0)
        , m_funcCalled2(0)
    {}

    MOCK_METHOD0(method1, void());
    MOCK_METHOD0(method2, void());
    MOCK_METHOD1(paramMethod, void(int param));

    void mutexFunc1()
    {
        bool signalConditionVariable = false;
        {
            capu::ScopedLock<capu::Mutex> mutexGuard(m_expectationReachedMutex);
            ++m_funcCalled1;
            //check whether the expectation has been reached with exactly this increase
            if (m_funcCalled1 == m_expectFunc1 && m_funcCalled2 >= m_expectFunc2)
            {
                signalConditionVariable = true;
            }
        }
        if (signalConditionVariable)
        {
            m_expectationReachedConditionVariable.signal();
        }
        method1();
    }
    void mutexFunc2()
    {
        bool signalConditionVariable = false;
        {
            capu::ScopedLock<capu::Mutex> mutexGuard(m_expectationReachedMutex);
            ++m_funcCalled2;
            //check whether the expectation has been reached with exactly this increase
            if (m_funcCalled1 >= m_expectFunc1 && m_funcCalled2 == m_expectFunc2)
            {
                signalConditionVariable = true;
            }
        }
        if (signalConditionVariable)
        {
            m_expectationReachedConditionVariable.signal();
        }
        method2();
    }

    //wait for the given time or until the expectation has been reached
    void waitForMutexFunc(const uint64_t& waitTime)
    {
        uint64_t currentTime = capu::Time::GetMilliseconds();
        const uint64_t waitUntil = currentTime + waitTime;
        {
            capu::ScopedLock<capu::Mutex> mutexGuard(m_expectationReachedMutex);
            //the conditions protect against spurious wakeups
            while (!expectationReached() && waitUntil > currentTime)
            {
                m_expectationReachedConditionVariable.wait(m_expectationReachedMutex, static_cast<uint32_t>(waitUntil - currentTime));
                currentTime = capu::Time::GetMilliseconds();
            }
        }
    }

    bool expectationReached() const
    {
        capu::ScopedLock<capu::Mutex> mutexGuard(m_expectationReachedMutex);
        return m_funcCalled1 >= m_expectFunc1 && m_funcCalled2 >= m_expectFunc2;
    }

    void deleteTimerAndCallMethod1()
    {
        if (m_timer)
        {
            delete m_timer;
            m_timer = NULL;
            method1();
        }
    }

    void paramMethodCaller()
    {
        paramMethod(m_param);
    }

    int                           m_param;
    capu::Timer*                  m_timer;
    const int                     m_expectFunc1;
    const int                     m_expectFunc2;
    int                           m_funcCalled1;
    int                           m_funcCalled2;
    mutable capu::Mutex           m_expectationReachedMutex;
    capu::CondVar                 m_expectationReachedConditionVariable;
};

TEST(TimerTest, NotCallingWithoutStart)
{
    TimerCallMock timerCallMock;
    EXPECT_CALL(timerCallMock, method1()).Times(0);
    //configure a oneshot timer with 10ms timeout
    capu::Timer oneShotTimer(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::method1>(timerCallMock), 5, 1);
    EXPECT_TRUE(oneShotTimer.executionsRemaining() > 0);
    //do not start the timer, wait 100ms
    //sleep is OK here, the real sleep time does not matter
    capu::Thread::Sleep(60);
    //check whether the execution counters work OK
    EXPECT_TRUE(oneShotTimer.executionsRemaining() > 0);
}

TEST(TimerTest, OneShot)
{
    TimerCallMock timerCallMock(1,1);
    EXPECT_CALL(timerCallMock, method1()).Times(1);
    EXPECT_CALL(timerCallMock, method2()).Times(1);
    //configure 2 timers as single shot timers, one with 10ms, other with 15ms timeout
    capu::Timer oneShotTimer1(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 10, 1);
    capu::Timer oneShotTimer2(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc2>(timerCallMock), 15, 1);
    //start the timers, wait 100ms
    oneShotTimer1.start();
    oneShotTimer2.start();
    timerCallMock.waitForMutexFunc(100);
    EXPECT_TRUE(timerCallMock.expectationReached());
    //wait some more time (no further calls should come)
    //sleep is OK here, the real sleep time does not matter
    capu::Thread::Sleep(50);
    //check whether the execution counters work OK
    EXPECT_TRUE(oneShotTimer1.maxExecutionsReached());
    EXPECT_TRUE(oneShotTimer1.executionsRemaining() == 0);
    EXPECT_TRUE(oneShotTimer2.maxExecutionsReached());
    EXPECT_TRUE(oneShotTimer2.executionsRemaining() == 0);
}

TEST(TimerTest, MultiShot)
{
    TimerCallMock timerCallMock(3,2);
    EXPECT_CALL(timerCallMock, method1()).Times(3);
    EXPECT_CALL(timerCallMock, method2()).Times(2);
    //configure 2 timers, one with 3 executions and 10ms timeout, other is 2 executions and 15ms timeout
    capu::Timer multiShotTimer1(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 10, 3);
    capu::Timer multiShotTimer2(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc2>(timerCallMock), 15, 2);
    //start the timers, wait 200ms
    multiShotTimer1.start();
    multiShotTimer2.start();
    timerCallMock.waitForMutexFunc(200);
    EXPECT_TRUE(timerCallMock.expectationReached());
    //check whether the execution counters work OK
    EXPECT_TRUE(multiShotTimer1.maxExecutionsReached());
    EXPECT_TRUE(multiShotTimer1.executionsRemaining() == 0);
    EXPECT_TRUE(multiShotTimer2.maxExecutionsReached());
    EXPECT_TRUE(multiShotTimer2.executionsRemaining() == 0);
}

TEST(TimerTest, Continuous)
{
    TimerCallMock timerCallMock(10,10);
    //expect a lower number of calls to make tests more stable
    EXPECT_CALL(timerCallMock, method1()).Times(::testing::AtLeast(10));
    EXPECT_CALL(timerCallMock, method2()).Times(::testing::AtLeast(10));
    //configure 2 continuously firing timers, timeouts 8ms and 20ms
    capu::Timer multiShotTimer1(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 12, 0);
    capu::Timer multiShotTimer2(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc2>(timerCallMock), 8, 0);
    //check executionsRemaining()
    EXPECT_TRUE(multiShotTimer1.executionsRemaining() == static_cast<uint32_t>(-1));
    EXPECT_TRUE(multiShotTimer2.executionsRemaining() == static_cast<uint32_t>(-1));
    //start and wait 300ms
    multiShotTimer1.start();
    multiShotTimer2.start();
    timerCallMock.waitForMutexFunc(300);
    EXPECT_TRUE(timerCallMock.expectationReached());
    EXPECT_TRUE(multiShotTimer1.executionsRemaining() == static_cast<uint32_t>(-1));
    EXPECT_TRUE(multiShotTimer2.executionsRemaining() == static_cast<uint32_t>(-1));
}

TEST(TimerTest, Stop)
{
    TimerCallMock timerCallMock;
    //there will be 2 phases, param 0 and 1
    EXPECT_CALL(timerCallMock, paramMethod(0)).Times(::testing::AnyNumber());
    EXPECT_CALL(timerCallMock, paramMethod(1)).Times(0);
    //configure a continuous timer with 10ms timeout
    capu::Timer multiShotTimer(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::paramMethodCaller>(timerCallMock), 10, 0);
    //start and wait some time (param=0)
    timerCallMock.m_param = 0;
    multiShotTimer.start();
    //sleep is OK here, the real sleep time does not matter (any calls are accepted)
    capu::Thread::Sleep(25);
    //stop and wait 200ms, in this period, no calls are allowed (param=1)
    multiShotTimer.stop();
    timerCallMock.m_param = 1;
    //sleep is OK here, the real sleep time does not matter
    capu::Thread::Sleep(50);
}

TEST(TimerTest, Restart)
{
    TimerCallMock timerCallMock(1,0);
    EXPECT_CALL(timerCallMock, method1()).Times(1);
    //configure a single shot timer with 40ms timeout
    capu::shared_ptr<capu::Timer> singleShotTimer(new capu::Timer(
        capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 40, 1));
    //restart the timer
    singleShotTimer->restart();
    //wait for the call
    timerCallMock.waitForMutexFunc(100);
    EXPECT_TRUE(timerCallMock.expectationReached());
}

TEST(TimerTest, Start)
{
    TimerCallMock timerCallMock;
    EXPECT_CALL(timerCallMock, method1()).Times(1);
    //configure a single shot timer with 40ms timeout
    capu::Timer singleShotTimer(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::method1>(timerCallMock), 40, 1);
    //keep starting the timer, the starts after the first should not have any effect, so the timer should be executed
    //sleep is OK here, the real sleep time does not matter
    singleShotTimer.start();
    capu::Thread::Sleep(20);
    singleShotTimer.start();
    capu::Thread::Sleep(20);
    singleShotTimer.start();
    capu::Thread::Sleep(20);
    singleShotTimer.start();
    capu::Thread::Sleep(20);
    EXPECT_TRUE(singleShotTimer.executionsRemaining() == 0);
}

TEST(TimerTest, StopAndStart)
{
    TimerCallMock timerCallMock(5,5);
    EXPECT_CALL(timerCallMock, method1()).Times(5);
    EXPECT_CALL(timerCallMock, method2()).Times(5);
    //configure 2 timers, each with 10 shots and 10ms timeout
    capu::Timer multiShotTimer1(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 10, 5);
    capu::Timer multiShotTimer2(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc2>(timerCallMock), 10, 5);
    //start them multiple times
    multiShotTimer1.start();
    multiShotTimer2.start();
    multiShotTimer1.start();
    multiShotTimer2.start();
    multiShotTimer1.start();
    multiShotTimer2.start();
    //wait some time
    //sleep is OK here, the real sleep time does not matter
    capu::Thread::Sleep(20);
    //stop them multiple times
    multiShotTimer1.stop();
    multiShotTimer2.stop();
    multiShotTimer1.stop();
    multiShotTimer2.stop();
    multiShotTimer1.stop();
    multiShotTimer2.stop();
    capu::Thread::Sleep(10);
    //start again, then wait for them to finish
    multiShotTimer1.start();
    multiShotTimer2.start();
    multiShotTimer1.start();
    multiShotTimer2.start();
    multiShotTimer1.start();
    multiShotTimer2.start();
    timerCallMock.waitForMutexFunc(100);
    EXPECT_TRUE(timerCallMock.expectationReached());
}

TEST(TimerTest, ExecutionsRemaining)
{
    TimerCallMock timerCallMock(1,0);
    //there will be 2 phases
    EXPECT_CALL(timerCallMock, method1()).Times(::testing::AtLeast(1));
    //configure a timer with 10 executions
    capu::Timer multiShotTimer(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::mutexFunc1>(timerCallMock), 50, 10);
    //check executionsRemaining()
    EXPECT_TRUE(multiShotTimer.executionsRemaining() == 10);
    //start the timer, wait 100ms (param=1)
    multiShotTimer.start();
    timerCallMock.waitForMutexFunc(100);
    EXPECT_TRUE(timerCallMock.expectationReached());
    multiShotTimer.stop();
    //verify executionsRemaining() decreased
    EXPECT_TRUE(multiShotTimer.executionsRemaining() < 10);
}

TEST(TimerTest, QuickExecutions)
{
    TimerCallMock timerCallMock;
    EXPECT_CALL(timerCallMock, method1()).Times(50);
    //create a timer with 50 executions and only 1ms timeout
    capu::Timer timer(capu::TimerManager::GetNewTimerManager(), capu::Delegate<>::Create<TimerCallMock, &TimerCallMock::method1>(timerCallMock), 1, 50);
    //Start the timer, run it until it has finished
    timer.start();
    while (!timer.maxExecutionsReached())
    {
        capu::Thread::Sleep(1);
    }
}
