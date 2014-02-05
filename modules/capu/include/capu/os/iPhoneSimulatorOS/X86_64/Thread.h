/*
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

#ifndef CAPU_IPHONESIMULATOROS_X86_64_THREAD_H
#define CAPU_IPHONESIMULATOROS_X86_64_THREAD_H

#include <capu/os/iPhoneOS/Thread.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Thread: private capu::iphoneos::Thread
            {
            public:
                using capu::iphoneos::Thread::start;
                using capu::iphoneos::Thread::join;
                using capu::iphoneos::Thread::cancel;
                using capu::iphoneos::Thread::resetCancel;
                using capu::iphoneos::Thread::getState;
                using capu::iphoneos::Thread::Sleep;
                using capu::iphoneos::Thread::CurrentThreadId;
            };
        }
    }
}

#endif // CAPU_IPHONESIMULATOROS_X86_64_THREAD_H
