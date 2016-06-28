#include "capu/util/ConsoleLogAppender.h"
#include "capu/util/LogMessage.h"
#include "capu/os/Console.h"
#include "capu/os/Time.h"
#include <stdio.h>

namespace capu
{
    Mutex ConsoleLogAppender::m_logMutex;

    ConsoleLogAppender::~ConsoleLogAppender()
    {
    }

    void ConsoleLogAppender::logMessage(const LogMessage& logMessage)
    {
        m_logMutex.lock();
        const uint64_t now = Time::GetMilliseconds();
        Console::Print(Console::WHITE, "%.3f ", now/1000.0);

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
        Console::Flush();
        m_logMutex.unlock();
    }
}
