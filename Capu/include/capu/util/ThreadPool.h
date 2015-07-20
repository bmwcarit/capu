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

#ifndef CAPU_THREADPOOL_H
#define CAPU_THREADPOOL_H

#include "capu/Config.h"
#include "capu/container/List.h"
#include "capu/container/Queue.h"
#include "capu/os/CondVar.h"
#include "capu/os/Mutex.h"
#include "capu/os/Thread.h"
#include "capu/util/Runnable.h"
#include "capu/util/SmartPointer.h"

namespace capu
{
    /**
     * Represents a set of threads that can be used to run Runnables.
     */
    class ThreadPool
    {
    public:

        /**
         * Maximum number of threads in a thread pool
         */
        static const uint32_t MAX_THREAD_POOL_THREADS;

        /**
         * creates a new threadpool instance.
         * @param size amounts of threads. Default value is 5.
         */
        ThreadPool(const uint32_t size = 5);

        /**
         * destructor.
         */
        virtual ~ThreadPool();

        /**
         * Adds a runnable to the threadpool.
         * @param runnable The runnable which should be executed by the threadpool
         */
        status_t add(SmartPointer<Runnable> runnable);

        /**
         * Waits until every thread has been terminated.
         * @param cancelThreads set the cancel flag on all workers before waiting
         */
        status_t close(bool cancelThreads = false);

        /**
         * Checks if the threadpool is closed, which means that no new work items can be added.
         * @return True if the pool is closed.
         */
        bool isClosed() const;

        /**
         * Returns the number of threads used by the threadpool
         * @return The number of threads used by the threadpool.
         */
        uint_t getSize() const;

    private:

        class PoolRunnable : public Runnable
        {
        public:
            PoolRunnable(ThreadPool& pool);
            void run();

            void cancelCurrentRunnable();

        private:
            ThreadPool& mPool;
            Mutex mCurrentRunnableMutex;
            Runnable* mCurrentRunnable;

            PoolRunnable operator=(const PoolRunnable pr);
        };

        class PoolWorker
        {
        public:
            PoolWorker(ThreadPool& pool);
            ~PoolWorker();
            status_t join();
            void cancel();
            bool isValid() const;

        private:
            ThreadPool& mPool;
            PoolRunnable mPoolRunnable;
            Thread mThread;

            bool mValid;

            PoolWorker operator=(const PoolWorker pw);
        };

        typedef SmartPointer<PoolWorker> PoolWorkerPtr;

        bool mClosed;
        bool mCloseRequested;
        Queue<SmartPointer<Runnable> > mRunnableQueue;
        CondVar mCV;
        Mutex mMutex;
        List<PoolWorkerPtr> mWorkerList;
    };
}

#endif // CAPU_THREADPOOL_H
