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

#ifndef CAPU_TESTUTILS_H
#define CAPU_TESTUTILS_H

#include <gmock/gmock.h>
#include <capu/os/CondVar.h>
#include <capu/os/Mutex.h>

namespace capu
{
    /**
     * Special GMock actionf or releasing condition variables on mock calls
     */
    ACTION_P(ReleaseSyncCall, CondVar)
    {
        CondVar->signal();
    }

    /**
     * Helper macro to concatenate strings
     */
#define CONCAT(a, b) a##b

    /**
     * Create local helper struct to handle condition variable and mutex
     */
#define  REGISTER_SYNC_CALL(name)                 \
    struct CONCAT(name, Class)                    \
    {                                             \
        capu::Mutex    mutex;                     \
        capu::CondVar condVar;                    \
    };                                            \
    CONCAT(name, Class) CONCAT(name ,LockHandler);

    /**
     * Returns the full name of a condition variable
     */
#define SYNC_CALL_CONDVAR(name)\
    CONCAT(name, LockHandler).condVar

    /**
     * Returns the full name of a mutex
     */
#define SYNC_CALL_LOCK(name)\
    CONCAT(name, LockHandler).mutex

    /**
     * Returns the condition variable of a caller
     */
#define SYNC_CALL_CONDVAR_OF_CALLER(callee, name)\
    callee.SYNC_CALL_CONDVAR(name)

    /**
     * Returns the lock variable of a caller
     */
#define SYNC_CALL_LOCK_OF_CALLER(callee, name)\
    callee.SYNC_CALL_LOCK(name)

    /**
     * Releases a condition variable
     */
#define RELEASE_SYNC_CALL(name)\
    SYNC_CALL_CONDVAR(name).signal();

    /**
     * Waits for a given call on a given caller
     */
#define WAIT_FOR_SYNC_CALL(callee, function)\
    SYNC_CALL_CONDVAR_OF_CALLER(callee, function).wait(&SYNC_CALL_LOCK_OF_CALLER(callee, function));

}

#endif // CAPU_TESTUTILS_H