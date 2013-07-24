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
#include "capu/os/EnvironmentVariables.h"

TEST(EnvironmentVariables, getAllTest)
{
    capu::EnvironmentVariables myEnv;
    const capu::HashTable<capu::String, capu::String>& env = myEnv.getAll();
    EXPECT_TRUE(env.count() > 0);
}

TEST(EnvironmentVariables, getTest)
{
    capu::EnvironmentVariables myEnv;
    capu::String tmp;
    capu::HashTable<capu::String, capu::String>::Iterator it = myEnv.getAll().begin();
    const capu::HashTable<capu::String, capu::String>::Iterator end = myEnv.getAll().end();

    for(; it != end; ++it)
    {
        EXPECT_TRUE(myEnv.get(it->key, tmp));
        EXPECT_EQ(tmp, it->value);
    }
}
