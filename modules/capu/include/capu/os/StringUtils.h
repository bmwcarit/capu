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

#ifndef CAPU_STRINGUTILS_H
#define CAPU_STRINGUTILS_H

#include "capu/Config.h"
#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(StringUtils)

namespace capu
{
    /**
     * Utilities for manipulating string.
     */
    class StringUtils
    {
    public:
        /**
         * Static method to copy a String of length dstSize from src to dst
         * @param dst destination buffer
         * @param dstSize number of chars to be copied
         * @param src source buffer
         */
        static void Strncpy(char_t* dst, const uint_t dstSize, const char_t* src);

        /**
         * Static method to write a C string according to the given format into the array pointed by buffer.
         * After the format parameter, the function expects at least as many additional arguments as specified in format.
         * @param buffer which contains the string
         * @param bufferSize size of the buffer
         * @param format the format of the string
         */
        static void Sprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, ...);

        /**
         * Static method to write a C string according to the given format into the array pointed by buffer.
         * The arguments specified in the format have to be passed by the values parameter.
         * @param buffer which contains the string
         * @param bufferSize size of the buffer
         * @param format the format of the string
         * @param values arguments for the format
         */
        static void Vsprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, va_list values);

        /**
         * Static method to count the bytes according to the given format
         * The arguments specified in the format have to be passed by the values parameter.
         * @param format the format of the string
         * @param values arguments for the format
         */
        static int32_t Vscprintf(const char_t* format, va_list values);

        /**
         * Static method to return the length of the given String
         * @param str the string
         * @return length of the string
         */
        static uint_t Strlen(const char_t* str);

        /**
         * Static method to compare two C strings.
         * @param str1 first string
         * @param str2 second string
         * @return 0 if both strings are equal
         *         > 0 if the first character which does not match has a greater value in str1
         *         < 0 otherwise
         */
        static int_t Strcmp(const char_t* str1, const char_t* str2);

        /**
         * Calculates the last index of a given character.
         * @param str the string
         * @param ch the search char.
         * @return The last index of the specified char or -1 if the char was not found.
         */
        static int_t LastIndexOf(const char_t* str, const char_t ch);

        /**
         * Calculates the first index of a given character.
         * @param str the string
         * @param ch the search char.
         * @return The first index of the specified char or -1 if the char was not found.
         */
        static int_t IndexOf(const char_t* str, const char_t ch);

        /**
         * Checks if the given testString starts with the given prefix.
         * @param str The string which is tested.
         * @param prefix The prefix.
         * @return True if the testString starts with the given prefix.
         */
        static bool_t StartsWith(const char_t* str, const char_t* prefix);
    };

    inline
    void
    StringUtils::Strncpy(char_t* dst, const uint_t dstSize, const char_t* src)
    {
        capu::os::arch::StringUtils::Strncpy(dst, dstSize, src);
    }

    inline
    void
    StringUtils::Sprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        capu::os::arch::StringUtils::Vsprintf(buffer, bufferSize, format, argptr);
        va_end(argptr);
    }

    inline
    void
    StringUtils::Vsprintf(char_t* buffer, const uint_t bufferSize, const char_t* format, va_list values)
    {
        capu::os::arch::StringUtils::Vsprintf(buffer, bufferSize, format, values);
    }

    inline
    int32_t
    StringUtils::Vscprintf(const char_t* format, va_list values)
    {
        return capu::os::arch::StringUtils::Vscprintf(format, values);
    }

    inline
    uint_t
    StringUtils::Strlen(const char_t* str)
    {
        return capu::os::arch::StringUtils::Strlen(str);
    }

    inline
    int_t
    StringUtils::Strcmp(const char_t* str1, const char_t* str2)
    {
        return capu::os::arch::StringUtils::Strcmp(str1, str2);
    }

    inline
    int_t
    StringUtils::LastIndexOf(const char_t* str, const char_t ch)
    {
        return capu::os::arch::StringUtils::LastIndexOf(str, ch);
    }

    inline
    int_t
    StringUtils::IndexOf(const char_t* str, const char_t ch)
    {
        return capu::os::arch::StringUtils::IndexOf(str, ch);
    }

    inline
    bool_t
    StringUtils::StartsWith(const char_t* str, const char_t* prefix)
    {
        return capu::os::arch::StringUtils::StartsWith(str, prefix);
    }

}
#endif //CAPU_STRINGUTILS_H

