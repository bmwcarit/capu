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

#ifndef CAPU_ASYNCHRONOUSLOGGER_H
#define CAPU_ASYNCHRONOUSLOGGER_H

#include "capu/util/Logger.h"
#include "capu/os/Thread.h"
#include "capu/container/BlockingQueue.h"
#include "capu/util/ILogAppender.h"
#include "capu/util/LogMessage.h"

namespace capu
{
     /**
     * Outputs LogMessages in a separate Thread
     */
    class AsynchronousLogger: public Logger, private Runnable
    {
    public:
        
        /**
         * Constructor of AsynchronousConsoleLogAppender
         * The output thread is started immediately 
         */
        AsynchronousLogger(ILogAppender& appender);

        /**
         * Destructor of AsynchronousConsoleLogAppender
         */
        virtual ~AsynchronousLogger();

        /**
         * @name Asynchronous implementation of log method
         * @see Logger
         */
        virtual void log(const LogMessage& message) override;

    private:

        typedef BlockingQueue<LogMessage> LogQueue;

        /**
         * Thread in which the message are logged
         */
        Thread m_loggerThread;

        /**
         * Queue with messages to log
         */
       LogQueue m_logQueue;

        /**
         * Internal run method for the thread
         */
        virtual void run() override;

    };
}

#endif // CAPU_ASYNCHRONOUSLOGGER_H
