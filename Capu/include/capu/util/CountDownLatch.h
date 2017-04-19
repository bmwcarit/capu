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

#ifndef CAPU_COUNTDOWNLATCH_H
#define CAPU_COUNTDOWNLATCH_H

#include "capu/Config.h"
#include "capu/os/LightweightMutex.h"
#include "capu/os/CondVar.h"
#include "capu/util/ScopedLock.h"

namespace capu
{
    /**
     * Synchronization helper that let's multiple threads wait until a batch of operations succeeded.
     */
    class CountDownLatch
    {
    public:
        /**
         * Creates a new instance of a count down latch.
         * @param count The amount of operations that must succeed (must call countDown) before await returns successfully.
         */
        CountDownLatch(const uint_t count);

        /**
         * Destructor.
         */
        virtual ~CountDownLatch();

        /**
         * Decrements the latch count. The the count reaches 0,
         * all waiting thread will get released. Also, all threads in the future which
         * call await will return successfully from the call.
         *
         * @return CAPU_OK if the count was decreased successfully. CAPU_ERROR if the count was 0 already.
         */
        status_t countDown();

        /**
         * Blocks the calling thread until the latch count reaches 0. If the count is 0 before the call, the call returns immediately.
         * @param timeoutMillis Optional timeout (default value is 0, which means to block forever).
         * @return CAPU_OK if the latch count is 0. CAPU_ETIMEOUT if a timeout occurred while waiting for the latch.
         */
        status_t await(const uint32_t timeoutMillis = 0);

    private:
        LightweightMutex mCountLock;
        CondVar mCountChanged;
        uint_t mCount;
    };

    inline CountDownLatch::CountDownLatch(const uint_t count)
        : mCount(count)
    {
    }

    inline CountDownLatch::~CountDownLatch()
    {
    }

    inline status_t CountDownLatch::countDown()
    {
        status_t retVal = CAPU_OK;
        ScopedLightweightMutexLock lock(mCountLock);
        if (mCount > 0)
        {
            --mCount;
            if (mCount == 0)
            {
                mCountChanged.broadcast();
            }
        }
        else
        {
            retVal = CAPU_ERROR;
        }
        return retVal;
    }

    inline status_t CountDownLatch::await(const uint32_t timeoutMillis)
    {
        ScopedLightweightMutexLock lock(mCountLock);
        while (mCount > 0)
        {
            status_t retVal = mCountChanged.wait(mCountLock, timeoutMillis);
            if (retVal != CAPU_OK)
            {
                return retVal;
            }
        }
        return CAPU_OK;
    }
}

#endif // CAPU_COUNTDOWNLATCH_H
