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

#include "capu/os/Time.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace capu
{
    namespace os
    {
        uint64_t Time::GetMilliseconds()
        {
            FILETIME now;
            GetSystemTimeAsFileTime(&now);
            uint64_t convertedTime = (((ULONGLONG)now.dwHighDateTime) << 32) + now.dwLowDateTime;
            return (convertedTime - 116444736000000000LL) / 10000; // convertedTime is since 1601.., but we want since 1970, so we must remove this amount
        }

        uint64_t Time::GetMicroseconds()
        {
            const uint_t microsecondsPerSecond = 1000000;
            LARGE_INTEGER frequency;
            LARGE_INTEGER t;

            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&t);

            return (ULONGLONG)t.QuadPart * microsecondsPerSecond / frequency.QuadPart;
        }
    }
}
