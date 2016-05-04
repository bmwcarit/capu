/*
 * Copyright (C) 2016 BMW Car IT GmbH
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

#ifndef CAPU_WINDOWS_ATOMICTYPES_H
#define CAPU_WINDOWS_ATOMICTYPES_H

#include <Windows.h>
#include "capu/Config.h"

namespace capu
{
    namespace os
    {
        class AtomicBool
        {
        public:
            void store(bool value);

            bool load();

        private:
            volatile LONG mValue;
        };

        inline
        void AtomicBool::store(bool value)
        {
            InterlockedExchange(&mValue, static_cast<LONG>(value));
        }

        inline
        bool AtomicBool::load()
        {
            return (InterlockedCompareExchange(&mValue,0,0) != 0);
        }
    }
}

#endif // CAPU_WINDOWS_X86_32_ATOMICTYPES_H
