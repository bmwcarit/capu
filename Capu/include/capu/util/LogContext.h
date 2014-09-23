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

#ifndef CAPU_LOGCONTEXT_H
#define CAPU_LOGCONTEXT_H

#include "capu/container/String.h"
#include "capu/util/LogLevel.h"

namespace capu
{

    /**
     * A LogContext groups log messages which are related. This is
     * to enable or disable groups of log messages
     */
    class LogContext
    {
    public:

        /**
         * Constructor of LogContext
         * @param name of the context
         */
        LogContext(const String& name);

        /**
         * Returns the name of the LogContext
         * @return the name of the LogContext
         */
        const String& getContextName() const;

        /**
         * Enables the LogContext for logging
         * @param enabled state of the LogContext
         */
        void setEnabled(bool_t enabled);

        /**
         * Returns the enabled state of the LogContext
         * @return the enabled state of the LogContext
         */
        bool_t isEnabled() const;


        /**
         * Sets the LogLevel of the context
         * @param logLevel for the context
         */
        void setLogLevel(const ELogLevel logLevel);

        /**
         * Returns the current LogLevel of the Context
         * @return the current LogLevel of the Context
         */
        ELogLevel getLogLevel() const;

    private:

        /**
         * The name of the LogContext
         */
        const String m_contextName;

        /**
         * Enabled state of the LogContext
         */
        bool_t m_enabled;

        /**
         * LogLevel of context
         */
        ELogLevel m_logLevel;
        
        /**
         * Private assignment operator prevents misuse
         */
        LogContext operator=(const LogContext lc);
    };

    inline
    void LogContext::setLogLevel(const ELogLevel logLevel)
    {
        m_logLevel = logLevel;
    }

    inline
    ELogLevel LogContext::getLogLevel() const
    {
        return m_logLevel;
    }

    inline
    const String& 
    LogContext::getContextName() const
    {
        return m_contextName;
    }

    inline
    void 
    LogContext::setEnabled(bool_t enabled)
    {
        m_enabled = enabled;
    }

    inline
    bool_t 
    LogContext::isEnabled() const
    {
        return m_enabled;
    }
}

#endif // CAPU_LOGCONTEXT_H
