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

#ifndef CAPU_LINUX_THREAD_H
#define CAPU_LINUX_THREAD_H

#include <capu/os/Posix/Thread.h>

namespace capu
{
    namespace os
    {
        class Thread: private capu::posix::Thread
        {

        public:
            Thread(const String& name);
            using capu::posix::Thread::start;
            using capu::posix::Thread::join;
            using capu::posix::Thread::cancel;
            using capu::posix::Thread::resetCancel;
            using capu::posix::Thread::getState;
            using capu::posix::Thread::Sleep;
            using capu::posix::Thread::getName;
            static uint_t CurrentThreadId();
        };

        inline
        Thread::Thread(const String& name)
            : capu::posix::Thread(name)
        {
        }

        inline
        uint_t
        Thread::CurrentThreadId()
        {
            return pthread_self();
        }
    }
}

#endif // CAPU_LINUX_THREAD_H
