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

#ifndef CAPU_ILOGAPPENDER_H
#define CAPU_ILOGAPPENDER_H

#include "capu/util/LogLevel.h"
#include "capu/os/Atomic.h"

namespace capu
{
    class LogMessage;

    /**
     * Base class for LogAppenders. Can be used to implement additional appenders
     */
    class ILogAppender
    {
    public:

        /**
         * Logs the given message
         * @param logMessage to log
         */
        virtual void log(const LogMessage& logMessage) = 0;

        /**
         * Query for the log level of the appender
         * @returns the log level
         */
        virtual ELogLevel getLogLevel() const = 0;

        /**
         * Set the log level for the appender
         * @param level the log level to set the appender to
         */
        virtual void setLogLevel(ELogLevel level) = 0;

        virtual ~ILogAppender(){}

    protected:

        /**
         * Log level of the appender
         */
        Atomic<int32_t> m_logLevel;
    };

}


#endif // CAPU_ILOGAPPENDER_H
