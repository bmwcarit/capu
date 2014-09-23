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
#include "capu/os/File.h"
#include "capu/util/FileTraverser.h"

class TestVisitor : public capu::IFileVisitor
{
public:
    capu::uint32_t mCallCount;
    capu::bool_t mStepIntoDirectory;
    capu::status_t mReturnValue;

    TestVisitor(capu::bool_t stepIntoDir = true)
        : mCallCount(0)
        , mStepIntoDirectory(stepIntoDir)
        , mReturnValue(capu::CAPU_OK)
    {
    }

    capu::status_t visit(capu::File& file, capu::bool_t& stepIntoDirectory)
    {
        EXPECT_TRUE(file.exists());
        ++mCallCount;
        stepIntoDirectory = mStepIntoDirectory;
        return mReturnValue;
    }
};

TEST(FileTraverser, defaultAcceptTest)
{
    // setup
    capu::File f1("foobarfolder");
    f1.createDirectory();
    capu::File f2(f1, "foobar1.txt");
    f2.createFile();
    capu::File f3(f1, "foobar2.txt");
    f3.createFile();

    // exec
    TestVisitor visitor;
    capu::FileTraverser::accept(f1, visitor); // accept visits all files
    EXPECT_EQ(2u, visitor.mCallCount);
    visitor.mCallCount = 0;
    capu::status_t retVal = capu::FileTraverser::accept(f2, visitor); // accept visits no files
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(0u, visitor.mCallCount);

    // cleanup
    f3.remove();
    f2.remove();
    f1.remove();
}

TEST(FileTraverser, acceptOnFile)
{
    // setup
    capu::File f1("foobar.txt");
    f1.createFile();

    // exec
    TestVisitor visitor;
    capu::status_t retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(0u, visitor.mCallCount); // accept visits also if called on a file

    // cleanup
    f1.remove();
}

TEST(FileTraverser, acceptTestOnEmptyDir)
{
    // setup
    capu::File f1("foobarfolder");
    f1.createDirectory();

    // exec
    TestVisitor visitor;
    capu::status_t retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(0u, visitor.mCallCount); // accepts on empty dir doesn't visit anything

    // cleanup
    f1.remove();
}

TEST(FileTraverser, abortTest)
{
    // setup
    capu::File f1("foobarfolder");
    f1.createDirectory();
    capu::File f2(f1, "foobarfolder2");
    f2.createDirectory();
    capu::File f3(f2, "foobar.txt");
    f3.createFile();
    TestVisitor visitor;

    // exec
    visitor.mReturnValue = capu::CAPU_ERROR; // will abort traversal immediately
    capu::status_t retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_ERROR, retVal);
    EXPECT_EQ(1u, visitor.mCallCount); // only foobarfolder, then abort
    visitor.mCallCount = 0;

    visitor.mReturnValue = capu::CAPU_OK; // will do full traversal
    retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(2u, visitor.mCallCount);

    // cleanup
    f3.remove();
    f2.remove();
    f1.remove();
}

TEST(FileTraverser, recursiveTraversalTest)
{
    // setup
    capu::File f1("foobarfolder");
    f1.createDirectory();
    capu::File f2(f1, "foobarfolder2");
    f2.createDirectory();
    capu::File f3(f2, "foobar1.txt");
    f3.createFile();
    capu::File f4(f2, "foobar2.txt");
    f4.createFile();
    TestVisitor visitor(false); // not stepping into directory

    // exec
    capu::status_t retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(1u, visitor.mCallCount); // only foobarfolder2 was visited, then no recursion
    visitor.mCallCount = 0;
    visitor.mStepIntoDirectory = true;

    retVal = capu::FileTraverser::accept(f1, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(3u, visitor.mCallCount); // all files and folders were visited
    visitor.mCallCount = 0;
    visitor.mStepIntoDirectory = false;

    retVal = capu::FileTraverser::accept(f2, visitor);
    EXPECT_EQ(2u, visitor.mCallCount); // only foobar1 and foobar2 was visited

    visitor.mCallCount = 0;
    visitor.mStepIntoDirectory = true;
    retVal = capu::FileTraverser::accept(f2, visitor);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(2u, visitor.mCallCount); // foobar1.txt and foobar2.txt was visited

    // cleanup
    f4.remove();
    f3.remove();
    f2.remove();
    f1.remove();
}

TEST(FileTraverser, acceptOnNonExistingFolderTest)
{
    // setup
    TestVisitor visitor;
    capu::File nonex("somenonexfile");

    // exec
    capu::status_t retVal = capu::FileTraverser::accept(nonex, visitor); // accept does not visit nonexisting files/folders
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_EQ(0u, visitor.mCallCount);
}
