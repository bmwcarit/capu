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

#include "capu/util/Guid.h"
#include "capu/container/Hash.h"
#include "gtest/gtest.h"

namespace capu
{
    TEST(HashTest, HashInt32)
    {
        int32_t intVal = 42;
        uint32_t expected32bitHash = 10u;
        EXPECT_EQ(expected32bitHash, CapuDefaultHashFunction<32>::Digest(intVal, 4));

        uint64_t expected64bitHash = 10u;
        EXPECT_EQ(expected64bitHash, CapuDefaultHashFunction<64>::Digest(intVal, 4));
    }

    TEST(HashTest, HashInt64)
    {
        int64_t intVal = 858918934591ll;
        uint32_t expected32bitHash = 11u;
        EXPECT_EQ(expected32bitHash, CapuDefaultHashFunction<32>::Digest(intVal, 4));

        uint64_t expected64bitHash = 12u;
        EXPECT_EQ(expected64bitHash, CapuDefaultHashFunction<64>::Digest(intVal, 4));
    }

    TEST(HashTest, HashPointer)
    {
        uint8_t* bytePtr = reinterpret_cast<uint8_t*>(0xabcdef);
        uint32_t expected32bitHash = 47u;
        EXPECT_EQ(expected32bitHash, CapuDefaultHashFunction<32>::Digest(bytePtr, 6));

        uint64_t expected64bitHash = 47u;
        EXPECT_EQ(expected64bitHash, CapuDefaultHashFunction<64>::Digest(bytePtr, 6));
    }

    enum Someenum
    {
        MEMBER0 = 1234,
        MEMBER1
    };

    TEST(HashTest, HashEnum)
    {
        Someenum val = MEMBER0;

        // we assume that we hash the enum identically to a primitive (int) with the same value
        EXPECT_EQ(CapuDefaultHashFunction<>::Digest(static_cast<uint_t>(MEMBER0), 4), CapuDefaultHashFunction<>::Digest(val, 4));
    }

    TEST(HashTest, HashGuid)
    {
        Guid guid;
        guid.toString(); // change internal state
        Guid guid2(guid);

        EXPECT_EQ(CapuDefaultHashFunction<>::Digest(guid, 4), CapuDefaultHashFunction<>::Digest(guid2, 4));
    }

    TEST(HashTest, requestOneBitHashValue)
    {
        char dataToHash[128];

        uint64_t hashValue = HashCalculator<uint64_t>::Hash(dataToHash, 128, 1);
        uint64_t zeroMap = 0xffffffffffffffffull << 1;
        uint64_t shouldBeZero = zeroMap & hashValue;
        EXPECT_EQ(0u, shouldBeZero);
    }

    TEST(HashTest, request32BitHashValueIn64BitType)
    {
        char dataToHash[128] = {1};

        uint64_t hashValue = HashCalculator<uint64_t>::Hash(dataToHash, 128, 32);
        uint64_t zeroMap = 0xffffffffffffffffull << 32;
        uint64_t shouldBeZero = zeroMap & hashValue;
        EXPECT_EQ(0u, shouldBeZero);
        EXPECT_NE(0u, hashValue);
    }

    TEST(HashTest, request63BitHashValue)
    {
        char dataToHash[128] = {1};

        uint64_t hashValue = HashCalculator<uint64_t>::Hash(dataToHash, 128, 63);
        uint64_t zeroMap = 0xffffffffffffffffull << 63;
        uint64_t shouldBeZero = zeroMap & hashValue;
        EXPECT_EQ(0u, shouldBeZero);
        EXPECT_NE(0u, hashValue);
    }
}

