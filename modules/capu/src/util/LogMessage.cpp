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
        , m_logLevel(LL_DEBUG)
        , m_message("")
    {

    }

    const String& LogMessage::getLogMessage() const
    {
        return m_message;
    }

    LogMessage& LogMessage::operator=(const LogMessage& other)
    {
        if(&other != this)
        {
            m_context = other.m_context;
            m_logLevel = other.m_logLevel;
            m_message = other.m_message;
        }
        return *this;
    }
}