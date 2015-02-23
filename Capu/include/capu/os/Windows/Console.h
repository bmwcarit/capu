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
            static status_t ReadChar(char_t& buffer);
            static void Flush();
            static void InterruptReadChar();

        private:
            static void InitializeInterruptEvent();
            static status_t ReadOneCharacter(HANDLE fileHandle, char_t& buffer);
            static void SetEventHandle(HANDLE eventHandle);
            static HANDLE GetInterruptEventHandle();

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
        status_t Console::ReadChar(char_t& buffer)
        {
            const HANDLE fileHandle = GetStdHandle(STD_INPUT_HANDLE);
            interruptMutex.lock();
            InitializeInterruptEvent();
            interruptMutex.unlock();

            DWORD previousConsoleMode;

            const DWORD inputType = GetFileType(fileHandle);
            if (inputType == FILE_TYPE_CHAR)
            {
                GetConsoleMode(fileHandle, &previousConsoleMode);
                SetConsoleMode(fileHandle, previousConsoleMode & ~(ENABLE_LINE_INPUT | ENABLE_MOUSE_INPUT));
            };

            const int_t STDIN_INDEX = 0;
            const int_t INTERRUPT_EVENT_INDEX = 1;

            HANDLE handles[2];
            handles[STDIN_INDEX] = fileHandle;
            handles[INTERRUPT_EVENT_INDEX] = GetInterruptEventHandle();
            const DWORD numberOfObjectsToWaitOn = 2;

            status_t status = CAPU_OK;
            bool_t haveReadCharacter = false;
            while (CAPU_OK == status && !haveReadCharacter)
            {
                const DWORD ret = WaitForMultipleObjects(numberOfObjectsToWaitOn, handles, false, INFINITE);
                if (ret == WAIT_OBJECT_0 + INTERRUPT_EVENT_INDEX)
                {
                    status = CAPU_INTERRUPTED;
                }
                else if (ret == WAIT_OBJECT_0 + STDIN_INDEX)
                {
                    if (inputType == FILE_TYPE_CHAR)
                    {
                        const uint32_t numberOfInputEventsToRead = 1u;
                        DWORD numberOfReadEvents = 0;
                        INPUT_RECORD inputRecord;
                        ZeroMemory(&inputRecord, sizeof(INPUT_RECORD));
                        const BOOL peekStatus = PeekConsoleInput(fileHandle, &inputRecord, numberOfInputEventsToRead, &numberOfReadEvents);
                        if (peekStatus)
                        {
                            if (numberOfReadEvents > 0)
                            {
                                if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
                                {
                                    // next available event is keyboard event
                                    status = ReadOneCharacter(fileHandle, buffer);
                                    if (CAPU_OK == status)
                                    {
                                        haveReadCharacter = true;
                                    }
                                }
                                else
                                {
                                    // prune non keyboard event
                                    ReadConsoleInput(fileHandle, &inputRecord, numberOfInputEventsToRead, &numberOfReadEvents);
                                }
                            }
                        }
                        else
                        {
                            const DWORD error = GetLastError();
                            if (ERROR_INVALID_HANDLE == error)
                            {
                                status = CAPU_EOF;
                            }
                            else
                            {
                                status = CAPU_ERROR;
                            }
                        }
                    }
                    else if (inputType == FILE_TYPE_PIPE || inputType == FILE_TYPE_DISK)
                    {
                        status = ReadOneCharacter(fileHandle, buffer);
                        if (CAPU_OK == status)
                        {
                            haveReadCharacter = true;
                        }
                    }
                }
                else if (ret == WAIT_FAILED)
                {
                    const DWORD error = GetLastError();
                    if (ERROR_INVALID_HANDLE == error)
                    {
                        status = CAPU_EOF;
                    }
                    else
                    {
                        status = CAPU_ERROR;
                    }
                }
            }
            return status;
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
            InitializeInterruptEvent();
            SetEvent(GetInterruptEventHandle());
            interruptMutex.unlock();
        }
    }
}

#endif //CAPU_WINDOWS_CONSOLE_H
