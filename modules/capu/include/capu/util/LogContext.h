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

namespace capu
{
    class LogContext
    {
    public:
        LogContext(const String& name);

        const String& getContextName() const;
    private:
        const String m_contextName;
    };

    inline
    const String& 
    LogContext::getContextName() const
    {
        return m_contextName;
    }
}

#endif // CAPU_LOGCONTEXT_H