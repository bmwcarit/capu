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

#ifndef CAPU_GENERIC_DEBUG_H
#define CAPU_GENERIC_DEBUG_H

#include <assert.h>

namespace capu
{
    namespace generic
    {
        class Debug
        {
        public:
            static void Assert(bool_t condition);
        };

        inline
        void
        Debug::Assert(bool_t condition)
        {
            UNUSED(condition);
#ifdef _DEBUG
            assert(condition);
#endif
        }
    }
}


#endif //CAPU_GENERIC_DEBUG_H
