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

#ifndef CAPU_THREAD_H
#define CAPU_THREAD_H

#include <capu/os/PlatformInclude.h>
#include "capu/Error.h"
#include "capu/util/Runnable.h"

namespace capu
{
    /**
     * Thread states
     */
    enum ThreadState
    {
        TS_NEW,
        TS_RUNNING,
        TS_TERMINATED
    };
}

#include CAPU_PLATFORM_INCLUDE(Thread)

namespace capu
{
    /**
     * Class representing a thread.
     */
    class Thread: private capu::os::arch::Thread
    {
    public:

        /**
         * Starts the thread.
         *
         * If the thread was already started it has to be joined to be
         * able to start another thread.
         * @param runnable the runnable which should be executed by the
         *                 new thread.
         * @return CAPU_OK if thread has been started successfully
         *         CAPU_ERROR otherwise
         */
        status_t start(Runnable& runnable);

        /**
         * Waits the thread completeness
         * @return CAPU_OK if thread is currently waiting for completeness
         *         CAPU_ERROR otherwise
         */
        status_t join();

        /**
         * Sets the cancel flag of the runnable
         */
        void cancel();

        /**
         * Return the current thread state
         * @return state of the thread
         */
        ThreadState getState() const;

        /**
         * Suspend the current thread for specific amount of time
         * @return CAPU_OK if thread is currently suspended
         *         CAPU_ERROR otherwise
         */
        static status_t Sleep(uint32_t millis);

        /**
         * Gets the id of the current thread.
         * @return The id of the current thread.
         */
        static uint_t CurrentThreadId();
    };

    inline
    status_t
    Thread::start(Runnable& runnable)
    {
        return capu::os::arch::Thread::start(runnable);
    }

    inline
    status_t
    Thread::join()
    {
        return capu::os::arch::Thread::join();
    }

    inline
    void
    Thread::cancel()
    {
        capu::os::arch::Thread::cancel();
    }

    inline
    ThreadState
    Thread::getState() const
    {
        return capu::os::arch::Thread::getState();
    }

    inline
    status_t
    Thread::Sleep(uint32_t millis)
    {
        return capu::os::arch::Thread::Sleep(millis);
    }

    inline
    uint_t
    Thread::CurrentThreadId()
    {
        return capu::os::arch::Thread::CurrentThreadId();
    }


}

#endif /* CAPU_THREAD_H */
