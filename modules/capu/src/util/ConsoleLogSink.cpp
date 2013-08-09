#include "capu/util/ConsoleLogSink.h"
#include "capu/util/LogMessage.h"
#include "capu/os/Console.h"
#include <stdio.h>

namespace capu
{

    void ConsoleLogSink::log(const LogMessage& logMessage)
    {
        Console::ConsoleColor color;

        switch(logMessage.getLogLevel())
        {
            //LL_TRACE        = 0x1000,
            case LL_DEBUG        : color = Console::WHITE; break;
            //LL_INFO         = 0x3000,
            case LL_WARN         : color = Console::YELLOW; break;
            case LL_ERROR        : color = Console::RED; break;
            default:               color = Console::WHITE; break;
            //LL_FATAL        = 0x6000,
        }

        m_logMutex.lock();
        Console::Print("[ ");
        Console::Print(color, "%s", LogLevel::GetLogLevelText(logMessage.getLogLevel()));
        Console::Print(" ]\t%s:   %s\n", logMessage.getContext().getContextName().c_str(),  logMessage.getLogMessage().c_str());
        m_logMutex.unlock();
    }

   
}