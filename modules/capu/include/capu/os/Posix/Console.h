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

#ifndef CAPU_POSIX_CONSOLE_H
#define CAPU_POSIX_CONSOLE_H

#include <stdio.h>
#include <capu/os/Memory.h>
#include <unistd.h>
#include <stdarg.h>

namespace capu
{
    namespace posix
    {
        class Console
        {
        public:
            static bool_t IsInputAvailable();
            static void Print(const char_t* format, va_list values);
            static void Print(uint32_t color, const char_t* format, va_list values);

            static const char_t* Colors[];

        };

        inline
        void
        Console::Print(const char_t* format, va_list values)
        {
            vprintf(format, values);
        }

        inline
        void
        Console::Print(uint32_t color, const char_t* format, va_list values)
        {
            printf(capu::posix::Console::Colors[color]);
            vprintf(format, values);
            printf("\e[0m");
        }

        inline
        bool_t
        Console::IsInputAvailable()
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            int ret = select(STDIN_FILENO + 1, &fds, 0, 0, &tv);
			if (-1 == ret)
			{
				return false;
			}

            return FD_ISSET(STDIN_FILENO, &fds) != 0;
        }

    }
}

#endif //CAPU_POSIX_CONSOLE_H
