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

#ifndef CAPU_UNIXBASED_THREAD_H
#define CAPU_UNIXBASED_THREAD_H

#include <pthread.h>
#include <unistd.h>

namespace capu
{
    namespace posix
    {
        class Thread
        {
        public:
            Thread();
            ~Thread();
            status_t start(Runnable& runnable);
            status_t join();
            void cancel();
            ThreadState getState() const;
            static status_t Sleep(uint32_t millis);

        private:

            class ThreadRunnable
            {
            public:
                ThreadRunnable();

                Thread* thread;
                Runnable* runnable;
            };

            pthread_t mThread;
            pthread_attr_t mAttr;
            ThreadState mState;
            ThreadRunnable mRunnable;

            void setState(ThreadState state);
            static void* run(void* arg);
        };

        inline
        Thread::ThreadRunnable::ThreadRunnable()
            : thread(NULL)
            , runnable(NULL)
        {
        }

        inline
        Thread::Thread()
            : mThread(0)
            , mState(TS_NEW)
        {
            pthread_attr_init(&mAttr);
            pthread_attr_setdetachstate(&mAttr, PTHREAD_CREATE_JOINABLE);
            mRunnable.thread = this;
        }

        inline
        Thread::~Thread()
        {
            join();
            pthread_attr_destroy(&mAttr);
        }

        inline
        void*
        Thread::run(void* arg)
        {
            ThreadRunnable* tr = (ThreadRunnable*) arg;
            if (tr->runnable != NULL)
            {
                tr->runnable->run();
            }
            tr->thread->setState(TS_TERMINATED);
            pthread_exit(NULL);
        }

        inline
        status_t
        Thread::start(Runnable& runnable)
        {
            if (mThread != 0)
            {
                // thread must be joined before it can be started again
                return CAPU_ERROR;
            }

            mRunnable.runnable = &runnable;
            mRunnable.thread->setState(TS_RUNNING);
            int32_t result = pthread_create(&mThread, &mAttr, Thread::run, &mRunnable);
            if (result != 0)
            {
                mRunnable.thread->setState(TS_NEW);
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        Thread::join()
        {
            if (mThread == 0)
            {
                return CAPU_ERROR;
            }
            if (pthread_join(mThread, NULL) == 0)
            {
                mThread = 0;
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }

        inline
        status_t
        Thread::Sleep(uint32_t millis)
        {
            if (usleep(millis * 1000) == 0)
            {
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }

        inline
        ThreadState Thread::getState() const
        {
            return mState;
        }

        inline
        void
        Thread::setState(ThreadState state)
        {
            mState = state;
        }

        inline
        void
        Thread::cancel()
        {
            if (mRunnable.runnable)
            {
                mRunnable.runnable->cancel();
            }
        }
    }
}
#endif // CAPU_UNIXBASED_THREAD_H
