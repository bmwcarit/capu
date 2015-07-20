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

#ifndef CAPU_TIMESTAMP_H
#define CAPU_TIMESTAMP_H

#include "capu/Config.h"
#include "capu/Error.h"
#include "capu/os/PlatformInclude.h"
#include "capu/container/String.h"

#include CAPU_PLATFORM_INCLUDE(TimeStamp)

namespace capu
{

    /*Date and Time constructs platform abstractions*/
    class TimeStamp : private os::arch::TimeStamp
    {
    public:

        /**
         * Default Constructor
         *
         * Initializes the timestamp with current date and time
         */
        TimeStamp();

        /**
         * Destructor
         */
        ~TimeStamp();

        /**
         * Returns current timestamp
         * @return current timestamp
         */
        static TimeStamp GetCurrentDateAndTime();

        /**
         *
         * @param result    string version of standard representation for the timestamp
         * @return CAPU_OK  if the timestamp is correctly converted to the string
         *          CAPU_ERROR      otherwise
         */
        status_t getFormattedString(String& result);

        /**
         *
         * @param pattern  String version of pattern which will be used to format the timestamp
         *                  The pattern has to contain the fields and one character delimiter.
         *                  The delimiter can not be in character range of a-z or A-Z
         *                  The valid fields used for formatting is listed below.
         *
         *                  Day             day
         *                  Date            dd
         *                  Year            yyyy
         *                  Months          mon
         *                  Seconds         ss
         *                  Minutes         mm
         *                  Hours           hh
         *
         *                  Example valid formats can be defined as
         *                  "dd mon yyyy"
         *                  "dd_mon_yyyy"
         *                  "day dd_mon_yyyy"
         *                  "day_dd_mon_yyyy"
         *
         * @param result    String version of standard representation for the timestamp
         * @return CAPU_OK  If the timestamp is correctly converted to the string
         *          CAPU_EINVAL     if the pattern is illegal
         *          CAPU_ERROR      otherwise
         */
        status_t getFormattedString(const String& pattern, String& result);

        /**
         * Operator overloading for operator <
         * @param timestamp
         * @return true if the this < timestamp
         *        false otherwise
         */
        bool operator<(const TimeStamp & timestamp) const;

        /**
         * Operator overloading for operator ==
         * @param timestamp
         * @return true if the this == timestamp
         *        false otherwise
         */
        bool operator==(const TimeStamp & timestamp) const;

        /**
         * Operator overloading for operator >
         * @param timestamp
         * @return true if the this > timestamp
         *        false otherwise
         */
        bool operator>(const TimeStamp & timestamp) const;


    };

    status_t TimeStamp::getFormattedString(String& result)
    {
        return capu::os::arch::TimeStamp::getFormattedString(result);
    }

    status_t TimeStamp::getFormattedString(const String& pattern, String& result)
    {
        return capu::os::arch::TimeStamp::getFormattedString(pattern, result);
    }

    inline bool TimeStamp::operator<(const TimeStamp & timestamp) const
    {
        return capu::os::arch::TimeStamp::operator<(timestamp);
    }

    inline bool TimeStamp::operator>(const TimeStamp & timestamp) const
    {
        return capu::os::arch::TimeStamp::operator>(timestamp);
    }

    inline bool TimeStamp::operator==(const TimeStamp & timestamp) const
    {
        return capu::os::arch::TimeStamp::operator==(timestamp);
    }

    inline TimeStamp TimeStamp::GetCurrentDateAndTime()
    {
        //TODO:
        //I did not liked it. Improve it on later iterations
        TimeStamp t;
        return t;
    }

    inline TimeStamp::TimeStamp()
    {

    }

    inline TimeStamp::~TimeStamp()
    {

    }


}
#endif	/* DATETIME_H */

