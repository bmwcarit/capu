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

#include "gtest/gtest.h"
#include "capu/container/Hash.h"

namespace capu
{
    class SomeClass
    {};

    enum SomeEnum
    {
        SOME_ENUM_MEMBER
    };

    TEST(CapuDefaultHashFunctionTest, useTraitsToFindCorrectHashFunction)
    {
        capu::uint64_t       sometype1 = 42;
        capu::uint32_t       sometype2 = 42;
        capu::int32_t        sometype3 = 42;
        capu::uint16_t       sometype4 = 42;
        capu::int16_t        sometype5 = 42;
        capu::uint8_t        sometype6 = 42;
        capu::int8_t         sometype7 = 42;
        capu::char_t         sometype8 = 42;
        capu::uchar_t        sometype9 = 42;
        float        sometype10 = 42.f;
        double       sometype11 = 42.0;
        bool         sometype12 = true;
        capu::char_t         sometype13[] = "HashTest";
        const capu::char_t*  sometype14 = "HashTest2";

        SomeClass clazz;
        SomeClass clazz2;
        SomeClass* ptr = &clazz;
        SomeClass* ptr2 = &clazz2;

        // everything must compile...
        capu::CapuDefaultHashFunction<>::Digest(sometype1);
        capu::CapuDefaultHashFunction<>::Digest(sometype2);
        capu::CapuDefaultHashFunction<>::Digest(sometype3);
        capu::CapuDefaultHashFunction<>::Digest(sometype4);
        capu::CapuDefaultHashFunction<>::Digest(sometype5);
        capu::CapuDefaultHashFunction<>::Digest(sometype6);
        capu::CapuDefaultHashFunction<>::Digest(sometype7);
        capu::CapuDefaultHashFunction<>::Digest(sometype8);
        capu::CapuDefaultHashFunction<>::Digest(sometype9);
        capu::CapuDefaultHashFunction<>::Digest(sometype10);
        capu::CapuDefaultHashFunction<>::Digest(sometype11);
        capu::CapuDefaultHashFunction<>::Digest(sometype12);
        capu::CapuDefaultHashFunction<>::Digest(sometype13);
        capu::CapuDefaultHashFunction<>::Digest(sometype14);
        capu::CapuDefaultHashFunction<>::Digest(clazz);
        capu::CapuDefaultHashFunction<>::Digest(ptr, 4);
        capu::CapuDefaultHashFunction<>::Digest(ptr2, 4);
        capu::CapuDefaultHashFunction<>::Digest(SOME_ENUM_MEMBER);

    }
}

