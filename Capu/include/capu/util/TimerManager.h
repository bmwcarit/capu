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

#ifndef CAPU_TIMERMANAGER_H
#define CAPU_TIMERMANAGER_H

#include "capu/container/vector.h"
#include "capu/os/Mutex.h"
#include "capu/os/CondVar.h"
#include "capu/os/Thread.h"
#include "capu/util/Runnable.h"
#include "capu/util/shared_ptr.h"

namespace capu
{
    class Timer;

    /**
     * Class responsible for calling the execute() of the capu::Timer objects.
     * Every Timer has to be initialized with a responsible TimerManager object.
     */
    class TimerManager : private Runnable
    {
        friend class Timer;
    public:
        /**
         * Constructs a new TimerManager, and returns a shared pointer on it.
         *
         * @param timerThreadName thread name to use for the internally created timer thread
         * @returns a shared pointer on a newly created TimerManager object.
         */
        static shared_ptr<TimerManager> GetNewTimerManager(const String& timerThreadName = "");

        /**
         * Destructor
         * Every timer will store a shared pointer of its TimerManager, so normally a
         * manager will only be destroyed when it is no longer responsible for any timers.
         */
        ~TimerManager();

    private:
        /**
         * Class to store the execution of the timer
         */
        class Execution
        {
        public:

            /**
             * Default constructor, only to be used by the container
             */
            Execution();

            /**
             * The constructor creates an execution in 'timeout' milliseconds
             * from now.
             * @param timer The timer for which the execution is created
             */
            Execution(const Timer& timer);

            //default copy-constructor, destructor, operator=
            //Execution(const Execution&);
            //~Execution();
            //Execution& operator=(const Execution& other);

            /**
             * Returns the timer for the execution
             */
            const Timer& getTimer() const;

            /**
             * Returns the timestamp when the execution should be performed
             */
            uint64_t getExecutionTime() const;

            /**
             * Comparison to be used by set.
             * Executions will be sorted by execution time
             */
            bool operator<(const Execution& other) const;

        private:
            const Timer* m_timer;
            uint64_t m_executionTime;
        };

        typedef vector< Execution > ExecutionsContainer;

        /**
         * Constructor
         * The timer manager can only be constructed via the static function GetNewTimerManager.
         *
         * @param timerThreadName thread name to use for the internally created timer thread
         */
        TimerManager(const String& timerThreadName);

        /**
         * Inserts an execution into the executions' container.
         * @param timer Reference of the timer for which the execution is inserted. The time of the execution
         * is set to current time + timeout of the timer.
         */
        void insertExecution(const Timer& timer);

        /**
         * Every timer has to register itself with this function so that it is executed periodically.
         * If the maximal number of executions has already been reached, the timer will not be registered.
         * @param timer Reference of the timer to register
         * @returns true if the timer was registered
         */
        bool registerTimer(Timer& timer);

        /**
         * When a timer is stopped, it has to unregister itself with this function.
         * @param timer Reference of the timer to unregister
         */
        void unregisterTimer(const Timer& timer);

        /**
         * Function to implement the timer loop. The execute() functions of the active timers will
         * be executed every 'timeoutMillis' milliseconds.
         */
        virtual void run() override;

        Thread m_waitThread;
        CondVar m_sleepConditionVariable;
        Mutex m_managerMutex;
        ExecutionsContainer m_queuedExecutions;
        bool m_threadRunning;
    };
}

#endif // CAPU_TIMERMANAGER_H
