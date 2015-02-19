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
#include "capu/container/Vector.h"
#include "capu/os/Mutex.h"

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
            static char_t ReadChar();
            static void Flush();
            static void InterruptReadChar();

        private:
            static char_t ReadOneCharacter(HANDLE fileHandle);
            static bool_t IsKeyboardEventAvailable(HANDLE);
            static void SetEventHandle(HANDLE eventHandle);
            static HANDLE GetEventHandle();

            static HANDLE m_event;
            static const uint8_t Colors[];
            static Mutex interruptMutex;
        };

        inline
        bool_t
        Console::IsInputAvailable()
        {
            HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);
            switch (GetFileType(inHandle))
            {
                case FILE_TYPE_CHAR:
                {
            return _kbhit() != 0;
        }
                case FILE_TYPE_DISK:
                case FILE_TYPE_PIPE:
                {
                    const uint32_t zeroTimeOut = 0u;
                    DWORD ret = WaitForSingleObject(inHandle, zeroTimeOut);
                    return (WAIT_OBJECT_0 == ret);
                }
            };
            return false;
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

        inline
        char_t Console::ReadChar()
        {
            const HANDLE fileHandle = GetStdHandle(STD_INPUT_HANDLE);
            interruptMutex.lock();
            HANDLE eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
            SetEventHandle(eventHandle);
            interruptMutex.unlock();
            DWORD previousConsoleMode;

            const DWORD inputType = GetFileType(fileHandle);
            if (inputType == FILE_TYPE_CHAR)
            {
                GetConsoleMode(fileHandle, &previousConsoleMode);
                SetConsoleMode(fileHandle, previousConsoleMode & ~(ENABLE_LINE_INPUT | ENABLE_MOUSE_INPUT));
            };

            HANDLE ObjectVector[2];
            ObjectVector[0] = (fileHandle);
            ObjectVector[1] = (eventHandle);
            DWORD nObjects = static_cast<DWORD>(2);
            char_t buffer = -1;
            bool_t interrupted = false;
            while (buffer == -1 && !interrupted)
            {
                DWORD ret = WaitForMultipleObjects(nObjects, &ObjectVector[0], 0, INFINITE);
                if (ret == WAIT_OBJECT_0 + 1)
                {
                    // interrupted
                    interrupted = true;
                }
                else if (ret == WAIT_OBJECT_0 + 0)
                {
                    if (inputType == FILE_TYPE_CHAR)
                    {
                        bool_t shouldRead = IsKeyboardEventAvailable(fileHandle);
                        if (shouldRead)
                        {
                            buffer = ReadOneCharacter(fileHandle);
                        }
                    }
                    else if (inputType == FILE_TYPE_PIPE || inputType == FILE_TYPE_DISK)
                    {
                        buffer = ReadOneCharacter(fileHandle);
                    }
                }
            }
            interruptMutex.lock();
            CloseHandle(eventHandle);
            SetEventHandle(INVALID_HANDLE_VALUE);
            interruptMutex.unlock();
            return buffer;
        }

        inline
        void Console::Flush()
        {
            fflush(stdout);
            fflush(stderr);
        }

        inline
        void Console::InterruptReadChar()
        {
            interruptMutex.lock();
            HANDLE eventHandle = GetEventHandle();
            if (eventHandle != INVALID_HANDLE_VALUE)
            {
                SetEvent(eventHandle);
            }
            interruptMutex.unlock();
        }
    }
}

#endif //CAPU_WINDOWS_CONSOLE_H
