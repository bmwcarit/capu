#include "capu/util/Logger.h"
#include "capu/util/ILogAppender.h"
#include "capu/container/String.h"
#include "capu/util/LogContext.h"
#include "capu/util/ConsoleLogAppender.h"

namespace capu
{
    Logger* Logger::DefaultLogger = 0;

    Logger::Logger(ILogAppender& appender)
        : m_logLevel(LL_ERROR)
    {
        addAppender(appender);
    }

    Logger::~Logger()
    {

        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(;current != end; ++current)
        {
            delete (*current);
        }

        m_logContexts.clear();

    }

    void*
    Logger::SetDefaultLogger(Logger& logger)
    {
        DefaultLogger = &logger;
        return 0;
    }

    LogContext&
    Logger::createContext(const String& name)
    {
        LogContext* context = new LogContext(name);
        m_logContexts.put(context);
        return *context;
    }

    void
    Logger::Log(const LogMessage& message)
    {
        Logger::DefaultLogger->log(message);
    }

    void
    Logger::log(const LogMessage& message)
    {
        AppenderSet::Iterator current = m_appenders.begin();
        const AppenderSet::Iterator end = m_appenders.end();

        for(; current != end; ++current)
        {
            (*current)->log(message);
        }
    }

    void
    Logger::setEnabled(bool_t enabled, const String& pattern)
    {
        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(; current != end; ++current)
        {
            if((*current)->getContextName().startsWith(pattern))
            {
                (*current)->setEnabled(enabled);
            }
        }
    }

}