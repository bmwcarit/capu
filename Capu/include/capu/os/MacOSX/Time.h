/*
 * Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_MACOSX_TIME_H
#define CAPU_MACOSX_TIME_H

#include <mach/clock.h>
#include <mach/mach.h>

namespace capu
{
    namespace os
    {
        class Time
        {
        public:
            static uint64_t GetMilliseconds();
            static uint64_t GetMicroseconds();
        private:
            static void getTimeInternal(mach_timespec_t& ts);
            static uint64_t getMultipliedTime(const uint_t& secondsFactor, const uint_t& nanosecondsFactor);
        };

        inline uint64_t Time::GetMilliseconds()
        {
            const uint_t millisecondsPerSecond = 1000;
            const uint_t nanosecondsPerMillisecond = 1000000;
            return getMultipliedTime(millisecondsPerSecond, nanosecondsPerMillisecond);
        }

        inline uint64_t Time::GetMicroseconds()
        {
            const uint_t microsecondsPerSecond = 1000000;
            const uint_t nanosecondsPerMicrosecond = 1000;
            return getMultipliedTime(microsecondsPerSecond, nanosecondsPerMicrosecond);
        }

        inline void Time::getTimeInternal(mach_timespec_t& ts)
        {
            clock_serv_t cl;
            host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cl);
            clock_get_time(cl, &ts);
            mach_port_deallocate(mach_task_self(), cl);
        }

        inline uint64_t Time::getMultipliedTime(const uint_t& secondsFactor, const uint_t& nanosecondsFactor)
        {
            mach_timespec_t ts;
            getTimeInternal(ts);
            return (static_cast<uint64_t>(ts.tv_sec) * secondsFactor) + (ts.tv_nsec / nanosecondsFactor);
        }
    }
}
#endif // CAPU_MACOSX_TIME_H
