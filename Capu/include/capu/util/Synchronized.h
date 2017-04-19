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

#ifndef CAPU_SYNCHRONIZED_H
#define CAPU_SYNCHRONIZED_H


#include "capu/os/LightweightMutex.h"

namespace capu
{

    /**
     * Encapsulates the access to a given Object and
     */
    template<typename T>
    class AutoLockReleaser
    {
    public:
        AutoLockReleaser(T& data, LightweightMutex& lock)
            : mData(data)
            , mLock(lock)
        {
            mLock.lock();
        }

        ~AutoLockReleaser()
        {
            mLock.unlock();
        }

        T* operator->()
        {
            return &mData;
        }

    private:
        T& mData;
        LightweightMutex& mLock;
    };

    /**
     * Allows synchronized access to the given Type
     */
    template<typename T>
    class Synchronized
    {
    public:
        /**
         * Allows access to the synchronized object with auto releasing after call
         * @return an AutoLockReleaser which unlocks the object automatically
         */
        AutoLockReleaser<T> operator->();

    private:
        LightweightMutex mMutex;
        T mSynchronizedObject;
    };

    template<typename T>
    inline
    AutoLockReleaser<T> Synchronized<T>::operator->()
    {
        return AutoLockReleaser<T>(mSynchronizedObject, mMutex);
    }
}

#endif
