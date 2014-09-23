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

#ifndef CAPU_UNIXBASED_TIME_H
#define CAPU_UNIXBASED_TIME_H

#include <time.h>

namespace capu
{
    namespace posix
    {
        class Time
        {
        public:
            static uint64_t GetMilliseconds();
        };

        inline uint64_t Time::GetMilliseconds()
        {
            struct timespec currentTime;
            if (clock_gettime(CLOCK_REALTIME, &currentTime) != 0)
            {
                // handle error somehow?
                return 0;
            }
            uint64_t millisecs = (static_cast<uint64_t>(currentTime.tv_sec) * 1000) + (currentTime.tv_nsec / 1000000);
            return millisecs;
        }
    }
}

#endif // CAPU_UNIXBASED_TIME_H

