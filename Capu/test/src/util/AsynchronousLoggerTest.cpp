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

#include "AsynchronousLoggerTest.h"
#include "capu/util/ConsoleLogAppender.h"
#include "capu/util/Logger.h"
#include "capu/container/String.h"
#include "capu/util/LogMessage.h"

namespace capu
{
    AsynchronousLoggerTest::AsynchronousLoggerTest()
        : defaultLogger(appender)
        , CAPU_CONTEXT(defaultLogger.createContext("capu.Logger", "CALO"))
        , LOGGER_CONTEXT(defaultLogger.createContext("capu.OtherContext", "CAOT"))
        , HELLO_CAPU_CONTEXT(defaultLogger.createContext("Hello.Capu", "HECA"))
    {
        Logger::SetDefaultLogger(defaultLogger);

        Logger::SetLogLevel(LL_ALL);
    }

    AsynchronousLoggerTest::~AsynchronousLoggerTest()
    {
    }

    void AsynchronousLoggerTest::logWithDefaultLogger()
    {
        LOG_TRACE(CAPU_CONTEXT       , "Trace message " << 5     );
        LOG_DEBUG(LOGGER_CONTEXT     , "Debug message " << 42.0f );
        LOG_INFO(CAPU_CONTEXT        , "Info message "  << 42    );
        LOG_WARN(HELLO_CAPU_CONTEXT  , "Warn message "  << 42    );
        LOG_ERROR(LOGGER_CONTEXT     , "Error message " << 42u   );
        LOG_FATAL(HELLO_CAPU_CONTEXT , "Fatal message " << 42u   );
    }


    TEST_F(AsynchronousLoggerTest, LogWithDefaultLogger)
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
}
