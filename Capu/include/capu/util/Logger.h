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

#ifndef CAPU_LOGGER_H
#define CAPU_LOGGER_H

#include "capu/container/HashSet.h"
#include "capu/util/StringOutputStream.h"
#include "capu/os/LightweightMutex.h"
#include "capu/util/LogLevel.h"
#include "capu/util/LogMessage.h"
#include "capu/util/ScopedLock.h"
#include "capu/container/vector.h"

namespace capu
{
    class ILogAppender;
    class String;
    class LogContext;
    

#define LOG_EXT(logger, context, logLevel, message)                     \
        if(logLevel >= context.getLogLevel() && context.isEnabled())    \
        {                                                               \
            capu::LogMessage logMessage(context, logLevel);             \
            (logMessage.getStream() << message).flush();                \
            logger.log(logMessage);                                     \
        }

#define LOG(context, logLevel, message)           \
    {                                             \
        LOG_EXT((*capu::Logger::GetDefaultLogger()), context, logLevel, message) \
    }

#define LOG_TRACE_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_TRACE, message)

#define LOG_INFO_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_INFO, message)

#define LOG_DEBUG_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_DEBUG, message)

#define LOG_WARN_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_WARN, message)

#define LOG_ERROR_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_ERROR, message)

#define LOG_FATAL_EXT(logger, context, message) \
    LOG_EXT(logger, (context), capu::LL_FATAL, message)


#define LOG_TRACE(context, message) \
    LOG((context), capu::LL_TRACE, message)

#define LOG_INFO(context, message) \
    LOG((context), capu::LL_INFO, message)

#define LOG_DEBUG(context, message) \
    LOG((context), capu::LL_DEBUG, message)

#define LOG_WARN(context, message) \
    LOG((context), capu::LL_WARN, message)

#define LOG_ERROR(context, message) \
    LOG((context), capu::LL_ERROR, message)

#define LOG_FATAL(context, message) \
    LOG((context), capu::LL_FATAL, message)

    /**
     * Logs messages to a given ILogAppender
     * There are two ways of logging available.
     * 1. Set a default logger with Logger::SetDefaultLogger and 
     *    use the log macros without "_EXT". In this case all log 
     *    messages will use the default logger
     * 2. Use the log macros with "_EXT" and provide a logger to the
     *    macro. In this case the log message will use the provided logger
     */
    class Logger
    {
    public:

        /**
         * Sets the LogLevel of all contexts
         * @param logLevel for all Contexts
         * @param filterContext only contexts which has the substring 'filterContext' in their name or id will change their logLevel,
                  if filterContext is empty all contexts of the logger will be modified
         * @{
         */
        static void SetLogLevel(const ELogLevel logLevel, const String& filterContext = "");
        void setLogLevel(const ELogLevel logLevel, const String& filterContext = "");
        /**
         *@}
         */


        /**
         * Get log level of a given context id
         * @param[out] logLevel context log level
         * @param[in] contextId context identifier
         * @{
         */
        static void GetLogLevel(const String& contextId,ELogLevel& logLevel);
        void getLogLevel(const String& contextId,ELogLevel& logLevel);
        /**
         *@}
         */

        /**
         * Get context name by its id
         * @param[in] contextId Context ID
         * @param[out] contextName Context name from the query
         * @{
         */
        static void GetContextNameByContextId(const String& contextId, String& contextName);
        void getContextName(const String& contextId, String& contextName);
        /**
         *@}
         */

        /**
         * Get list of IDs of contexts created already
         * @param[out] contextIds context identifiers vector from the query
         * @{
         */
        static void GetContextIds(vector<String>& contextIds);
        void getContextIds(vector<String>& contextIds);
        /**
         *@}
         */

        /**
         * Sets the default logger
         * @return the method always returns 0. This is just 
         *         to allow usage in static initializations
         */
        static void* SetDefaultLogger(Logger& logger);

        /**
         * Returns the instance to the global logger
         * @return the instance to the global logger
         */
        static Logger* GetDefaultLogger();

        /**
         * Creates a new LogContext
         * @param name of the LogContext
         * @param id of the LogContext
         * @return the create LogContext
         */
        LogContext& createContext(const String& name, const String& id = "");

        /**
         * Logs the given LogMessage with all registered LogAppenders of 
         * of the logger
         * @param message to log
         * @{
         */
        static void Log(const LogMessage& message);
        virtual void log(const LogMessage& message);
        /**
         * @}
         */

