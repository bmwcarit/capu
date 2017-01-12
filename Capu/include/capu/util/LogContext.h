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
#include "capu/os/Atomic.h"

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
         * @param id of the context
         */
        LogContext(const String& name, const String& id = "");

        /**
         * Returns the name of the LogContext
         * @return the name of the LogContext
         */
        const String& getContextName() const;

        /**
        * Returns the id of the LogContext
        * @return the id of the LogContext
        */
        const String& getContextId() const;

        /**
         * Enables the LogContext for logging
         * @param enabled state of the LogContext
         */
        void setEnabled(bool enabled);

        /**
         * Returns the enabled state of the LogContext
         * @return the enabled state of the LogContext
         */
        bool isEnabled() const;


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

        /**
         * Set the general purpose pointer
         * @param dataPtr pointer to the data being stored
         */
        void setUserData(void* dataPtr);

        /**
         * Get the general purpose pointer
         * @return the data pointer
         */
        void* getUserData() const;

    private:

        /**
         * The name of the LogContext
         */
        const String m_contextName;

        /**
        * The id of the LogContext
        */
        const String m_contextId;

        /**
         * Enabled state of the LogContext
         */
        bool m_enabled;

        /**
         * LogLevel of context
         */
        Atomic<int32_t> m_logLevel;

        /**
         * Private assignment operator prevents misuse
         */
        LogContext operator=(const LogContext lc);

        /**
         * General purpose data
         */
        void* m_data;
    };

    inline
    void LogContext::setLogLevel(const ELogLevel logLevel)
    {
        m_logLevel = static_cast<int32_t>(logLevel);
    }

    inline
    ELogLevel LogContext::getLogLevel() const
    {
        return static_cast<ELogLevel>(m_logLevel.load());
    }

    inline
    const String& 
    LogContext::getContextName() const
    {
        return m_contextName;
    }

    inline
    const String&
    LogContext::getContextId() const
    {
        return m_contextId;
    }

    inline
    void 
    LogContext::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    inline
    bool
    LogContext::isEnabled() const
    {
        return m_enabled;
    }

    inline
    void
    LogContext::setUserData(void* dataPtr)
    {
        m_data = dataPtr;
    }

    inline
    void*
    LogContext::getUserData() const
    {
        return m_data;
    }
}

#endif // CAPU_LOGCONTEXT_H
