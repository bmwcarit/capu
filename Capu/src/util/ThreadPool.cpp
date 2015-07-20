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

#include "capu/util/ThreadPool.h"

#include "capu/util/ScopedLock.h"

const capu::uint32_t capu::ThreadPool::MAX_THREAD_POOL_THREADS = 64;

capu::ThreadPool::ThreadPool(const capu::uint32_t size)
    : mClosed(false)
    , mCloseRequested(false)
{
    const capu::uint32_t poolSize = size < MAX_THREAD_POOL_THREADS ? size : MAX_THREAD_POOL_THREADS;

    // create the workers
    for (capu::uint32_t i = 0; i < poolSize; i++)
    {
        capu::ThreadPool::PoolWorkerPtr t(new capu::ThreadPool::PoolWorker(*this));
        if (t->isValid())
        {
            mWorkerList.insert(t);
        }
        else
        {
            // abort on first thread creation error
            break;
        }
    }
}

capu::ThreadPool::~ThreadPool()
{
    // wait for all jobs to be finished
    close();
}

capu::status_t capu::ThreadPool::add(capu::SmartPointer<capu::Runnable> runnable)
{
    if (runnable.get() == NULL || mClosed || mCloseRequested)
    {
        // no adding to closed queue
        return CAPU_ERROR;
    }

    ScopedMutexLock lock(mMutex);
    status_t result = mRunnableQueue.push(runnable);
    mCV.signal();
    return result;
}

capu::status_t capu::ThreadPool::close(bool cancelThreads)
{
    {
        ScopedMutexLock lock(mMutex);
        mCloseRequested = true;
        mCV.broadcast(); // let all worker wake up to terminate
    }

    // request all workers to cancel
    if (cancelThreads)
    {
        capu::List<PoolWorkerPtr>::Iterator it = mWorkerList.begin();
        while (it != mWorkerList.end())
        {
            if (*it)
            {
                (*it)->cancel();
            }
            ++it;
        }
    }

    // wait for all workers to get finished
    status_t result = capu::CAPU_OK;
    capu::List<PoolWorkerPtr>::Iterator it = mWorkerList.begin();
    while (it != mWorkerList.end())
    {
        if (*it)
        {
            result = (*it)->join(); // wait
            if (result != capu::CAPU_OK)
            {
                return result;
            }
        }
        ++it;
    }
    mClosed = true;
    return result;
}

capu::uint_t capu::ThreadPool::getSize() const
{
    return mWorkerList.size();
}

bool capu::ThreadPool::isClosed() const
{
    return mClosed;
}

capu::ThreadPool::PoolRunnable::PoolRunnable(ThreadPool& pool)
    : mPool(pool), mCurrentRunnable(NULL)
{
}

void capu::ThreadPool::PoolWorker::cancel()
{
    mThread.cancel(); // pool runnable

    mPoolRunnable.cancelCurrentRunnable();
}

void capu::ThreadPool::PoolRunnable::cancelCurrentRunnable()
{
    ScopedMutexLock lock(mCurrentRunnableMutex);
    if (mCurrentRunnable != NULL)
    {
        mCurrentRunnable->cancel();
    }
}

void capu::ThreadPool::PoolRunnable::run()
{
    while (!isCancelRequested())
    {
        status_t result;
        SmartPointer<Runnable> r = NULL;
        {
            ScopedMutexLock lock(mPool.mMutex);
            while (mPool.mRunnableQueue.empty() && !mPool.isClosed())
            {
                if (mPool.mCloseRequested)
                {
                    // if queue is empty and close was requested
                    // set closed flag and exit
                    mPool.mClosed = true;
                    break;
                }
                mPool.mCV.wait(&mPool.mMutex); // block until a job is available
            }
            if (mPool.isClosed())
            {
                break;
            }
            result = mPool.mRunnableQueue.pop(&r);
        }
        if (result == CAPU_OK)
        {
            mCurrentRunnableMutex.lock();
            mCurrentRunnable = r.get();
            mCurrentRunnableMutex.unlock();
            if (mCurrentRunnable != NULL)
            {
                mCurrentRunnable->run();
                mCurrentRunnableMutex.lock();
                mCurrentRunnable = NULL;
                mCurrentRunnableMutex.unlock();
            }
        }
    }
}

capu::ThreadPool::PoolWorker::PoolWorker(capu::ThreadPool& pool)
    : mPool(pool)
    , mPoolRunnable(mPool)
{
    mValid = mThread.start(mPoolRunnable) == CAPU_OK;
}

capu::ThreadPool::PoolWorker::~PoolWorker()
{
}

bool capu::ThreadPool::PoolWorker::isValid() const
{
    return mValid;
}

capu::status_t capu::ThreadPool::PoolWorker::join()
{
    return mThread.join();
}
