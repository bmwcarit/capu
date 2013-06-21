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

#include "SynchronizedTest.h"
#include "capu/os/Thread.h"
#include "capu/util/TestUtils.h"

namespace capu
{
    typedef Synchronized<List<uint32_t> >  SynchronizedList;

    class SynchronizedTestRunnable: public Runnable
    {
    public:
        SynchronizedTestRunnable(SynchronizedList& list)
            : mList(list)
        {

        }

        void run()
        {
            RELEASE_SYNC_CALL(ready);
            for (uint32_t i = 0; i < 100; ++i)
            {
                mList->pop_back();
            }
        }

        REGISTER_SYNC_CALL(ready);

    private:

        SynchronizedList& mList;
    };


    TEST_F(SynchronizedTest, LockUnlock)
    {
        SynchronizedList syncList;
        SynchronizedTestRunnable testRunnable(syncList);


        for (uint32_t i = 0; i < 100; ++i)
        {
            syncList->push_back(1);
        }

        Thread thread;
        thread.start(testRunnable);
        WAIT_FOR_SYNC_CALL(testRunnable, ready);
        for (uint32_t i = 0; i < 100; ++i)
        {
            syncList->push_back(1);
        }

        thread.join();

        // Maybe we need a special class for iteratable objects since the iterator is not locked
        List<uint32_t>::Iterator iter = syncList->begin();
        const List<uint32_t>::Iterator end = syncList->end();

        uint32_t result = 0;
        for (; iter != end; ++iter)
        {
            result += *iter;
        }

        EXPECT_EQ(100, result);
    }
}
