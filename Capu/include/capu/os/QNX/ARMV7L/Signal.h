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

#ifndef CAPU_QNX_ARMV7L_SIGNAL_H
#define CAPU_QNX_ARMV7L_SIGNAL_H

#include <capu/os/QNX/Signal.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Signal: private os::Signal
            {
            public:
                using os::Signal::signal;
                using os::Signal::raise;
            };
        }
    }
}

#endif // CAPU_QNX_ARMV7L_SIGNAL_H
