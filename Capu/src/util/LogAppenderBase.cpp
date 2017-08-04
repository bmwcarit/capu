/*
 * Copyright (C) 2015 BMW Car IT GmbH
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


#include "capu/util/LogAppenderBase.h"

namespace capu
{
    LogAppenderBase::~LogAppenderBase()
    {
    }

    void LogAppenderBase::log(const LogMessage& message)
    {
        if(message.getLogLevel() >= m_logLevel)
        {
            logMessage(message);
        }
    }

    void LogAppenderBase::setLogLevel(ELogLevel logLevel)
    {
        m_logLevel = logLevel;
    }

    ELogLevel LogAppenderBase::getLogLevel() const
    {
        return static_cast<ELogLevel>(m_logLevel.load());
    }
}
