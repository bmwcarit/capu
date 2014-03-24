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

#ifndef CAPU_WINDOWS_TIME_H
#define CAPU_WINDOWS_TIME_H

#define _WINSOCKAPI_
#include <windows.h>
#include "capu/Config.h"

namespace capu
{
    namespace os
    {
        class Time
        {
        public:
            static uint64_t GetMilliseconds();
        };

        inline
        uint64_t
        Time::GetMilliseconds()
        {
            FILETIME now;
            GetSystemTimeAsFileTime(&now);
            uint64_t convertedTime = (((ULONGLONG) now.dwHighDateTime) << 32) + now.dwLowDateTime;
            return (convertedTime - 116444736000000000LL) / 10000; // convertedTime is since 1601.., but we want since 1970, so we must remove this amount
        }
    }
}
#endif //CAPU_WINDOWS_TIME_H
