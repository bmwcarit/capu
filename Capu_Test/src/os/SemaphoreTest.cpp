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

#include <gtest/gtest.h>
#include "capu/os/Semaphore.h"
#include "capu/os/Time.h"

TEST(Semaphore, singleThreadTest)
{
    capu::Semaphore sem;
    EXPECT_EQ(capu::CAPU_OK, sem.release());
    EXPECT_EQ(capu::CAPU_OK, sem.release());
    EXPECT_EQ(capu::CAPU_OK, sem.release());
    EXPECT_EQ(capu::CAPU_OK, sem.aquire());
    EXPECT_EQ(capu::CAPU_OK, sem.aquire());
    EXPECT_EQ(capu::CAPU_OK, sem.aquire());
    EXPECT_EQ(capu::CAPU_ETIMEOUT, sem.tryAquire(1));
}

TEST(Semaphore, timeoutTest)
{
    capu::Semaphore sem;
    uint64_t start = capu::Time::GetMilliseconds();
    EXPECT_EQ(capu::CAPU_ETIMEOUT, sem.tryAquire(100));
    uint64_t dur = capu::Time::GetMilliseconds() - start;
    EXPECT_GE(dur, 50u);
}
