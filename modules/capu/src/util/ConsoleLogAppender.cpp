#include "capu/util/ConsoleLogAppender.h"
#include "capu/util/LogMessage.h"
#include "capu/os/Console.h"
#include <stdio.h>

namespace capu
{

    void ConsoleLogAppender::log(const LogMessage& logMessage)
    {
        m_logMutex.lock();

        switch(logMessage.getLogLevel())
        {
        case LL_TRACE:
            Console::Print(Console::WHITE, "[  Trace  ] ");
            break;
        default:
        case LL_DEBUG :
            Console::Print(Console::WHITE, "[  Debug  ] ");
            break;
        case LL_INFO :
            Console::Print(Console::GREEN, "[  Info   ] ");
            break;
        case LL_WARN :
            Console::Print(Console::YELLOW, "[  Warn   ] ");
            break;
        case LL_ERROR :
            Console::Print(Console::RED, "[  Error  ] ");
            break;
        case LL_FATAL :
            Console::Print(Console::RED, "[  Fatal  ] ");
            break;
        }

        Console::Print(Console::AQUA, logMessage.getContext().getContextName().c_str());
        Console::Print(" | %s\n", logMessage.getLogMessage());
        m_logMutex.unlock();
    }
}
