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

#ifndef CAPU_APPENDER_H
#define CAPU_APPENDER_H

#include "capu/util/Logger.h"

namespace capu
{
    /**
     * Interface for all appenders of log messages.
     */
    class Appender
    {
    protected:
        /**
         * The log level
         */
        capu::LoggerLevel mLvl;
    public:

        virtual ~Appender() {}

        /**
         * open logger appender
         */
        virtual status_t open() = 0;

        /**
         * print trace message to the logger appender.
         * @param message that will be logged
         */
        virtual status_t log(class LoggerMessage& message) = 0;

        /**
         * close logger appender
         */
        virtual status_t close() = 0;

        /**
         * set Logging Level               /\
         * Error Logging = CLL_ERROR,     /||\
         * Waring Logging = CLL_WARN,      || CLL_WARN includes CLL_ERROR etc.
         * Info Logging = CLL_INFO,        ||
         * Debug Logging = CLL_DEBUG,      ||
         * Trace Logging = CLL_TRACE,      ||
         */
        void setLoggingLevel(const LoggerLevel level)
        {
            mLvl = level;
        }
    };
}

#endif /* CAPU_APPENDER_H */
