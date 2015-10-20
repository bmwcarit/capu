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

#ifndef CAPU_SIGNAL_H
#define CAPU_SIGNAL_H

#include "capu/Config.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(Signal)


namespace capu
{
    /**
     * Signal handler.
     */
    class Signal: private os::arch::Signal
    {
    public:

        /// Enum of possible signals.
        //using os::arch::ESignal;

        /// Registers a signal handler.
        /** @param sig The signal.
         *  @param func Signal handler function, which is called when the signal happens.
         *  @return Returns the previous installed signal handler. */
        static void (*signal(ESignal sig, void (*func)(int32_t)))(int32_t);

        /// Generates a signal.
        /** @param sig The signal.
         *  @return Returns CAPU_OK when successful, and CAPU_ERROR otherwise. */
        static status_t raise(ESignal sig);
    };

    inline void (*Signal::signal(ESignal sig, void (*func)(int32_t)))(int32_t)
    {
        return os::arch::Signal::signal(sig, func);
    }

    inline status_t Signal::raise(ESignal sig)
    {
        return os::arch::Signal::raise(sig);
    }
}

#endif //CAPU_SIGNAL_H
