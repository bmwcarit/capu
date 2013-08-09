#include "capu/util/Logger2.h"
#include "capu/util/LogSink.h"
#include "capu/container/String.h"
#include "capu/util/LogContext.h"

namespace capu
{
    Logger2* Logger2::Instance = 0;

    Logger2::Logger2(LogSink& logSink)
        : m_logSink(logSink)
    {

    }

    Logger2::~Logger2()
    {

        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(;current != end; ++current)
        {
            delete (*current);
        }

        m_logContexts.clear();
    }

    Logger2& Logger2::InitGlobalLogger(LogSink& logSink)
    {
        if(0 == Instance)
        {
            Instance = new Logger2(logSink);
        }
        return *Instance;
    }

    LogContext& Logger2::CreateContext(const String& name)
    {
        return Logger2::Instance->createContext(name);
    }

    LogContext& Logger2::createContext(const String& name)
    {
        LogContext* context = new LogContext(name);
        m_logContexts.put(context);
        return *context;
    }

    void Logger2::Log(const LogMessage& message)
    {
        Logger2::Instance->log(message);
    }

    void Logger2::log(const LogMessage& message)
    {
        m_logSink.log(message);
    }

}