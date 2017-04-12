/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_LOGAPPENDERBASE_H
#define CAPU_LOGAPPENDERBASE_H

#include "capu/util/ILogAppender.h"
#include "capu/util/LogMessage.h"

namespace capu
{
    class LogAppenderBase : public ILogAppender
    {
    public:

        /**
         * Default Constructor using log level LL_ALL
         */
        LogAppenderBase()
        {
            setLogLevel(LL_ALL);
        }

        /**
         * Logs a message after LogAppenderBase has performed a log level comparison
         */
        virtual void logMessage(const LogMessage& message) = 0;

        /**
         * Logs the current message and performs a log level check
         */
        virtual void log(const LogMessage& message) override;

        /**
         * Query for the log level of the appender
         * @returns the log level
         */
        virtual ELogLevel getLogLevel() override;

        /**
         * Set the log level for the appender
         * @param level the log level to set the appender to
         */
        virtual void setLogLevel(ELogLevel level) override;

        /**
         * virtual destructor
         */
        virtual ~LogAppenderBase();
    };

}


#endif // CAPU_LOGAPPENDERBASE_H
