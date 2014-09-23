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

#ifdef BUILD_CHECK_MEMORY
#ifdef OS_WINDOWS
#include "vld.h"
#endif
#endif

#include <iostream>
#include "gtest/gtest.h"
#include "capu/os/Time.h"

#ifdef __linux__
#include <sys/resource.h>
#endif

GTEST_API_ int main(int argc, char** argv)
{
    std::cout << "Running main() from capu-test\n";

    // Set soft limit for core dump files in order to create a core file on crash.
    // The default value is usually 0 which means no core file is created.
#ifdef __linux__
    rlimit limits;
    getrlimit(RLIMIT_CORE, &limits);
    rlim_t newLimit = 1073741824;
    limits.rlim_cur = newLimit < limits.rlim_max ? newLimit : limits.rlim_max;
    std::cout << "Core soft limit : setting to " << limits.rlim_cur << std::endl;
    setrlimit(RLIMIT_CORE, &limits);
    getrlimit(RLIMIT_CORE, &limits);
#endif

    //initialize pseudo random generate used for all the tests
    srand(static_cast<capu::uint32_t>(capu::Time::GetMilliseconds()));

    bool wait = false;
    // special wait command
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--gtest_pause") == 0)
        {
            wait = true;
            argv[i] = const_cast<char*>("");
        }
    }

    testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    if (wait)
    {
        printf("press <enter> to exit!");
        getchar();
    }

    return testResult;
}
