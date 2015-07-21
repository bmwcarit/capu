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

#ifndef CAPU_CAPUSTDINT_H
#define CAPU_CAPUSTDINT_H

#include "capu/Config.h"

#if !defined(CAPU_HAVE_STDINT_H)

/* INT64_T */
#if defined(HAVE_CAPU_SIZEOF___INT64)
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#elif defined(HAVE_CAPU_SIZEOF_LONG_INT) && (CAPU_SIZEOF_LONG_INT == 8)
typedef signed long int int64_t;
typedef unsigned long int uint64_t;
#elif defined(HAVE_CAPU_SIZEOF_LONG_LONG_INT) && (CAPU_SIZEOF_LONG_LONG_INT == 8)
typedef signed long long int int64_t;
typedef unsigned long long int uint64_t;
#endif // HAVE_CAPU_SIZEOF_LONG_INT

/* INT8_T */
#if defined(HAVE_CAPU_SIZEOF_CHAR) && (CAPU_SIZEOF_CHAR == 1)
typedef signed char int8_t;
typedef unsigned char uint8_t;
#endif // HAVE_CAPU_SIZEOF_CHAR

/* INT16_T */
#if defined(HAVE_CAPU_SIZEOF_SHORT_INT) && (CAPU_SIZEOF_SHORT_INT == 2)
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
#endif // HAVE_CAPU_SIZEOF_SHORT_INT

/* INT32_T */
#if defined(HAVE_CAPU_SIZEOF_INT) && (CAPU_SIZEOF_INT == 4)
typedef signed int int32_t;
typedef unsigned int uint32_t;
#endif // HAVE_CAPU_SIZEOF_INT

#endif // CAPU_HAVE_STDINT_H


#endif // CAPU_CAPUSTDINT_H