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

#include "capu/util/LogContext.h"

namespace capu
{
    LogContext::LogContext(const String& name, const String& id)
        : m_contextName(name)
        , m_contextId(id)
        , m_enabled(true)
        , m_logLevel(static_cast<int32_t>(LL_ERROR))
        , m_data(0)
    {
    }
}
