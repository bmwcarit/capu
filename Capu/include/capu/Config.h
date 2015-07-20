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

#ifndef CAPU_CONFIG_H
#define CAPU_CONFIG_H

/**
 * os specific defines
 */
#ifdef OS_WINDOWS
#define _WINSOCKAPI_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UNUSED(x) {(void)(x);}

#define capu_foreach(IteratableType, Iteratable, Iter)\
    IteratableType::Iterator Iter = (Iteratable).begin();\
    for(const IteratableType::Iterator end = (Iteratable).end(); Iter != end; ++Iter)

#define capu_const_foreach(IteratableType, Iteratable, Iter)\
    IteratableType::ConstIterator Iter = (Iteratable).begin(); \
    for (const IteratableType::ConstIterator end = (Iteratable).end(); Iter != end; ++Iter)

namespace capu
{
    typedef signed char    int8_t;
    typedef signed short   int16_t;
    typedef unsigned char   uchar_t;
    typedef unsigned char   uint8_t;
    typedef unsigned short  uint16_t;
    typedef signed int int32_t;
    typedef signed long long int int64_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long int uint64_t;

    #if defined(ARCH_X86_64) || defined(ARCH_ARM64)
        typedef signed long long int int_t;
        typedef unsigned long long  int uint_t;
    #else
        typedef signed int int_t;
        typedef unsigned int uint_t;
    #endif
}

#endif //CAPU_CONFIG_H

