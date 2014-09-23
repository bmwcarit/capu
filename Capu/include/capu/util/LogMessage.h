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

#ifndef CAPU_LOGMESSAGE_H
#define CAPU_LOGMESSAGE_H

#include "capu/util/LogLevel.h"
#include "capu/util/LogContext.h"
#include "capu/util/StringOutputStream.h"

namespace capu
{
    /**
     * This class encapsulates a log message. The containing data can be used in a ILogAppender to 
     * print the message.
     */
    class LogMessage
    {
    public:

        /**
         * Default constructor of LogMessage
         */
        LogMessage();

        /**
         * Constructor of LogMessage with context and logLevel
         * @param context of the LogMessage
         * @param logLevel of the LogMessage
         */
        LogMessage(const LogContext& context, ELogLevel logLevel);

        /**
         * Returns a StringOutputStream for formatting the message
         * @return StringOutputStream for formatting the message
         */
        StringOutputStream& getStream();

        /**
         * Returns a const char_t pointer to the message
         * @return a const char_t pointer to the message
         */
        const char_t* getLogMessage() const;

        /**
         * Returns the context of the message
         * @return the context of the message
         */
        const LogContext& getContext() const;

        /**
         * Returns the logLevel of the message
         * @return the logLevel of the message
         */
        ELogLevel getLogLevel() const;

        /**
         * Assignment operator for LogMessage
         * @param other LogMessage to assign to this message
         */
        LogMessage& operator=(const LogMessage& other);

    private:

        /**
         * Pointer to the used LogContext for the message
         */
        const LogContext* m_context;

        /**
         * The logging level of this logging message.
         */
        ELogLevel m_logLevel;

        /**
         * StringOutputStream for formatting the message
         */
        StringOutputStream m_outputStream;

    };

    inline
    StringOutputStream& LogMessage::getStream()
    {
        return m_outputStream;
    }


    inline
    const LogContext&
    LogMessage::getContext() const
    {
        return *m_context;
    }

    inline
    ELogLevel
    LogMessage::getLogLevel() const
    {
        return m_logLevel;
    }
}

#endif // CAPU_LOGMESSAGE_H
