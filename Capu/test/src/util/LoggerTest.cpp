/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#include "LoggerTest.h"
#include "capu/util/ConsoleLogAppender.h"
#include "capu/util/Logger.h"
#include "capu/container/String.h"
#include "capu/util/LogMessage.h"

namespace capu
{
    LoggerTest::LoggerTest()
        : defaultLogger(appender)
        , CAPU_CONTEXT(defaultLogger.createContext("capu.Logger", "CALO"))
        , LOGGER_CONTEXT(defaultLogger.createContext("capu.OtherContext", "CAOT"))
        , HELLO_CAPU_CONTEXT(defaultLogger.createContext("Hello.Capu", "HECA"))
    {
        Logger::SetDefaultLogger(defaultLogger);

        Logger::SetLogLevel(LL_ALL);
    }

    LoggerTest::~LoggerTest()
    {
    }

    void LoggerTest::logWithDefaultLogger()
    {
        LOG_TRACE(CAPU_CONTEXT       , "Trace message " << 5     );
        LOG_DEBUG(LOGGER_CONTEXT     , "Debug message " << 42.0f );
        LOG_INFO(CAPU_CONTEXT        , "Info message "  << 42    );
        LOG_WARN(HELLO_CAPU_CONTEXT  , "Warn message "  << 42    );
        LOG_ERROR(LOGGER_CONTEXT     , "Error message " << 42u   );
        LOG_FATAL(HELLO_CAPU_CONTEXT , "Fatal message " << 42u   );
    }

    void LoggerTest::logWithLocalLogger(Logger& logger)
    {
        LOG_TRACE_EXT(logger, CAPU_CONTEXT       , "Trace message " << 5     );
        LOG_DEBUG_EXT(logger, LOGGER_CONTEXT     , "Debug message " << 42.0f );
        LOG_INFO_EXT (logger, CAPU_CONTEXT       , "Info message "  << 42    );
        LOG_WARN_EXT (logger, HELLO_CAPU_CONTEXT , "Warn message "  << 42    );
        LOG_ERROR_EXT(logger, LOGGER_CONTEXT     , "Error message " << 42u   );
        LOG_FATAL_EXT(logger, HELLO_CAPU_CONTEXT , "Fatal message " << 42u   );
    }

    TEST_F(LoggerTest, LogWithDefaultLogger)
    {
        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogWithLocalLogger)
    {
        Logger localLogger(appender);

        localLogger.setLogLevel(LL_ALL);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithLocalLogger(localLogger);
    }

    TEST_F(LoggerTest, LogFatal)
    {
        Logger::SetLogLevel(LL_FATAL);

        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();

    }

    TEST_F(LoggerTest, LogError)
    {
        Logger::SetLogLevel(LL_ERROR);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogWarn)
    {
        Logger::SetLogLevel(LL_WARN);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogInfo)
    {
        Logger::SetLogLevel(LL_INFO);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogDebug)
    {
        Logger::SetLogLevel(LL_DEBUG);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogTrace)
    {
        Logger::SetLogLevel(LL_TRACE);

        testing::InSequence seq;
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, LogOnlyOneContext)
    {
        {
            CAPU_CONTEXT.setEnabled(false);

            testing::InSequence seq;
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

            logWithDefaultLogger();
        }

        {
            LOGGER_CONTEXT.setEnabled(false);
            CAPU_CONTEXT.setEnabled(true);
            
            testing::InSequence seq;
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

            logWithDefaultLogger();
        }

        {
            CAPU_CONTEXT.setEnabled(false);

            testing::InSequence seq;
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

            logWithDefaultLogger();
        }

        {
            HELLO_CAPU_CONTEXT.setEnabled(false);
            logWithDefaultLogger();
        }
    }

    TEST_F(LoggerTest, EnableContextByPattern)
    {
        {
            Logger::SetEnabled(false, "capu");

            testing::InSequence seq;
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

            logWithDefaultLogger();
        }

        {
            Logger::SetEnabled(true, "capu.Logger");

            testing::InSequence seq;
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

            logWithDefaultLogger();
        }

        {
            Logger::SetEnabled(false, "Hello.Capu");
            Logger::SetLogLevel(LL_INFO);

            EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));

            logWithDefaultLogger();
        }
    }

    TEST_F(LoggerTest, AddAppender)
    {
        MockLogAppender appender2;

        Logger::AddAppender(appender2);

        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, AddAppenderTwice)
    {
        Logger::AddAppender(appender);

        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST_F(LoggerTest, RemoveAppender)
    {
        MockLogAppender appender2;

        Logger::AddAppender(appender2);

        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender2, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();

        Logger::RemoveAppender(appender2);

        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_TRACE)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_DEBUG)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_INFO)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_WARN)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_ERROR)));
        EXPECT_CALL(appender, logMessage(testing::Property(&LogMessage::getLogLevel, LL_FATAL)));

        logWithDefaultLogger();
    }

    TEST(LogContextTest, SetAndGetUserData)
    {
        LogContext context("LogContext name", "LOCO");
        void* someUserData = (void*)0x123;
        context.setUserData(someUserData);
        EXPECT_EQ(someUserData, context.getUserData());
    }

    TEST(LogAppenderTest,SetAndGetLogLevel)
    {
        ConsoleLogAppender consoleAppender;
        ELogLevel ll = capu::LL_ALL;
        consoleAppender.setLogLevel(ll);
        EXPECT_EQ(ll,consoleAppender.getLogLevel());
    }
}
