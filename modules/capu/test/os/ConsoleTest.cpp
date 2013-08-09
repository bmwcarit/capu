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

namespace capu
{
    ConsoleTest::ConsoleTest()
    {
    }

    ConsoleTest::~ConsoleTest()
    {
    }

    TEST_F(ConsoleTest, CheckForInput)
    {
        EXPECT_FALSE(Console::IsInputAvailable());
    }


    TEST_F(ConsoleTest, Print)
    {
        Console::Print("The Message\n");
    }

    TEST_F(ConsoleTest, PrintColor)
    {
        Console::Print(Console::GREEN, "The Message\n");
    }

}

