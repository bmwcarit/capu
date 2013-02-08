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

#include "capu/util/Logger.h"
#include "capu/util/Appender.h"
#include "capu/os/Memory.h"
#include "capu/container/Array.h"
#include "capu/os/Time.h"
#include "capu/os/Thread.h"

namespace capu
{
    //
    // LoggerMessage
    //

    LoggerMessage::LoggerMessage()
        : mId(0)
        , mTimestamp(0)
        , mThreadId(0)
        , mLevel(CLL_INVALID)
        , mTag("")
        , mFile("")
        , mLine(0)
        , mMessage("")
    {
    }

    LoggerMessage::~LoggerMessage()
    {
    }

    //
    // Logger
    //

    Logger::Logger(int32_t id)
        : mId(id)
        , mOpen(false)
    {
        Memory::Set(mAppenders, 0, sizeof(Appender*) * LOGGER_APPENDER_MAX);
    }

    Logger::~Logger()
    {
        if (mOpen)
        {
            close();
        }
    }

    status_t Logger::setAppender(Appender& appender)
    {
        for (int i = 0; i < LOGGER_APPENDER_MAX; i++)
        {
            if (mAppenders[i] == NULL)
            {
                mAppenders[i] = &appender;
                return CAPU_OK;
            }
        }
        return CAPU_ERROR;
    }

    status_t Logger::removeAppender(Appender& appender)
    {
        for (int i = 0; i < LOGGER_APPENDER_MAX; i++)
        {
            if (mAppenders[i] == &appender)
            {
                mAppenders[i] = NULL;
                return CAPU_OK;
            }
        }
        return CAPU_ERROR;
    }

    status_t Logger::open()
    {
        for (int i = 0; i < LOGGER_APPENDER_MAX; i++)
        {
            if (mAppenders[i] != NULL)
            {
                mAppenders[i]->open();
            }
        }
        mOpen = true;
        return CAPU_OK;
    }

    status_t Logger::vlog(const LoggerLevel level, const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, va_list args)
    {
        LoggerMessage msg;
        msg.setId(mId);
        msg.setTimestamp(Time::GetMilliseconds());
        msg.setThreadId(Thread::CurrentThreadId());
        msg.setLevel(level);
        msg.setTag(tag);
        msg.setFile(file);
        msg.setLine(line);

        const int32_t size = StringUtils::Vscprintf(msgFormat, args);
        Array<char_t> buffer(size + 1);
        StringUtils::Vsprintf(buffer.getRawData(), size + 1, msgFormat, args);
        msg.setMessage(buffer.getRawData());

        // log message
        for (int i = 0; i < LOGGER_APPENDER_MAX; i++)
        {
            if (mAppenders[i] != NULL)
            {
                mAppenders[i]->log(msg);
            }
        }
        return CAPU_OK;
    }

    status_t Logger::close()
    {
        for (int i = 0; i < LOGGER_APPENDER_MAX; i++)
        {
            if (mAppenders[i] != NULL)
            {
                mAppenders[i]->close();
            }
        }
        mOpen = false;
        return CAPU_OK;
    }
}
