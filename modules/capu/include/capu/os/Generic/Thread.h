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

#ifndef CAPU_GENERIC_THREAD_H
#define CAPU_GENERIC_THREAD_H

#include "capu/util/Runnable.h"
#include "capu/os/ThreadState.h"

namespace capu
{
    namespace generic
    {

        class Thread;
        class ThreadRunnable
        {
        public:
            ThreadRunnable();

            Thread* thread;
            Runnable* runnable;
        };

        class Thread
        {
        public:
            Thread();
            void cancel();
            void resetCancel();
            ThreadState getState() const;
            void setState(ThreadState state);

        protected:
            ThreadState mState;
            ThreadRunnable mRunnable;
            bool mIsStarted;
        };


        inline
        ThreadRunnable::ThreadRunnable()
            : thread(NULL)
            , runnable(NULL)
        {
        }

        inline
        Thread::Thread()
            : mState(TS_NEW)
            , mIsStarted(false)
        {
            mRunnable.thread = this;
        }

        inline
        ThreadState
        Thread::getState() const
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

        inline
        void
        Thread::resetCancel()
        {
            if (mRunnable.runnable)
            {
                mRunnable.runnable->resetCancel();
            }
        }
    }
}

#endif //CAPU_GENERIC_THREAD_H
