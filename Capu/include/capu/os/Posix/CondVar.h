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

#ifndef CAPU_UNIXBASED_CONDVAR_H
#define CAPU_UNIXBASED_CONDVAR_H

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "capu/os/Time.h"

namespace capu
{
    namespace posix
    {

        class CondVar
        {
        public:
            CondVar()
            {
                pthread_condattr_init(&mCondAttr);
                pthread_cond_init(&mCond, &mCondAttr);
            }

            ~CondVar()
            {
                pthread_cond_destroy(&mCond);
                pthread_condattr_destroy(&mCondAttr);
            }

            status_t signal()
            {
                if (pthread_cond_signal(&mCond) == 0)
                {
                    return CAPU_OK;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }


            status_t broadcast()
            {
                if (pthread_cond_broadcast(&mCond) == 0)
                {
                    return CAPU_OK;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }

            status_t wait(capu::Mutex* mutex, uint32_t timeoutMillis)
            {
                if (mutex == NULL)
                {
                    return CAPU_EINVAL;
                }

                if (timeoutMillis != 0)
                {
                    // use platform independent GetMilliseconds() function here because clock_gettime is not always available
                    uint64_t endTime = Time::GetMilliseconds() + timeoutMillis;
                    struct timespec timeout;
                    timeout.tv_sec = endTime / 1000;
                    timeout.tv_nsec = (endTime % 1000) * 1000000;

                    int32_t ret = pthread_cond_timedwait(&mCond, &mutex->mLock, &timeout);

                    switch (ret)
                    {
                    case 0:
                        return CAPU_OK;
                    case ETIMEDOUT:
                        return CAPU_ETIMEOUT;
                    default:
                        return CAPU_ERROR;
                    }
                }
                else
                {
                    if (pthread_cond_wait(&mCond, &mutex->mLock) == 0)
                    {
                        return CAPU_OK;
                    }
                    else
                    {
                        return CAPU_ERROR;
                    }
                }
            }
        private:
            pthread_cond_t mCond;
            pthread_condattr_t mCondAttr;
        };
    }
}
#endif // CAPU_UNIXBASED_CONDVAR_H
