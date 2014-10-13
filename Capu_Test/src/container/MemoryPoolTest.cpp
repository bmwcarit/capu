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
#include "capu/container/MemoryPool.h"
#include "capu/Config.h"

TEST(MemoryPoolTest, GetElementsUntilFull)
{
    capu::MemoryPool<capu::uint32_t, 3> m_pool;

    capu::uint32_t* first = m_pool.getEntry();
    capu::uint32_t* second = m_pool.getEntry();
    capu::uint32_t* third = m_pool.getEntry(); 
    capu::uint32_t* fourth = m_pool.getEntry();

    *first = 1;
    *second = 2;
    *third = 3;

    EXPECT_TRUE(0 != first);
    EXPECT_EQ(1u, *first);
    EXPECT_EQ(2u, *second);
    EXPECT_EQ(3u, *third);

    EXPECT_TRUE(0 != second);
    EXPECT_TRUE(0 != third);
    EXPECT_TRUE(0 == fourth);
}

TEST(MemoryPoolTest, FillPoolAndRelease)
{
    capu::MemoryPool<capu::uint32_t, 3> m_pool;

    capu::uint32_t* first = m_pool.getEntry();
    capu::uint32_t* second = m_pool.getEntry();
    capu::uint32_t* third = m_pool.getEntry();

    m_pool.releaseEntry(second);

    second = m_pool.getEntry();
    *second = 2;

    EXPECT_TRUE(0 != second);
    EXPECT_EQ(2u, *second);

    m_pool.releaseEntry(first);
    m_pool.releaseEntry(third);

    third = m_pool.getEntry();
    *third = 3;
    
    EXPECT_TRUE(0 != third);
    EXPECT_EQ(2u, *second);
    EXPECT_EQ(3u, *third);

    first = m_pool.getEntry();
    *first = 1;

    EXPECT_TRUE(0 != first);
    EXPECT_EQ(2u, *second);
    EXPECT_EQ(3u, *third);
    EXPECT_EQ(1u, *first);

    capu::uint32_t* fourth = m_pool.getEntry();

    EXPECT_TRUE(0 == fourth);

}

