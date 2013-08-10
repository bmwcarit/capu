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
        , m_logLevel(LL_ERROR)
    {

    }

    const char_t* LogMessage::getLogMessage() const
    {
        return m_outputStream.c_str();
    }

    LogMessage& LogMessage::operator=(const LogMessage& other)
    {
        if(&other != this)
        {
            m_context = other.m_context;
            m_logLevel = other.m_logLevel;
            m_outputStream = other.m_outputStream;
        }
        return *this;
    }
}