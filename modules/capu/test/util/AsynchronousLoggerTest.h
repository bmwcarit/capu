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


#ifndef CAPU_ASYNCHRONOUSELOGGERTEST_H
#define CAPU_ASYNCHRONOUSELOGGERTEST_H

#include "gmock/gmock.h"
#include <capu/container/Hash.h>
#include "capu/util/ILogAppender.h"
#include "capu/util/AsynchronousLogger.h"
#include "capu/util/ConsoleLogAppender.h"
#include "LoggerTest.h"

namespace capu
{

    class AsynchronousLoggerTest : public testing::Test
    {
    public:

        AsynchronousLoggerTest();
        ~AsynchronousLoggerTest();

    protected:
        MockLogAppender appender;
        AsynchronousLogger defaultLogger;
        LogContext& CAPU_CONTEXT;
        LogContext& LOGGER_CONTEXT;
        LogContext& HELLO_CAPU_CONTEXT;

        void logWithDefaultLogger();

    };
}

#endif // CAPU_ASYNCHRONOUSELOGGERTEST_H
