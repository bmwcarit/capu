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

#include <gtest/gtest.h>
#include "capu/Config.h"
#include "capu/os/TimeStamp.h"
#include "capu/os/Thread.h"

TEST(TimeStampTest, getFormattedStringTest)
{
    capu::TimeStamp stamp;
    capu::String str;
    capu::String pattern;
    //get regular current time stamp
    EXPECT_TRUE(stamp.getFormattedString(str) == capu::CAPU_OK);
    //str should be modified although there is an string associated
    str = "not empty";
    EXPECT_TRUE(stamp.getFormattedString(str) == capu::CAPU_OK);
    EXPECT_FALSE(str == "not empty");

    //EMPTY PATTERN FORMAT TEST
    str = "";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    str = "not empty";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);


    //CORRECT INPUTS TEST
    str = "";
    pattern = "mm";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "dd";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "yyyy";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "mon";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "ss";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "day";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd_mon";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd_mon_yyyy";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd_mon_yyyy_mm";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd_mon_yyyy_mm_ss";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    str = "";
    pattern = "hh_dd_mon_yyyy_mm_ss_day";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_OK);
    EXPECT_TRUE(str.getLength() != 0);

    //INCORRECT INPUTS TEST
    str = "";
    pattern = "________";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "hh_dd__mon_yyyy_mm_ss_day";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "hh_dd_Amon_yyyy_mm_ss_day";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "hh_ddamon_yyyy_mm_ss_day";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "asdasd";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "asdasd_asdasdsad";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);

    str = "";
    pattern = "_";
    EXPECT_TRUE(stamp.getFormattedString(pattern, str) == capu::CAPU_EINVAL);
    EXPECT_TRUE(str.getLength() == 0);
}

TEST(TimeStampTest, LessThanTest)
{
    capu::TimeStamp time1 = capu::TimeStamp::GetCurrentDateAndTime();
    capu::Thread::Sleep(2000);
    capu::TimeStamp time2 = capu::TimeStamp::GetCurrentDateAndTime();
    EXPECT_TRUE(time1 < time2);
    EXPECT_FALSE(time2 < time1);
}

TEST(TimeStampTest, GreaterThanTest)
{
    capu::TimeStamp time1 = capu::TimeStamp::GetCurrentDateAndTime();
    capu::Thread::Sleep(2000);
    capu::TimeStamp time2 = capu::TimeStamp::GetCurrentDateAndTime();

    EXPECT_TRUE(time1 > time2);
    EXPECT_FALSE(time2 > time1);
}

TEST(TimeStampTest, EqualToTest)
{
    capu::TimeStamp time1 = capu::TimeStamp::GetCurrentDateAndTime();
    capu::TimeStamp time2 = capu::TimeStamp::GetCurrentDateAndTime();
    EXPECT_TRUE(time1 == time2);
    capu::Thread::Sleep(2000);
    time2 = capu::TimeStamp::GetCurrentDateAndTime();
    EXPECT_FALSE(time1 == time2);
}
