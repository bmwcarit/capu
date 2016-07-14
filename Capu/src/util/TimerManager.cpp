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

#include "capu/util/TimerManager.h"
#include "capu/util/Timer.h"
#include "capu/util/ScopedLock.h"
#include "capu/os/Time.h"


capu::shared_ptr<capu::TimerManager> capu::TimerManager::GetNewTimerManager()
{
    return shared_ptr<TimerManager>(new TimerManager());
}

capu::TimerManager::~TimerManager()
{
    //should only be called when the last shared pointer runs out of scope,
    //which means the manager is not responsible for any timers

    //stop the thread if running
    bool wakeThread = false;
    {
        ScopedLock<Mutex> mutexGuard(m_managerMutex);
        if (m_threadRunning)
        {
            //clear all executions just to be safe
            m_queuedExecutions.clear();
            wakeThread = true;
        }
    }
    if (wakeThread)
    {
        m_sleepConditionVariable.signal();
        m_waitThread.join();
    }
}

capu::TimerManager::Execution::Execution()
    : m_timer(NULL)
    , m_executionTime(0)
{

}

capu::TimerManager::Execution::Execution(const Timer& timer)
    : m_timer(&timer)
    , m_executionTime(0)
{
    m_executionTime = Time::GetMilliseconds() + timer.getTimeout();
}

const capu::Timer& capu::TimerManager::Execution::getTimer() const
{
    return *m_timer;
}

uint64_t capu::TimerManager::Execution::getExecutionTime() const
{
    return m_executionTime;
}

bool capu::TimerManager::Execution::operator<(const TimerManager::Execution& other) const
{
    return getExecutionTime() < other.getExecutionTime();
}

capu::TimerManager::TimerManager()
    : m_threadRunning(false)
{

}

void capu::TimerManager::insertExecution(const Timer& timer)
{
    Execution newExecution(timer);
    ExecutionsContainer::iterator begin = m_queuedExecutions.begin();
    ExecutionsContainer::iterator end = m_queuedExecutions.end();
    while (begin != end)
    {
        ExecutionsContainer::iterator center = begin + ((end - begin) / 2);
        if (*center < newExecution)
        {
            begin = center + 1;
        }
        else
        {
            end = center;
        }
    }
    m_queuedExecutions.insert(begin, newExecution);
}

bool capu::TimerManager::registerTimer(Timer& timer)
{
    bool bRegistered = false;
    if (!timer.maxExecutionsReached())
    {
        bool wakeThread = true;
        {
            //add the next execution of the timer to the queue
            ScopedLock<Mutex> mutexGuard(m_managerMutex);
            bRegistered = true;
            insertExecution(timer);
            if (!m_threadRunning)
            {
                m_threadRunning = true;
                wakeThread = false;
                if (m_waitThread.getState() != TS_NEW)
                {
                    m_waitThread.join();
                }
                m_waitThread.start(*this);
            }
        }
        if (wakeThread)
        {
            //wake thread to handle the modified queue
            m_sleepConditionVariable.signal();
        }
    }
    return bRegistered;
}

void capu::TimerManager::unregisterTimer(const Timer& timer)
{
    {
        //delete future executions of the timer
        ScopedLock<Mutex> mutexGuard(m_managerMutex);
        for (ExecutionsContainer::Iterator it = m_queuedExecutions.begin(); it != m_queuedExecutions.end();)
        {
            if (&it->getTimer() == &timer)
            {
                m_queuedExecutions.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    //wake thread to handle the modified queue
    m_sleepConditionVariable.signal();
}

void capu::TimerManager::run()
{
    while (!isCancelRequested())
    {
        ScopedLock<Mutex> mutexGuard(m_managerMutex);
        if (m_queuedExecutions.empty())
        {
            //no more execution queued, stop the thread
            m_threadRunning = false;
            break;
        }
        uint64_t currentTime = Time::GetMilliseconds();
        ExecutionsContainer::Iterator nextExecutionIt = m_queuedExecutions.begin();
        if (nextExecutionIt->getExecutionTime() <= currentTime)
        {
            //it is time to execute
            const Timer& timerToExecute = nextExecutionIt->getTimer();
            //replace the old execution with a new one
            m_queuedExecutions.erase(nextExecutionIt);
            if (timerToExecute.executionsRemaining() > 1)
            {
                //do not add a new execution if less then 2 executions remain
                insertExecution(timerToExecute);
            }
            timerToExecute.execute();
        }
        else
        {
            //sleep until the next execution, or until  a timer is (un)registered
            const uint64_t sleepTime = nextExecutionIt->getExecutionTime() - currentTime;
            m_sleepConditionVariable.wait(m_managerMutex, static_cast<const uint32_t>(sleepTime));
        }
    }
}
