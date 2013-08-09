#include "capu/util/AsynchronousConsoleLogSink.h"

namespace capu
{
    AsynchronousConsoleLogSink::AsynchronousConsoleLogSink()
    {
        m_loggerThread.start(*this);
    }

    AsynchronousConsoleLogSink::~AsynchronousConsoleLogSink()
    {
        m_loggerThread.cancel();
        m_loggerThread.join();
    }

    void AsynchronousConsoleLogSink::log(const LogMessage& logMessage)
    {
        m_logQueue.push(logMessage);
    }

    void AsynchronousConsoleLogSink::run()
    {
        while(!isCancelRequested())
        {
            LogMessage message;
            if(CAPU_OK == m_logQueue.pop(&message, 100))
            {
                ConsoleLogSink::log(message);
            }
        }
    }

}