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

#ifndef CAPU_IPHONEOS_ARMV7_TIME_H
#define CAPU_IPHONEOS_ARMV7_TIME_H

#include <mach/clock.h>
#include <mach/mach.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Time
            {
            public:
                static uint64_t GetMilliseconds();
            };

            inline uint64_t Time::GetMilliseconds()
            {
                mach_timespec_t ts;
                clock_serv_t cl;
                host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cl);
                clock_get_time(cl, &ts);
                mach_port_deallocate(mach_task_self(), cl);
                return (static_cast<uint64_t>(ts.tv_sec) * 1000) + (ts.tv_nsec / 1000000);
            }
        }
    }
}
#endif // CAPU_IPHONEOS_ARMV7_TIME_H
