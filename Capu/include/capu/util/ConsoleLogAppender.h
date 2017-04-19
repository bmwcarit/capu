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

#ifndef CAPU_CONSOLELOGAPPENDER_H
#define CAPU_CONSOLELOGAPPENDER_H

#include "capu/util/LogAppenderBase.h"
#include "capu/util/LogMessage.h"
#include "capu/os/LightweightMutex.h"

namespace capu
{

    /**
     * Logs messages directly to console
     */
    class ConsoleLogAppender: public capu::LogAppenderBase
    {
    public:

        /**
         * @name log implementation of ConsoleLogAppender
         * @see LogAppenderBase
         */
        virtual void logMessage(const LogMessage& logMessage) override;

        virtual ~ConsoleLogAppender();
    private:

        /**
         * Mutex to make messages to console atomic
         */
        static LightweightMutex m_logMutex;
    };
}

#endif // CAPU_CONSOLELOGAPPENDER_H
