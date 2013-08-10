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

#ifndef CAPU_ILOGAPPENDER_H
#define CAPU_ILOGAPPENDER_H

namespace capu
{
    class LogMessage;

    /**
     * Base class for LogAppenders. Can be used to implement additional appenders
     */
    class ILogAppender
    {
    public:

        /**
         * Logs the given message
         * @param logMessage to log
         */
        virtual void log(const LogMessage& logMessage) = 0;
    };

}


#endif // CAPU_ILOGAPPENDER_H