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
    class LogMessage
    {
    public:
        LogMessage();
        LogMessage(const LogContext& context, ELogLevel logLevel);

        StringOutputStream& getOutputStream();

        const String& getLogMessage() const;
        const LogContext& getContext() const;
        ELogLevel getLogLevel() const;
        void flush();

        LogMessage& operator=(const LogMessage& other);

    private:


        const LogContext* m_context;

        /**
         * The logging level of this logging message.
         */
        ELogLevel m_logLevel;

        String m_message;

        /**
         * The line from which the logging was called.
         */
        int32_t m_line;


        StringOutputStream m_outStream;

        /**8
         * The context from which the logging was called.
         */
        /*char_t m_context[MAX_CONTEXT_LENGTH];
        /**
         * The file from which the logging was called.
         */
      //  Char m_file[MAX_FILE_LENGTH];
        /**
         * The formatted logging message.
         */
       // Char m_message[MAX_MESSAGE_LENGTH];*/
    };

    inline
    StringOutputStream& LogMessage::getOutputStream()
    {
        return m_outStream;
    }

    inline
    void 
    LogMessage::flush()
    {
        m_message = m_outStream.c_str();
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