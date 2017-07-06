/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#include "capu/util/LogMessage.h"

namespace capu
{

    LogMessage::LogMessage(const LogContext& context, ELogLevel logLevel)
        : m_context(&context)
        , m_logLevel(logLevel)
    {

    }

    LogMessage::LogMessage()
        : m_context(0)
        , m_logLevel(LL_ERROR)
    {

    }

    const char* LogMessage::getLogMessage() const
    {
        return m_outputStream.c_str();
    }

    uint32_t LogMessage::getLogMessageLength() const
    {
        return m_outputStream.length();
    }

    LogMessage& LogMessage::operator=(const LogMessage& other)
    {
        if(&other != this)
        {
            m_context = other.m_context;
            m_logLevel = other.m_logLevel;
            m_outputStream = other.m_outputStream;
        }
        return *this;
    }
}
