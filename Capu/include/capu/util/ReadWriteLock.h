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

#ifndef CAPU_READWRITELOCK_H
#define CAPU_READWRITELOCK_H

#include "capu/os/CondVar.h"
#include "capu/os/LightweightMutex.h"
#include "capu/util/ScopedLock.h"

namespace capu
{
    /**
     * Models a lock that permits multiple readers, but only one writer.
     */
    class ReadWriteLock
    {
    public:
        /**
         * Creates a new ReadWriteLock.
         */
        ReadWriteLock();

        /**
         * Aquires a read lock. Blocks until the read was permitted.
         */
        void lockRead();

        /**
         * Frees a formerly taken read-lock.
         */
        void unlockRead();

        /**
         * Aquires an exclusive write lock. Blocks until all readers have returned the lock.
         */
        void lockWrite();

        /**
         * Frees a formerly taken write-lock.
         */
        void unlockWrite();

    private:
        uint32_t m_readerCount;
        CondVar m_readerLeftArea;
        LightweightMutex m_readerLock;
        LightweightMutex m_writerLock;
    };

    inline ReadWriteLock::ReadWriteLock()
        : m_readerCount(0)
    {
    }

    inline void ReadWriteLock::lockRead()
    {
        ScopedLightweightMutexLock lockWriter(m_writerLock); // will block reader if writer is active
        ScopedLightweightMutexLock lockReader(m_readerLock);
        ++m_readerCount;
    }

    inline void ReadWriteLock::unlockRead()
    {
        ScopedLightweightMutexLock lockReader(m_readerLock);
        if (m_readerCount > 0)
        {
            // defensive style to avoid double unlock
            --m_readerCount;
        }
        m_readerLeftArea.broadcast(); // inform writer that readercount changed
    }

    inline void ReadWriteLock::unlockWrite()
    {
        m_writerLock.unlock();
    }

    inline void ReadWriteLock::lockWrite()
    {
        m_writerLock.lock();
        ScopedLightweightMutexLock lockReader(m_readerLock);
        while (m_readerCount > 0)
        {
            // block writer until all readers are finished
            m_readerLeftArea.wait(m_readerLock);
        }
    }
}

#endif // CAPU_READWRITELOCK_H