        /**
         * Constructor of the logger
         * @param appender to use for logging
         */
        Logger(ILogAppender& appender);

        /**
         * Destructor of the Logger
         */
        virtual ~Logger();

        /**
         * Sets all contexts to the given enabled value which start with the 
         * given pattern.
         * E.g. setEnabled(true, "capu"); 
         * Will enable all contexts which start with capu
         * @param enabled state of the context
         * @param pattern for start of context
         * @{
         */
        void setEnabled(bool enabled, const String& pattern);
        static void SetEnabled(bool enabled, const String& pattern);
        /**
         * @}
         */

        /**
         * Adds the given appender to the logger
         * @param appender to add
         * @{
         */
        void addAppender(ILogAppender& appender);
        static void AddAppender(ILogAppender& appender);
        /**
         * @}
         */

        /**
         * Removes the given appender from the logger
         * @param appender to remove
         * @{
         */
        void removeAppender(ILogAppender& appender);
        static void RemoveAppender(ILogAppender& appender);
        /**
         * @}
         */

    protected:

        static Logger* DefaultLogger;

    private:
        typedef HashSet<LogContext*> ContextSet;
        typedef HashSet<ILogAppender*> AppenderSet;

        /**
         * HashSet with all LogContexts
         */
        ContextSet m_logContexts;

        /**
         * Set with all log appenders
         */
        AppenderSet m_appenders;

        LightweightMutex m_appenderLock;
        typedef ScopedLock<LightweightMutex> MutexLocker;

    };

    inline
    Logger* Logger::GetDefaultLogger()
    {
        return Logger::DefaultLogger;
    }

    inline 
    void 
    Logger::SetLogLevel(const ELogLevel logLevel, const String& filterContext)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->setLogLevel(logLevel, filterContext);
        }
    }

    inline 
    void 
    Logger::setLogLevel(const ELogLevel logLevel, const String& filterContext)
    {
        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(; current != end; ++current)
        {
            LogContext* lc = *current;

            if (filterContext == "" ||
                lc->getContextName().find(filterContext) >= 0 ||
                lc->getContextId().find(filterContext) >= 0)
            {
                lc->setLogLevel(logLevel);
            }
        }
    }

    inline
    void
    Logger::GetLogLevel(const String& filterContext, ELogLevel& logLevel)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->getLogLevel(filterContext,logLevel);
        }
    }

    inline
    void
    Logger::getLogLevel(const String& contextId, ELogLevel& logLevel)
    {
        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(; current != end; ++current)
        {
            LogContext* lc = *current;

            if (lc->getContextId() == contextId)
            {
                logLevel = lc->getLogLevel();
                break;
            }
        }
    }

    inline
    void
    Logger::GetContextNameByContextId(const String& contextId, String& contextName)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->getContextName(contextId, contextName);
        }
    }

    inline
    void
    Logger::getContextName(const String& contextId, String& contextName)
    {
        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(; current != end; ++current)
        {
            LogContext* lc = *current;

            if (lc->getContextId() == contextId)
            {
                contextName = lc->getContextName();
                break;
            }
        }
    }

    inline
    void
    Logger::GetContextIds(vector<String>& contextIds)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->getContextIds(contextIds);
        }
    }

    inline
    void
    Logger::getContextIds(vector<String>& contextIds)
    {
        ContextSet::Iterator current = m_logContexts.begin();
        const ContextSet::Iterator end = m_logContexts.end();

        for(; current != end; ++current)
        {
            LogContext* lc = *current;

            contextIds.push_back(lc->getContextId());
        }
    }

    inline
    void
    Logger::SetEnabled(bool enabled, const String& pattern)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->setEnabled(enabled, pattern);
        }
    }

    inline
    void 
    Logger::addAppender(ILogAppender& appender)
    {
        MutexLocker lock(m_appenderLock);
        m_appenders.put(&appender);
    }

    
    inline
    void
    Logger::removeAppender(ILogAppender& appender)
    {
        MutexLocker lock(m_appenderLock);
        m_appenders.remove(&appender);
    }

    inline
    void
    Logger::RemoveAppender(ILogAppender& appender)
    {
         if(0 != DefaultLogger)
         {
             DefaultLogger->removeAppender(appender);
         }
    }


    inline
    void 
    Logger::AddAppender(ILogAppender& appender)
    {
        if(0 != DefaultLogger)
        {
            DefaultLogger->addAppender(appender);
        }
    }

}

#endif // CAPU_LOGGER_H
