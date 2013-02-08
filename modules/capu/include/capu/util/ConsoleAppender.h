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

#ifndef CAPU_CONSOLEAPPENDER_H
#define CAPU_CONSOLEAPPENDER_H

#include "capu/util/Appender.h"
#include "capu/util/Logger.h"

namespace capu
{
    /**
     * Appender for logging to console.
     */
    class ConsoleAppender : public capu::Appender
    {
    public:

        virtual ~ConsoleAppender() {}

        capu::status_t open()
        {
            return capu::CAPU_OK;
        }

        capu::status_t log(capu::LoggerMessage& message)
        {
            if (message.getLevel() >= mLvl)
            {
                printf("%s:%d %s, %s\n", message.getFile(), message.getLine(), message.getTag(), message.getMessage());
            }
            return capu::CAPU_OK;
        }

        capu::status_t close()
        {
            return capu::CAPU_OK;
        }
    };
}

#endif //CAPU_CONSOLEAPPENDER_H
