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

#include "stdio.h"
#include "stdlib.h"

namespace capu
{
#if defined (OS_WINDOWS)
    typedef signed char            int8_t;
    typedef unsigned char          uint8_t;
    typedef short                  int16_t;
    typedef unsigned short         uint16_t;
    typedef int                    int32_t;
    typedef signed __int64         int64_t;
    typedef unsigned int           uint32_t;
    typedef unsigned __int64       uint64_t;
    typedef float                  float_t;
    typedef double                 double_t;
    typedef bool                   bool_t;
    typedef char                   char_t;
    typedef uint8_t                uchar_t;
    typedef int64_t                time_t;
#elif defined (OS_LINUX)
    typedef signed char            int8_t;
    typedef unsigned char          uint8_t;
    typedef short int              int16_t;
    typedef unsigned short int     uint16_t;
    typedef int                    int32_t;
    typedef long long int          int64_t;
    typedef unsigned int           uint32_t;
    typedef unsigned long long int uint64_t;
    typedef float                  float_t;
    typedef bool                   bool_t;
    typedef double                 double_t;
    typedef char                   char_t;
    typedef uint8_t                uchar_t;
    typedef int64_t                time_t;
#elif defined (OS_MACOSX)
    typedef signed char            int8_t;
    typedef unsigned char          uint8_t;
    typedef short int              int16_t;
    typedef unsigned short int     uint16_t;
    typedef int                    int32_t;
    typedef long long int          int64_t;
    typedef unsigned int           uint32_t;
    typedef unsigned long long int uint64_t;
    typedef float                  float_t;
    typedef bool                   bool_t;
    typedef double                 double_t;
    typedef char                   char_t;
    typedef uint8_t                uchar_t;
    typedef int64_t                time_t;
#elif defined (OS_INTEGRITY)
    typedef signed char            int8_t;
    typedef unsigned char          uint8_t;
    typedef short int              int16_t;
    typedef unsigned short int     uint16_t;
    typedef int                    int32_t;
    typedef long int               int64_t;
    typedef unsigned int           uint32_t;
    typedef unsigned long int      uint64_t;
    typedef float                  float_t;
    typedef double                 double_t;
    typedef bool                   bool_t;
    typedef double                 double_t;
    typedef char                   char_t;
    typedef uint8_t                uchar_t;
    typedef int64_t                time_t;
#elif defined (OS_QNX)
    typedef signed char            int8_t;
    typedef unsigned char          uint8_t;
    typedef short int              int16_t;
    typedef unsigned short int     uint16_t;
    typedef int                    int32_t;
    typedef long long int          int64_t;
    typedef unsigned int           uint32_t;
    typedef unsigned long long int uint64_t;
    typedef float                  float_t;
    typedef double                 double_t;
    typedef bool                   bool_t;
    typedef double                 double_t;
    typedef char                   char_t;
    typedef int64_t                time_t;
    typedef uint8_t                uchar_t;
#endif

    /**
    * platform depended types
    */
#if defined (ARCH_X86_32)
    typedef int32_t                int_t;
    typedef uint32_t               uint_t;
#elif defined (ARCH_X86_64)
    typedef int64_t                int_t;
    typedef uint64_t               uint_t;
#elif defined (ARCH_ARMV7L)
    typedef int32_t                int_t;
    typedef uint32_t               uint_t;
#endif
}

#endif //CAPU_CONFIG_H

