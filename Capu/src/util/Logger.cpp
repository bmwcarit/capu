#include "capu/util/Logger.h"
#include "capu/util/ILogAppender.h"
#include "capu/container/String.h"
#include "capu/util/LogContext.h"
#include "capu/util/ConsoleLogAppender.h"

namespace capu
{
    Logger* Logger::DefaultLogger = 0;

    Logger::Logger(ILogAppender& appender)
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
    Logger::createContext(const String& name, const String& id)
    {
        LogContext* context = new LogContext(name, id);
        m_logContexts.put(context);
        return *context;
    }

    void
    Logger::Log(const LogMessage& message)
    {
        if(0 != DefaultLogger)
        {
            Logger::DefaultLogger->log(message);
        }
    }

    void
    Logger::log(const LogMessage& message)
    {
        MutexLocker lock(m_appenderLock);

        AppenderSet::Iterator current = m_appenders.begin();
        const AppenderSet::Iterator end = m_appenders.end();

        for(; current != end; ++current)
        {
            (*current)->log(message);
        }
    }

    void
    Logger::setEnabled(bool enabled, const String& pattern)
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
