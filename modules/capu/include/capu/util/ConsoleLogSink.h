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

#ifndef CAPU_CONSOLELOGSINK_H
#define CAPU_CONSOLELOGSINK_H

#include "capu/util/LogSink.h"
#include "capu/util/LogMessage.h"
#include "capu/os/Mutex.h"

namespace capu
{

    class ConsoleLogSink: public LogSink
    {
    public:
        virtual void log(const LogMessage& logMessage);
    private:
        Mutex m_logMutex;
    };
}

#endif // CAPU_CONSOLELOGSINK_H