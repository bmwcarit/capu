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

#include <gtest/gtest.h>
#include "capu/Config.h"
#include "capu/os/DynamicLibrary.h"


TEST(DynamicLibrary, Constructor)
{
    capu::DynamicLibrary dl("filepath");
    EXPECT_TRUE(capu::StringUtils::Strcmp("filepath", dl.getPath().c_str()) >= 0);
}

TEST(DynamicLibrary, Constructor2)
{
    capu::DynamicLibrary dl("/abolute/path/test");
    EXPECT_TRUE(capu::StringUtils::Strcmp("/abolute/path/test", dl.getPath().c_str()) >= 0);
}

TEST(DynamicLibrary, Constructor3)
{
    capu::DynamicLibrary dl("./relative/../path");
    EXPECT_TRUE(capu::StringUtils::Strcmp("./relative/../path", dl.getPath().c_str()) >= 0);
}

TEST(DynamicLibrary, LoadAndClose)
{
    capu::String filename = capu::DynamicLibrary::defaultLibraryPrefix;
    filename.append("DynamicTestLibrary");
    filename.append(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::DynamicLibrary dl(filename);

    EXPECT_EQ(capu::CAPU_OK, dl.load());
    EXPECT_EQ(capu::CAPU_OK, dl.unload());
}

TEST(DynamicLibrary, MultipleLoadGivesError)
{
    capu::String filename = capu::DynamicLibrary::defaultLibraryPrefix;
    filename.append("DynamicTestLibrary");
    filename.append(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::DynamicLibrary dl(filename);

    EXPECT_EQ(capu::CAPU_OK, dl.load());
    EXPECT_EQ(capu::CAPU_ERROR, dl.load());
    EXPECT_EQ(capu::CAPU_ERROR, dl.load());
    EXPECT_EQ(capu::CAPU_ERROR, dl.load());

    EXPECT_EQ(capu::CAPU_OK, dl.unload());
}

TEST(DynamicLibrary, MultipleUnloadGivesError)
{
    capu::String filename = capu::DynamicLibrary::defaultLibraryPrefix;
    filename.append("DynamicTestLibrary");
    filename.append(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::DynamicLibrary dl(filename);

    EXPECT_EQ(capu::CAPU_OK, dl.load());
    EXPECT_EQ(capu::CAPU_OK, dl.unload());
    EXPECT_EQ(capu::CAPU_ERROR, dl.unload());
    EXPECT_EQ(capu::CAPU_ERROR, dl.unload());
}

TEST(DynamicLibrary, GetSymbolFromLibrary)
{
    capu::String filename = capu::DynamicLibrary::defaultLibraryPrefix;
    filename.append("DynamicTestLibrary");
    filename.append(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::DynamicLibrary dl(filename);

    ASSERT_EQ(capu::CAPU_OK, dl.load());

    void* loadedSymbol = 0;
    EXPECT_EQ(capu::CAPU_OK, dl.loadSymbol("testFunction", loadedSymbol));
    ASSERT_TRUE(loadedSymbol != 0);

    EXPECT_EQ(capu::CAPU_OK, dl.unload());
}

TEST(DynamicLibrary, CallSymbolFromLibrary)
{
    // load
    capu::String filename = capu::DynamicLibrary::defaultLibraryPrefix;
    filename.append("DynamicTestLibrary");
    filename.append(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::DynamicLibrary dl(filename);

    ASSERT_EQ(capu::CAPU_OK, dl.load());

    void* loadedSymbol = 0;
    EXPECT_EQ(capu::CAPU_OK, dl.loadSymbol("testFunction", loadedSymbol));
    ASSERT_TRUE(loadedSymbol != 0);

    // call
    typedef int (*funcPtr)();
    funcPtr loadedFunction;
    loadedFunction = (funcPtr)loadedSymbol;

    EXPECT_EQ(123, loadedFunction());
    EXPECT_EQ(capu::CAPU_OK, dl.unload());
}

TEST(DynamicLibrary, CheckUnresolvedExportSymbol)
{
    capu::String test(capu::DynamicLibrary::defaultLibraryPostfix);
    capu::String test2(capu::DynamicLibrary::defaultLibraryPrefix);
}
