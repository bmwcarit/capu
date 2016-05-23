/*
* Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_COMPLEXTESTTYPE_H
#define CAPU_COMPLEXTESTTYPE_H

#include "capu/container/vector.h"
#include "capu/Config.h"

class ComplexTestType
{
public:
    ComplexTestType(capu::uint_t value_ = 0u)
        :value(value_)
    {
        ++ctor_count;
    }

    ComplexTestType(const ComplexTestType& other)
    {
        value = other.value;
        ++copyctor_count;
    }

    bool operator==(const ComplexTestType& other) const
    {
        return value == other.value;
    }

    bool operator!=(const ComplexTestType& other) const
    {
        return value != other.value;
    }

    ~ComplexTestType()
    {
        ++dtor_count;
    }

    static void Reset()
    {
        ctor_count = 0;
        copyctor_count = 0;
        dtor_count = 0;
    }

    capu::uint_t value;

    static capu::uint_t ctor_count;
    static capu::uint_t copyctor_count;
    static capu::uint_t dtor_count;
};

#endif
