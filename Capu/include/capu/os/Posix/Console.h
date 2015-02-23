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
#include <capu/os/Mutex.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <termios.h>
#include <errno.h>

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
            static status_t ReadChar(char_t& buffer);
            static const char_t* Colors[];
            static void Flush();
            static void InterruptReadChar();
        private:
            static int32_t GetReadEndOfPipe();
            static int32_t GetWriteEndOfPipe();
            static void SetReadEndOfPipe(int32_t descriptor);
            static void SetWriteEndOfPipe(int32_t descriptor);
            static int32_t initializePipe();

            static int32_t pipeDescriptorsForInterruption[2];
            static Mutex interruptMutex;
        };

        inline
        int32_t Console::initializePipe()
        {
            const int32_t writeEndOfPipe = GetWriteEndOfPipe();
            if (writeEndOfPipe == -1)
            {
                int32_t ret = ::pipe(pipeDescriptorsForInterruption);
                return ret;
            }
            return 0;
        }

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
            printf("%s", capu::posix::Console::Colors[color]);
            vprintf(format, values);
            printf("\e[0m");
        }

        inline
        status_t Console::ReadChar(char_t& buffer)
        {
            struct termios oldTerminalSettings, temporaryWithoutEcho;

            // save previous settings
            if (0 != tcgetattr(fileno(stdin), &oldTerminalSettings))
            {
                if (errno == EBADF)
                {
                    return CAPU_EOF;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }

            // create new settings on top of previous settings
            Memory::Copy(&temporaryWithoutEcho, &oldTerminalSettings, sizeof(struct termios));
            temporaryWithoutEcho.c_lflag &= ~(ECHO | ICANON);
            temporaryWithoutEcho.c_cc[VTIME] = 0;
            temporaryWithoutEcho.c_cc[VMIN] = 1;

            // use new settings
            tcsetattr(fileno(stdin), TCSANOW, &temporaryWithoutEcho);
            ssize_t bytesRead = 0;
            interruptMutex.lock();
            const int32_t ret = initializePipe();
            interruptMutex.unlock();
            status_t status = CAPU_OK;
            if(0 == ret)
            {
                const int32_t stdinHandle = fileno(stdin);
                if (-1 == stdinHandle)
                {
                    return CAPU_ERROR;
                }
                bool_t hasReadCharacter = false;
                while (status == CAPU_OK && !hasReadCharacter) {
                    fd_set fdset;
                    FD_ZERO(&fdset);
                    FD_SET(GetReadEndOfPipe(), &fdset); // read end of pipe;
                    FD_SET(stdinHandle, &fdset);
                    int32_t highestFileDesciptor = GetReadEndOfPipe();
                    if (stdinHandle > highestFileDesciptor)
                    {
                        highestFileDesciptor = stdinHandle;
                    }
                    const int_t result = select(highestFileDesciptor + 1, &fdset, NULL, NULL, NULL);
                    if (result > 0)
                    {
                        if (FD_ISSET(stdinHandle, &fdset))
                        {
                            char_t readBuffer;
                            bytesRead = read(stdinHandle, &readBuffer, 1);
                            if (bytesRead <= 0)
                            {
                                status = CAPU_ERROR;
                            }
                            else
                            {
                                buffer = readBuffer;
                                hasReadCharacter = true;
                            }
                        }
                        if (FD_ISSET(GetReadEndOfPipe(), &fdset))
                        {
                            // explicit user interrupt
                            status = CAPU_INTERRUPTED;
                            char_t readBuffer;
                            const int32_t readStatus = read(GetReadEndOfPipe(), &readBuffer, 1);
                            UNUSED(readStatus);
                        }
                    }
                    else
                    {
                        status = CAPU_ERROR;
                        if (-1 == result && errno == EINTR)
                        {
                            // read another time, if it was only a signal interrupt
                            status = CAPU_OK;
                        }
                    }
                };
            }
            else
            {
                status = CAPU_ERROR;
            }

            tcsetattr(fileno(stdin), TCSANOW, &oldTerminalSettings);
            return status;
        }

        inline
        int32_t Console::GetReadEndOfPipe()
        {
            return pipeDescriptorsForInterruption[0];
        }

        inline int32_t Console::GetWriteEndOfPipe()
        {
            return pipeDescriptorsForInterruption[1];
        }

        inline
        void Console::SetReadEndOfPipe(int32_t descriptor)
        {
            pipeDescriptorsForInterruption[0] = descriptor;
        }

        inline
        void Console::SetWriteEndOfPipe(int32_t descriptor)
        {
            pipeDescriptorsForInterruption[1] = descriptor;
        }

        inline
        void
        Console::InterruptReadChar()
        {
            interruptMutex.lock();
            const int32_t ret = initializePipe();
            if (0 == ret)
            {
                const int32_t writeEndOfPipe = GetWriteEndOfPipe();
                ssize_t result = ::write(writeEndOfPipe,"#",1u);
                UNUSED(result);
            }
            interruptMutex.unlock();
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

        inline
        void
        Console::Flush()
        {
            fflush(stdout);
            fflush(stderr);
        }

    }
}

#endif //CAPU_POSIX_CONSOLE_H
