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
#include "capu/util/Traits.h"
#include "capu/container/Hash.h"
#include "capu/Config.h"

class SomeClass
{};

enum SomeEnum
{
    SOME_ENUM_MEMBER
};

class Tester
{
public:

    template <typename T>
    static capu::uint32_t type(T val)
    {
        val = val;
        enum
        {
            type = capu::is_CAPU_PRIMITIVE<T>::Value | capu::is_CAPU_REFERENCE<T>::Value | capu::is_CAPU_POINTER<T>::Value
        };
        return type;
    }
};

class SomeClassWithEnumSize
{
    capu::uint32_t myValue;

    SomeClassWithEnumSize(){
        UNUSED(myValue);
    }
};

union SomeUnion
{
    capu::uint32_t intVal1;
    capu::uint32_t intVal2;
    struct
    {
        capu::uint16_t shortVal1;
        capu::uint16_t shortVal2;
        capu::uint16_t shortVal3;
        capu::uint16_t shortVal4;
    };
};

union SomeUnionWithEnumSize
{
    char data[sizeof(SomeEnum)];
    struct
    {
        char data2[sizeof(SomeEnum)];
    };
};

TEST(Traits, TestTypeIdentifier)
{
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::uint64_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::uint32_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::int32_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::uint16_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::int16_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::uint8_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::int8_t>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<char>::Identifier);
    EXPECT_EQ(CAPU_TYPE_PRIMITIVE, capu::Type<capu::uchar_t>::Identifier);

    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<void*>::Identifier);
    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeClass*>::Identifier);
    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeClass**>::Identifier);
    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeClass***>::Identifier);

    EXPECT_EQ(CAPU_TYPE_REFERENCE, capu::Type<SomeClass&>::Identifier);
    EXPECT_EQ(CAPU_TYPE_REFERENCE, capu::Type<capu::uint32_t&>::Identifier);
    EXPECT_EQ(CAPU_TYPE_REFERENCE, capu::Type<SomeEnum&>::Identifier);

    EXPECT_EQ(CAPU_TYPE_ENUM, capu::Type<SomeEnum>::Identifier);

    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeEnum*>::Identifier);
    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeEnum**>::Identifier);
    EXPECT_EQ(CAPU_TYPE_POINTER, capu::Type<SomeEnum***>::Identifier);

    EXPECT_EQ(CAPU_TYPE_VOID, capu::Type<void>::Identifier);

    EXPECT_EQ(CAPU_TYPE_CLASS, capu::Type<SomeUnion>::Identifier);
    EXPECT_EQ(CAPU_TYPE_CLASS, capu::Type<SomeClass>::Identifier);
    EXPECT_EQ(CAPU_TYPE_CLASS, capu::Type<SomeUnionWithEnumSize>::Identifier);
    EXPECT_EQ(CAPU_TYPE_CLASS, capu::Type<SomeClassWithEnumSize>::Identifier);
}

TEST(Traits, TestPrimitiveTypes)
{
    capu::uint64_t sometype1 = 42;
    capu::uint32_t sometype2 = 42;
    capu::int32_t  sometype3 = 42;
    capu::uint16_t sometype4 = 42;
    capu::int16_t  sometype5 = 42;
    capu::uint8_t  sometype6 = 42;
    capu::int8_t   sometype7 = 42;
    char   sometype8 = 42;
    capu::uchar_t  sometype9 = 42;
    float  sometype10 = 42.f;
    double sometype11 = 42.0;
    bool   sometype12 = true;

    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype1));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype2));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype3));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype4));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype5));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype6));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype7));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype8));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype9));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype10));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype11));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_PRIMITIVE), Tester::type(sometype12));
}

TEST(Traits, TestPointerAndReferences)
{
    SomeClass clazz;
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_POINTER), Tester::type(&clazz));
    EXPECT_EQ(static_cast<capu::uint32_t>(CAPU_TYPE_REFERENCE), Tester::type<SomeClass&>(clazz));
}
