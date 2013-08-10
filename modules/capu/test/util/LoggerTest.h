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


#ifndef CAPU_LOGGERTEST_H
#define CAPU_LOGGERTEST_H

#include "gmock/gmock.h"
#include <capu/container/Hash.h>
#include "capu/util/ILogAppender.h"
#include "capu/util/Logger.h"

namespace capu
{
    class MockLogAppender: public ILogAppender
    {
    public:
        MOCK_METHOD1(log, void(const LogMessage& logMessage));
    };

    class LoggerTest : public testing::Test
    {
    public:


        LoggerTest();
        ~LoggerTest();

    protected:
        MockLogAppender appender;
        Logger defaultLogger;
        LogContext* CAPU_CONTEXT; 
        LogContext* LOGGER_CONTEXT;
        LogContext* HELLO_CAPU_CONTEXT;

        void logWithDefaultLogger();
        void logWithLocalLogger(Logger& logger);

    };


}

#endif // CAPU_LOGGERTEST_H