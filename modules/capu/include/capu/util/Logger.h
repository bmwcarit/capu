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

#include "capu/Error.h"
#include "capu/Config.h"
#include "capu/container/String.h"
#include "capu/os/StringUtils.h"
#include <stdarg.h>

// Defines the max number of possible appenders
#define LOGGER_APPENDER_MAX 10

#if CAPU_LOGGING_ENABLED
#define CAPU_LOG(logger, level, tag, format, ...) if(logger != NULL) logger->log(level, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define CAPU_LOG_TRACE(logger, tag, format, ...) if(logger != NULL) logger->log(capu::CLL_TRACE, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define CAPU_LOG_DEBUG(logger, tag, format, ...) if(logger != NULL) logger->log(capu::CLL_DEBUG, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define CAPU_LOG_INFO(logger, tag, format, ...) if(logger != NULL) logger->log(capu::CLL_INFO, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define CAPU_LOG_WARN(logger, tag, format, ...) if(logger != NULL) logger->log(capu::CLL_WARN, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define CAPU_LOG_ERROR(logger, tag, format, ...) if(logger != NULL) logger->log(capu::CLL_ERROR, tag, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define CAPU_LOG(logger, level, tag, format, ...)
#define CAPU_LOG_TRACE(logger, tag, format, ...)
#define CAPU_LOG_DEBUG(logger, tag, format, ...)
#define CAPU_LOG_INFO(logger, tag, format, ...)
#define CAPU_LOG_WARN(logger, tag, format, ...)
#define CAPU_LOG_ERROR(logger, tag, format, ...)
#endif

namespace capu
{
    class Appender;

    /**
     * Logger levels
     */
    enum LoggerLevel
    {
        CLL_INVALID,
        CLL_TRACE,
        CLL_DEBUG,
        CLL_INFO,
        CLL_WARN,
        CLL_ERROR
    };

    /**
     * Message for logging.
     */
    class LoggerMessage
    {
    public:
        /**
         * Creates a new LoggerMessage instance
         */
        LoggerMessage();

        /**
         * Set the id
         * @param id the id to set
         */
        void setId(const int32_t id);

        /**
         * Get the message id
         * @return the id
         */
        int32_t getId() const;

        /**
         * Set the timestamp
         * @param timestamp the timestamp to set
         */
        void setTimestamp(const uint64_t timestamp);

        /**
         * Get the timestamp
         * @return the timestamp
         */
        uint64_t getTimestamp() const;

        /**
         * Set the thread id
         * @param threadId the thread id to set
         */
        void setThreadId(const uint_t threadId);

        /**
         * Get the thread id
         * @return the thread id
         */
        uint_t getThreadId() const;

        /**
         * Set the log level
         * @param level the log level to set
         */
        void setLevel(const LoggerLevel level);

        /**
         * Get the log level
         * @return the log level
         */
        LoggerLevel getLevel() const;

        /**
         * Set the tag
         * @param tag the tag to set
         */
        void setTag(const char_t* tag);

        /**
         * Get the tag
         * @return the tag
         */
        const char_t* getTag() const;

        /**
         * Set the filename
         * @param file the filename to set
         */
        void setFile(const char_t* file);

        /**
         * Get the filename
         * @return the filename
         */
        const char_t* getFile() const;

        /**
         * Set the line
         * @param line the line to set
         */
        void setLine(const int32_t line);

        /**
         * Get the line
         * @return the line
         */
        int32_t getLine() const;

        /**
         * Set the log message
         * @param message the message to set
         */
        void setMessage(const char_t* message);

        /**
         * Get the log message
         * @return
         */
        const char_t* getMessage() const;

        /**
         * cleanup current message
         */
        ~LoggerMessage();

    private:
        int32_t mId;
        uint64_t mTimestamp;
        uint_t mThreadId;
        LoggerLevel mLevel;
        String mTag;
        String mFile;
        int32_t mLine;
        String mMessage;
    };

    /**
     * Class for logging
     */
    class Logger
    {
    public:

        /**
         * creats a new Logger
         * @param id of the logger
         */
        Logger(int32_t id = 0);

