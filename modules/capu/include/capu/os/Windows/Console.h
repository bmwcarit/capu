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

#ifndef CAPU_WINDOWS_CONSOLE_H
#define CAPU_WINDOWS_CONSOLE_H

#include <conio.h>
#include "capu/container/String.h"

namespace capu
{
    namespace os
    {
        class Console
        {
        public:
            static bool_t IsInputAvailable();
            static void Print(const char_t* format, va_list values);
            static void Print(uint32_t color, const char_t* format, va_list values);

        private:

            static const uint8_t Colors[];

        };

        inline
        bool_t
        Console::IsInputAvailable()
        {
            return _kbhit() != 0;
        }

        inline
        void Console::Print(const char_t* format, va_list values)
        {
            vprintf(format, values);
        }

        inline
        void Console::Print(uint32_t color,  const char_t* format, va_list values)
        {
            uint8_t colorCode = Colors[color];

            HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo( hstdout, &csbi );
            SetConsoleTextAttribute( hstdout, colorCode );

            Console::Print(format, values);

            SetConsoleTextAttribute( hstdout, csbi.wAttributes);

        }

    }
}

#endif //CAPU_WINDOWS_CONSOLE_H
