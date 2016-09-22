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

#include "capu/util/AsynchronousLogger.h"

namespace capu
{
    AsynchronousLogger::AsynchronousLogger(ILogAppender& appender)
        :  Logger(appender)
        , m_loggerThread("capu::AsynchronousLogger")
    {
        m_loggerThread.start(*this);
    }

    AsynchronousLogger::~AsynchronousLogger()
    {
        m_loggerThread.cancel();
        m_loggerThread.join();

        while(!m_logQueue.empty())
        {
            LogMessage message;
            m_logQueue.pop(&message);
            Logger::log(message);
        }
    }

    void
    AsynchronousLogger::log(const LogMessage& message)
    {
        m_logQueue.push(message);
    }

    void
    AsynchronousLogger::run()
    {
        while(!isCancelRequested())
        {
            LogMessage message;
            if(CAPU_OK == m_logQueue.pop(&message, 100))
            {
                Logger::log(message);
            }
        }
    }
}
