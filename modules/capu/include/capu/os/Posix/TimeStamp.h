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

#ifndef CAPU_UNIXBASED_TIMESTAMP_H
#define CAPU_UNIXBASED_TIMESTAMP_H
#include <time.h>
#include "capu/os/Generic/TimeStamp.h"

namespace capu
{
    namespace posix
    {
        /**
         * Platform abstract Timestamps
         */
        class TimeStamp : public capu::generic::TimeStamp
        {
        public:
            status_t getFormattedString(String& result);
            status_t getFormattedString(const String& pattern, String& result);
        };


        inline status_t TimeStamp::getFormattedString(String& result)
        {
            struct tm *timeInfo;
            timeInfo = localtime(&mTime);
            //convert it to standard ascii representation
            char * asciiTimeInfo = asctime(timeInfo);
            if (asciiTimeInfo == NULL) {
                //in case conversion fails
                return CAPU_ERROR;
            }
            result.append(asciiTimeInfo);
            return CAPU_OK;
        }

        inline status_t TimeStamp::getFormattedString(const String& pattern, String& result)
        {
            if (pattern.getLength() == 0) {
                //empty pattern
                result = "";
                return CAPU_EINVAL;
            }

            struct tm *timeInfo;
            timeInfo = localtime(&mTime);
            //convert it to standard ascii representation
            char_t * asciiTimeInfo = asctime(timeInfo);
            if (asciiTimeInfo == NULL) {
                //in case conversion fails
                return CAPU_ERROR;
            }
            String tmp = asciiTimeInfo;
            return formatString(result, tmp, pattern);
        }
    }
}

#endif // CAPU_UNIXBASED_TIME_H

