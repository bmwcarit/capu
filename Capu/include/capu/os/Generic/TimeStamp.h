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

#ifndef CAPU_GENERIC_TIMESTAMP_H
#define CAPU_GENERIC_TIMESTAMP_H
#include <time.h>
#include "capu/container/String.h"

namespace capu
{
    namespace generic
    {

        /**
         * Platform abstraction for timestamps
         */
        class TimeStamp
        {
        protected:
            time_t mTime;
            static const capu::int32_t START_DAY = 0;
            static const capu::int32_t LENGTH_DAY = 3;
            static const capu::int32_t START_MONTH = 4;
            static const capu::int32_t LENGTH_MONTH = 3;
            static const capu::int32_t START_DATE = 8;
            static const capu::int32_t LENGTH_DATE = 2;
            static const capu::int32_t START_HOUR = 11;
            static const capu::int32_t LENGTH_HOUR = 2;
            static const capu::int32_t START_MINUTE = 14;
            static const capu::int32_t LENGTH_MINUTE = 2;
            static const capu::int32_t START_SECONDS = 17;
            static const capu::int32_t LENGTH_SECONDS = 2;
            static const capu::int32_t START_YEAR = 20;
            static const capu::int32_t LENGTH_YEAR = 4;
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
             * Operator overloading for operator smaller than
             * @param timestamp
             * @return true if the "this" smaller than timestamp
             *          false otherwise
             */
            bool operator<(const TimeStamp & timestamp) const;

            /**
             * Operator overloading for operator equal to
             * @param timestamp
             * @return true if the "this" is equal tp timestamp
             *          false otherwise
             */
            bool operator==(const TimeStamp & timestamp) const;

            /**
             * Operator overloading for operator greater than
             * @param timestamp
             * @return true if the "this" is greater than timestamp
             *          false otherwise
             */
            bool operator>(const TimeStamp & timestamp) const;

        protected:
            status_t formatString(String& result, const String& tmp, const String& pattern);
        };

        inline bool TimeStamp::operator<(const TimeStamp & timestamp) const
        {
            if (difftime(timestamp.mTime, this->mTime) > 0)
                return true;
            return false;
        }

        inline bool TimeStamp::operator>(const TimeStamp & timestamp) const
        {
            if (difftime(this->mTime, timestamp.mTime) < 0)
                return true;
            return false;
        }

        inline bool TimeStamp::operator==(const TimeStamp & timestamp) const
        {
            if (difftime(this->mTime, timestamp.mTime) == 0)
                return true;
            return false;
        }

        inline TimeStamp::TimeStamp()
        {
            time(&mTime);
        }

        inline TimeStamp::~TimeStamp()
        {

        }

        inline
        status_t TimeStamp::formatString(String& result, const String& tmp, const String& pattern)
        {
            //parse week day as string
            String day = tmp.substr(START_DAY, LENGTH_DAY);
            //parse month as string
            String month = tmp.substr(START_MONTH, LENGTH_MONTH);
            //parse date as string
            String date = tmp.substr(START_DATE, LENGTH_DATE);
            //parse hours as string
            String hour = tmp.substr(START_HOUR, LENGTH_HOUR);
            //parse minutes as string
            String minute = tmp.substr(START_MINUTE, LENGTH_MINUTE);
            //parse seconds as string
            String seconds = tmp.substr(START_SECONDS, LENGTH_SECONDS);
            //parse seconds as string
            String year = tmp.substr(START_YEAR, LENGTH_YEAR);

            bool parsed = false;
            result = "";
            for (uint32_t i = 0; i < pattern.getLength(); ++i) {

                if (parsed) {
                    //check if the deliminator is character, it should not be allowed
                    if (((pattern[i] >= 'a') && (pattern[i] <= 'z')) || ((pattern[i] >= 'A') && (pattern[i] <= 'Z'))) {
                        result = "";
                        return CAPU_EINVAL;
                    }
                    //if it is acceptable deliminator then append it
                    /*APPEND FUNCTION FOR STRINGS WOULD BE NICE TO INSERT SINGLE CHARACTER*/
                    char tmp[2] = {'0', '\0'};
                    tmp[0] = pattern[i];
                    result = result + tmp;
                    parsed = false;
                    continue;
                }
                //check if the input is parsable to minutes, mm or MM or mM or Mm
                if ((i + 1 < pattern.getLength()) && ((pattern[i] == 'm') || (pattern[i] == 'M'))
                    && ((pattern[i + 1] == 'm') || (pattern[i + 1] == 'M'))) {
                        result = result + minute;
                        ++i;
                        parsed = true;
                        continue;
                }//check if the input is parsable to dates, dd or DD or dD or Dd
                else if ((i + 1 < pattern.getLength()) && ((pattern[i] == 'd') || (pattern[i] == 'D'))
                    && ((pattern[i + 1] == 'd') || (pattern[i + 1] == 'D'))) {
                        result = result + date;
                        ++i;
                        parsed = true;
                        continue;
                }//check if the input is parsable to hours, hh or HH or hH or Hh
                else if ((i + 1 < pattern.getLength()) && ((pattern[i] == 'h') || (pattern[i] == 'H'))
                    && ((pattern[i + 1] == 'h') || (pattern[i + 1] == 'H'))) {
                        result = result + hour;
                        ++i;
                        parsed = true;
                        continue;
                }//check if the input is parsable to seconds, ss or SS or sS or Ss
                else if ((i + 1 < pattern.getLength()) && ((pattern[i] == 's') || (pattern[i] == 'S'))
                    && ((pattern[i + 1] == 's') || (pattern[i + 1] == 'S'))) {
                        result = result + seconds;
                        ++i;
                        parsed = true;
                        continue;
                }//check if the input is parsable to years, yyyy or YYYY and other variations
                else if ((i + 3 < pattern.getLength()) && ((pattern[i] == 'y') || (pattern[i] == 'Y'))
                    && ((pattern[i + 1] == 'y') || (pattern[i + 1] == 'Y'))
                    && ((pattern[i + 2] == 'y') || (pattern[i + 2] == 'Y'))
                    && ((pattern[i + 3] == 'y') || (pattern[i + 3] == 'Y'))) {
                        result = result + year;
                        i = i + 3;
                        parsed = true;
                        continue;
                }//check if the input is parsable to years, mon or MON and other variations
                else if ((i + 2 < pattern.getLength()) && ((pattern[i] == 'm') || (pattern[i] == 'M'))
                    && ((pattern[i + 1] == 'o') || (pattern[i + 1] == 'O'))
                    && ((pattern[i + 2] == 'n') || (pattern[i + 2] == 'N'))) {
                        result = result + month;
                        i = i + 2;
                        parsed = true;
                        continue;
                }//check if the input is parsable to years, day or DAY and other variations
                else if ((i + 2 < pattern.getLength()) && ((pattern[i] == 'd') || (pattern[i] == 'D'))
                    && ((pattern[i + 1] == 'a') || (pattern[i + 1] == 'A'))
                    && ((pattern[i + 2] == 'y') || (pattern[i + 2] == 'Y'))) {
                        result = result + day;
                        i = i + 2;
                        parsed = true;
                        continue;
                }
                //there is some invalid inputs therefore reset the result and return invalid
                result = "";
                return CAPU_EINVAL;
            }
            return CAPU_OK;
        }

    }
}

#endif

