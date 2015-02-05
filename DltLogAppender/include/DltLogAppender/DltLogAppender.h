/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_DLTLOGAPPENDER_H
#define CAPU_DLTLOGAPPENDER_H

#include "capu/util/ILogAppender.h"
#include "capu/util/LogMessage.h"
#include "capu/os/Mutex.h"

#include "DltLogAppender/DltAdapter.h"

namespace capu
{
    //forward declaration
    class DltAdapter;

    /**
     * Logs messages via dlt
     */
    class DltLogAppender: public capu::ILogAppender
    {
    public:

        /**
         * Constructor
         */
        DltLogAppender();

        /**
         * Destructor
         */
        virtual ~DltLogAppender();

        /**
         * @param message log message passed to dlt adapter
         * @see ILogAppender
         */
        virtual void log(const capu::LogMessage& message);

    private:

        /**
         * Mutex to make messages to console atomic
         */
        capu::Mutex m_logMutex;


        /**
         * The dlt adapter singleton
         */
        capu::DltAdapter* m_dltAdapter;
    };
}

#endif // CAPU_DLTLOGAPPENDER_H
