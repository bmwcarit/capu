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

namespace capu
{
    typedef ::int8_t    int8_t;
    typedef ::int16_t   int16_t;
    typedef unsigned char   uchar_t;
    typedef ::int64_t   time_t;
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

    typedef ::intptr_t  int_t;
    typedef ::uintptr_t  uint_t;
}

#endif //CAPU_CONFIG_H

