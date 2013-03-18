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

#ifndef CAPU_UNIXBASED_STRINGUTILS_H
#define CAPU_UNIXBASED_STRINGUTILS_H

#include "capu/Config.h"
#include <string.h>
#include <stdarg.h>

namespace capu
{
    namespace posix
    {
        class StringUtils
        {
        public:
            static void Strncpy(char_t* dst, const uint_t dstSize, const char_t* src);
            static void Sprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, ...);
            static void Vsprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, va_list values);
            static int32_t Vscprintf(const char_t* format, va_list values);
            static uint_t Strlen(const char_t* str);
            static int_t Strcmp(const char_t* str1, const char_t* str2);
            static int_t LastIndexOf(const char_t* str, const char_t ch);
            static int_t IndexOf(const char_t* str, const char_t ch);
            static int_t IndexOf(const char_t* str, const char_t* str2);
            static bool_t StartsWith(const char_t* str, const char_t* prefix);
        };

        inline
        void
        StringUtils::Strncpy(char_t* dst, uint_t dstSize, const char_t* src)
        {
            // reproduce behaviour of strncpy_s
            strncpy(dst, src, dstSize - 1);
            dst[dstSize - 1] = 0;
        }

        inline
        uint_t
        StringUtils::Strlen(const char_t* str)
        {
            if (str == 0)
            {
                return 0;
            }
            return strlen(str);
        }

        inline
        int_t
        StringUtils::Strcmp(const char_t* str1, const char_t* str2)
        {
            return strcmp(str1, str2);
        }

        inline
        void
        StringUtils::Vsprintf(char_t* buffer, uint_t bufferSize, const char_t* format, va_list values)
        {
            vsnprintf(buffer, bufferSize, format, values);
        }

        inline
        int32_t
        StringUtils::Vscprintf(const char_t* format, va_list values)
        {
            char c;
            return vsnprintf(&c, 1, format, values);
        }

        inline
        void
        StringUtils::Sprintf(char_t* buffer, uint_t bufferSize, const char_t* format, ...)
        {
            va_list argptr;
            va_start(argptr, format);
            StringUtils::Vsprintf(buffer, bufferSize, format, argptr);
            va_end(argptr);
        }

        inline
        int_t
        StringUtils::LastIndexOf(const char_t* str, const char_t ch)
        {
            if (!str)
            {
                return -1;
            }
            const char_t* pos = strrchr(str, ch);
            return pos ? pos - str : -1;
        }

        inline
        int_t
        StringUtils::IndexOf(const char_t* str, const char_t ch)
        {
            if (!str)
            {
                return -1;
            }
            const char_t* pos = strchr(str, ch);
            return pos ? pos - str : -1;
        }

        inline
        bool_t
        StringUtils::StartsWith(const char_t* str, const char_t* prefix)
        {
            if (!prefix || !str)
            {
                return false;
            }
            return strncmp(str, prefix, strlen(prefix)) == 0;
        }
		
        inline
        int_t 
        StringUtils::IndexOf(const char_t* str, const char_t* str2)
       {
            const char_t* start = strstr(str, str2);
            return start ? (start - str) : -1;
       }
    }
}
#endif // CAPU_UNIXBASED_STRINGUTILS_H
