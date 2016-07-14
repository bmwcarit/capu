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

capu::Timer::Timer(shared_ptr<TimerManager> timerManager, const Delegate<>& delegate, uint64_t timeoutMillis, uint32_t maxExecution)
    : m_delegate(delegate)
    , m_timeoutMillis(timeoutMillis)
    , m_maxExecution(maxExecution)
    , m_numExecuted(0)
    , m_registered(false)
    , m_currentTimerManager(timerManager)
{

}

capu::Timer::~Timer()
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    m_currentTimerManager->unregisterTimer(*this);
}

uint64_t capu::Timer::getTimeout() const
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    return m_timeoutMillis;
}

void capu::Timer::start()
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    if (!m_registered)
    {
        m_registered = m_currentTimerManager->registerTimer(*this);
    }
}

void capu::Timer::stop()
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    m_currentTimerManager->unregisterTimer(*this);
    m_registered = false;
}

void capu::Timer::restart()
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    stop();
    start();
}

uint32_t capu::Timer::executionsRemaining() const
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    if (m_maxExecution == 0)
    {
        return static_cast<uint32_t>(-1);
    }
    else
    {
        return m_maxExecution - m_numExecuted;
    }
}

bool capu::Timer::maxExecutionsReached() const
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    return executionsRemaining() == 0;
}

void capu::Timer::execute() const
{
    ScopedLock<Mutex> mutexGuard(m_currentTimerManager->m_managerMutex);
    if (!maxExecutionsReached())
    {
        ++m_numExecuted;
        if (!(m_delegate == Delegate<>()))
        {
            //note: the 'this' object might be deleted in the call
            m_delegate();
        }
    }
}
