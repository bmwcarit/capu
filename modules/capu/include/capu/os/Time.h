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

#ifndef CAPU_TIME_H
#define CAPU_TIME_H

#include "capu/Config.h"
#include <capu/os/PlatformInclude.h>
#include CAPU_PLATFORM_INCLUDE(Time)

namespace capu
{
    /**
     * Operating system time functions.
     */
    class Time
    {
    public:
        /**
         * Get the current time in milliseconds since 01.01.1970.
         */
        static uint64_t GetMilliseconds();
    };

    inline
    uint64_t
    Time::GetMilliseconds()
    {
        return capu::os::arch::Time::GetMilliseconds();
    }
}
#endif //CAPU_TIME_H

