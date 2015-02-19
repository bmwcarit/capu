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

#include "ConsoleTest.h"
#include "capu/util/Runnable.h"
#include "capu/os/Thread.h"

namespace capu
{
    ConsoleTest::ConsoleTest()
    {
    }

    ConsoleTest::~ConsoleTest()
    {
    }

    TEST_F(ConsoleTest, Print)
    {
        Console::Print("The Message\n");
    }

    TEST_F(ConsoleTest, PrintColor)
    {
        Console::Print(Console::GREEN, "The Message\n");
    }

    // Console must be tested manually

    TEST_F(ConsoleTest, DISABLED_CheckForInput)
    {
        EXPECT_FALSE(Console::IsInputAvailable());
    }

    // Disabled tests can be forcefully enabled from commandline to test this
    TEST_F(ConsoleTest, DISABLED_ReadChar)
    {
        char_t readChar = Console::ReadChar();
        Console::Print("I have read: %c\n",readChar);
        readChar = Console::ReadChar();
        Console::Print("I have read: %c\n", readChar);
        readChar = Console::ReadChar();
        Console::Print("I have read: %c\n", readChar);
        readChar = Console::ReadChar();
        Console::Print("I have read: %c\n", readChar);
        readChar = Console::ReadChar();
        Console::Print("I have read: %c\n", readChar);
    }

    class CallInteruptAfter1Second : public Runnable
    {
    public:
        virtual void run()
        {
            Thread::Sleep(1000);
            Console::InterruptReadChar();
        }
    };

    TEST_F(ConsoleTest, ReadChar_Interupt)
    {
        CallInteruptAfter1Second interupter;
        Thread t;
        t.start(interupter);
        char_t readChar = Console::ReadChar();
        EXPECT_EQ(-1, readChar);
    }

}

