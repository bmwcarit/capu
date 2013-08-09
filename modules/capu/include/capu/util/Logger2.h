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

#ifndef CAPU_LOGGER2_H
#define CAPU_LOGGER2_H

#include "capu/container/HashSet.h"

namespace capu
{
    class LogSink;
    class String;
    class LogContext;
    class LogMessage;

#define LOG2(context, logLevel, message) \
    { \
        LogMessage logMessage(context, logLevel); \
        logMessage.getOutputStream() << message; \
        logMessage.flush(); \
        Logger2::Log(logMessage);\
    }
    

#define LOG2_FANCYNAME(logger, context, logLevel, message) \
    { \
    LogMessage logMessage(context, logLevel); \
    logMessage.getOutputStream() << message; \
    logMessage.flush(); \
    logger.log(logMessage);\
    }

#define LOG_WARN(context, message) \
    LOG2(context, LL_WARN, message);

    class Logger2
    {
    public:

        static Logger2& InitGlobalLogger(LogSink& logSink);

        static LogContext& CreateContext(const String& name);
        LogContext& createContext(const String& name);

        static void Log(const LogMessage& message);

        void log(const LogMessage& message);

        Logger2(LogSink& logSink);
        ~Logger2();

    private:
        LogSink& m_logSink;

        static Logger2* Instance;

        
        typedef HashSet<LogContext*> ContextSet;
        ContextSet m_logContexts;

    };


  /*  inline
    Logger2& Logger2::GetInstance()
    {
        return *Logger2::Instance;
    }*/

}

#endif // CAPU_LOGGER2_H