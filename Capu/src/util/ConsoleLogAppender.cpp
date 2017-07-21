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

#include "capu/util/ConsoleLogAppender.h"
#include "capu/util/LogMessage.h"
#include "capu/util/ScopedLock.h"
#include "capu/os/Console.h"
#include "capu/os/Time.h"
#include <stdio.h>

namespace capu
{
    LightweightMutex ConsoleLogAppender::m_logMutex;

    ConsoleLogAppender::~ConsoleLogAppender()
    {
    }

    void ConsoleLogAppender::logMessage(const LogMessage& logMessage)
    {
        ScopedLightweightMutexLock lock(m_logMutex);
        const uint64_t now = Time::GetMilliseconds();
        Console::Print(Console::WHITE, "%.3f ", now/1000.0);

        switch(logMessage.getLogLevel())
        {
        case LL_TRACE:
            Console::Print(Console::WHITE,  "[ Trace ] ");
            break;
        default:
        case LL_DEBUG :
            Console::Print(Console::WHITE,  "[ Debug ] ");
            break;
        case LL_INFO :
            Console::Print(Console::GREEN,  "[ Info  ] ");
            break;
        case LL_WARN :
            Console::Print(Console::YELLOW, "[ Warn  ] ");
            break;
        case LL_ERROR :
            Console::Print(Console::RED,    "[ Error ] ");
            break;
        case LL_FATAL :
            Console::Print(Console::RED,    "[ Fatal ] ");
            break;
        }

        Console::Print(Console::AQUA, logMessage.getContext().getContextName().c_str());
        Console::Print(" | %s\n", logMessage.getLogMessage());
        Console::Flush();
    }
}