        /**
         * destroy logger instance
         */
        ~Logger();

        /**
         * sets an appender for the logger
         */
        status_t setAppender(Appender& appender);

        /**
         * removes the appender identified by name
         */
        status_t removeAppender(Appender& appender);

        /**
         * opens the logger
         */
        status_t open();

        /**
         * print trace message to the logger.
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t trace(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * print debug message to the logger.
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t debug(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * print info message to the logger.
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t info(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * print warn message to the logger.
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t warn(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * print error message to the logger.
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t error(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * print log message with given level to the logger.
         * @param level of the lov message
         * @param tag that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         */
        status_t log(const LoggerLevel level, const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...);

        /**
         * close the logger
         */
        status_t close();

    private:
        /**
         * print log message with given level to the logger.
         * @param level of the lov message
         * @param tag that will be logged
         * @param message that will be logged
         * @param file name of the log call
         * @param line number of the log call
         * @param msgFormat of the log message
         * @param args variable argument list
         */
        status_t vlog(const LoggerLevel level, const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, va_list args);

    private:
        int32_t mId;
        Appender* mAppenders[LOGGER_APPENDER_MAX];
        bool_t mOpen;
    };

    /*
     * Implementation LoggerMessage
     */

    // id
    inline void LoggerMessage::setId(const int32_t id)
    {
        mId = id;
    }
    inline int32_t LoggerMessage::getId() const
    {
        return mId;
    }

    // timestamp
    inline void LoggerMessage::setTimestamp(const uint64_t timestamp)
    {
        mTimestamp = timestamp;
    }
    inline uint64_t LoggerMessage::getTimestamp() const
    {
        return mTimestamp;
    }

    // threadId
    inline void LoggerMessage::setThreadId(const uint_t threadId)
    {
        mThreadId = threadId;
    }
    inline uint_t LoggerMessage::getThreadId() const
    {
        return mThreadId;
    }

    // level
    inline void LoggerMessage::setLevel(const LoggerLevel level)
    {
        mLevel = level;
    }
    inline LoggerLevel LoggerMessage::getLevel() const
    {
        return mLevel;
    }

    // tag
    inline void LoggerMessage::setTag(const char_t* tag)
    {
        mTag = tag;
    }
    inline const char_t* LoggerMessage::getTag() const
    {
        return mTag;
    }

    // filename
    inline void LoggerMessage::setFile(const char_t* file)
    {
        mFile = file;
    }
    inline const char_t* LoggerMessage::getFile() const
    {
        return mFile;
    }

    // line
    inline void LoggerMessage::setLine(const int32_t line)
    {
        mLine = line;
    }
    inline int32_t LoggerMessage::getLine() const
    {
        return mLine;
    }

    // message
    inline void LoggerMessage::setMessage(const char_t* message)
    {
        mMessage = message;
    }

    inline const char_t* LoggerMessage::getMessage() const
    {
        return mMessage;
    }

    /*
     * Implementation Logger
     */

    inline status_t Logger::trace(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(CLL_TRACE, tag, file, line, msgFormat, args);
        va_end(args);
        return status;
    }

    inline status_t Logger::debug(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(CLL_DEBUG, tag, file, line, msgFormat, args);
        va_end(args);
        return status;
    }

    inline status_t Logger::info(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(CLL_INFO, tag, file, line, msgFormat, args);
        va_end(args);
        return status;
    }

    inline status_t Logger::warn(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(CLL_WARN, tag, file, line, msgFormat, args);
        return status;
    }

    inline status_t Logger::error(const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(CLL_ERROR, tag, file, line, msgFormat, args);
        va_end(args);
        return status;
    }

    inline status_t Logger::log(LoggerLevel level, const char_t* tag, const char_t* file, const int32_t line, const char_t* msgFormat, ...)
    {
        va_list args;
        va_start(args, msgFormat);
        status_t status = vlog(level, tag, file, line, msgFormat, args);
        va_end(args);
        return status;
    }
}

#endif /* CAPU_LOGGER_H */
