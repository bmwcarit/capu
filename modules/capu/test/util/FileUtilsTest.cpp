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

#include "gmock/gmock.h"
#include "capu/util/FileUtils.h"

TEST(FileUtilsTest, TestRemoveDirectory)
{
    // setup
    capu::File f1("foobarfolder");
    f1.createDirectory();
    capu::File f2(f1, "foobarfolder2");
    f2.createDirectory();
    capu::File f3(f2, "foobar.txt");
    f3.createFile();

    EXPECT_EQ(capu::CAPU_ERROR, f1.remove()); // will not work because folder is not empty

    capu::status_t retVal = capu::FileUtils::removeDirectory(f1);
    EXPECT_EQ(capu::CAPU_OK, retVal);
    EXPECT_FALSE(f1.exists());
}

TEST(FileUtilsTest, CreateAndRemoveDirectories)
{
    // setup
    capu::File temp1234("temp1/temp2/temp3/temp4");

    // execute
    EXPECT_FALSE(temp1234.isDirectory());
    EXPECT_FALSE(temp1234.exists());
    capu::FileUtils::createDirectories(temp1234);
    EXPECT_TRUE(temp1234.exists());
    EXPECT_TRUE(temp1234.isDirectory());
    temp1234.remove();
    EXPECT_FALSE(temp1234.exists());

    // cleanup
    capu::File temp123("temp1/temp2/temp3");
    temp123.remove();
    capu::File temp12("temp1/temp2");
    temp12.remove();
    capu::File temp1("temp1");
    temp1.remove();
}

TEST(FileUtilsTest, CreateDirectoriesOnFile)
{
    capu::File temp("temp.txt");
    temp.createFile();
    EXPECT_EQ(capu::CAPU_ERROR, capu::FileUtils::createDirectories(temp));
    temp.remove();
}

TEST(FileUtilsTest, CreateDirectoriesOnExistingFolder)
{
    capu::File temp("temp");
    temp.createDirectory();
    EXPECT_EQ(capu::CAPU_OK, capu::FileUtils::createDirectories(temp));
    temp.remove();
}

TEST(FileUtilsTest, ReadWriteAllText)
{
    capu::File temp("temp.txt");
    EXPECT_FALSE(temp.exists());

    capu::String content("this is some text\nwith multi lines and\ngerman umlauts öäüß !§$%&/()\r\nas well as some different line breaks and \\ escapes.");
    capu::FileUtils::writeAllText(temp, content);
    EXPECT_TRUE(temp.exists());

    capu::String fromFile = capu::FileUtils::readAllText(temp);

    EXPECT_STREQ(content.c_str(), fromFile.c_str());

    capu::String content2("this is a shorter text");
    capu::FileUtils::writeAllText(temp, content2);
    EXPECT_TRUE(temp.exists());

    capu::String fromFile2 = capu::FileUtils::readAllText(temp);
    EXPECT_STREQ(content2.c_str(), fromFile2.c_str());

    temp.remove();
    EXPECT_FALSE(temp.exists());
}