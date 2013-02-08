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

#include "container/HashTest.h"
#include "capu/util/Guid.h"

namespace capu
{
    HashTest::HashTest()
    {
    }

    HashTest::~HashTest()
    {
    }

    void HashTest::SetUp()
    {
    }

    void HashTest::TearDown()
    {
    }

    TEST_F(HashTest, HashInt32)
    {
        int32_t intVal = 42;
        EXPECT_EQ(10u, CapuDefaultHashFunction::Digest(intVal, 4));
    }

    enum Someenum
    {
        MEMBER0 = 1234,
        MEMBER1
    };

    TEST_F(HashTest, HashEnum)
    {
        Someenum val = MEMBER0;

        // we assume that we hash the enum identically to a primitive (int) with the same value
        EXPECT_EQ(CapuDefaultHashFunction::Digest(static_cast<uint_t>(MEMBER0), 4), CapuDefaultHashFunction::Digest(val, 4));
    }

    TEST_F(HashTest, HashGuid)
    {
        Guid guid;
        guid.toString(); // change internal state
        Guid guid2(guid);

        EXPECT_EQ(CapuDefaultHashFunction::Digest(guid, 4), CapuDefaultHashFunction::Digest(guid2, 4));
    }
}

