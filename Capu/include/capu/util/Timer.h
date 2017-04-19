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

#ifndef CAPU_TIMER_H
#define CAPU_TIMER_H

#include "capu/util/Delegate.h"
#include "capu/util/shared_ptr.h"

namespace capu
{
    class TimerManager;

    /**
     * Utility class to call a delegate every 'timeoutMillis' milliseconds
     * Every Timer has to be initialized with a TimerManager to work.
     */
    class Timer
    {
        friend class TimerManager;
    public:
        /**
         * Constructor
         * @param timerManager Shared pointer on the TimerManager object which is responsible for executing the delegate
         * @param delegate The delegate to be executed when the timer expires.
         * @param timeoutMillis The timeout interval in milliseconds. Must be positive.
         * @param maxExecution If the total number of executions reaches this number, the timer will not be executed again.
         * This number cannot be changed later. Zero means the timer is always executed while it is active.
         */
        Timer(shared_ptr<TimerManager> timerManager, const Delegate<>& delegate, uint64_t timeoutMillis, uint32_t maxExecution);

        /**
         * Destructor
         * The timer will be stopped when deleted.
         */
        ~Timer();

        /**
         * Get the value of the timeout interval
         * @return The value of the timeout interval in milliseconds
         */
        uint64_t getTimeout() const;

        /**
         * Starts the timer. Has no effect if the timer is already started.
         * The delegate will be called every 'timeoutMillis' milliseconds until the timer is stopped.
         */
        void start();

        /**
         * Stops the timer.
         */
        void stop();

        /**
         * Restarts the timer, equivalent to calling stop() and then start().
         */
        void restart();

        /**
         * Returns the amount of executions that can be performed before the maximal number of executions is reached.
         * @returns The amount of executions that can be performed before the maximal number of executions is reached,
         * or 0xFFFFFFFF if unlimited executions can be performed.
         */
        uint32_t executionsRemaining() const;

        /**
         * @returns whether the maximal number of executions have been reached. Equivalent to !executionsRemaining(1)
         */
        bool maxExecutionsReached() const;

    private:
        /**
         * A timer should never be copied
         */
        Timer(const Timer&);

        /**
         * Executes the timer, in other words calls the stored delegate.
         * If the timer is running, this function will be called every 'timeoutMillis' milliseconds.
         * Calling this will count towards 'maxExecution'.
         */
        void execute() const;

        Delegate<> m_delegate;
        uint64_t m_timeoutMillis;
        uint32_t m_maxExecution;
        mutable uint32_t m_numExecuted;
        bool m_registered;
        shared_ptr<TimerManager> m_currentTimerManager;
    };
}

#endif // CAPU_TIMER_H
