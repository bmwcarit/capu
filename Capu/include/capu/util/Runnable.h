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

#ifndef CAPU_RUNNABLE_H
#define CAPU_RUNNABLE_H

#include "capu/Config.h"

#include "capu/os/Atomic.h"


namespace capu
{
    /**
     * Interface for classes that can be run by threads.
     */
    class Runnable
    {
    public:

        Runnable() : mCancel(false) {}

        virtual ~Runnable() {}

        /**
         * Thread Execution Function
         */
        virtual void run() = 0;

        /**
         * Sets the cancel flag to true.
         *
         * The runnable can (should) test this flag periodically in its run method.
         */
        virtual void cancel()
        {
            mCancel = true;
        }

        /**
         * Resets the cancel flag to false.
         *
         * Allows a runnable to potentially be run again (test if cancel requested does not fail).
         */
        void resetCancel()
        {
            mCancel = false;
        }

        /**
         * Checks if the cancel flag is set.
         *
         * @return TRUE if the runnable should exit, FALSE otherwise
         */
        bool isCancelRequested()
        {
            return mCancel;
        }

    private:
        Atomic<bool> mCancel;
    };
}

#endif /* CAPU_RUNNABLE_H */
