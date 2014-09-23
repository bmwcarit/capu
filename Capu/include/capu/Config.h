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
#include <stdint.h>
#include <time.h>

#define UNUSED(x) {(void)(x);}

#define capu_foreach(IteratableType, Iteratable, Iter)\
    IteratableType::Iterator Iter = Iteratable.begin();\
    const IteratableType::Iterator end = Iteratable.end();\
    for(; Iter != end; ++Iter)

#define capu_const_foreach(IteratableType, Iteratable, Iter)\
    IteratableType::ConstIterator Iter = Iteratable.begin(); \
    const IteratableType::ConstIterator end = Iteratable.end(); \
for (; Iter != end; ++Iter)

namespace capu
{
    typedef ::int8_t    int8_t;
    typedef ::int16_t   int16_t;
    typedef unsigned char   uchar_t;
    typedef ::time_t   time_t;
    typedef ::uint8_t   uint8_t;
    typedef ::int16_t   int16_t;
    typedef ::uint16_t  uint16_t;
    typedef ::int32_t   int32_t;
    typedef ::int64_t   int64_t;
    typedef ::uint32_t  uint32_t;
    typedef ::uint64_t  uint64_t;
    typedef float   float_t;
    typedef double  double_t;
    typedef bool    bool_t;
    typedef char    char_t;

    #if defined(OS_MacOSX) || defined(OS_IPHONEOS) || defined(OS_IPHONESIMULATOROS)
        #if defined(ARCH_X86_64) || defined(ARCH_ARM64)
            typedef ::int64_t  int_t;
            typedef ::uint64_t  uint_t;
        #else
            typedef ::int32_t  int_t;
            typedef ::uint32_t  uint_t;
        #endif
    #else
        typedef ::intptr_t  int_t;
        typedef ::uintptr_t  uint_t;
    #endif
}

#endif //CAPU_CONFIG_H

