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

TEST(File, ConstructorTest)
{
    capu::File* f1 = new capu::File("foobar.txt");
    f1->open(capu::READ_ONLY);
    EXPECT_TRUE(f1 != NULL);
    EXPECT_FALSE(f1->isOpen());
    delete f1;

    capu::File* f2 = new capu::File(capu::String("test.txt"));
    f2->open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(f2 != NULL);
    EXPECT_TRUE(f2->isOpen());
    delete f2;
}

TEST(File, ConstructorParentTest)
{
    capu::File parent("ParentDir");
    capu::File childFile(parent, "childFilename");

    EXPECT_STREQ("childFilename", childFile.getFileName().c_str());
    EXPECT_STREQ("ParentDir/childFilename", childFile.getPath().c_str());
}

TEST(File, CantOpenNonExistingFile)
{
    capu::File* f1 = new capu::File("foobar.txt");
    ASSERT_TRUE(f1 != NULL);
    f1->open(capu::READ_ONLY);
    EXPECT_FALSE(f1->isOpen());
    delete f1;
}

TEST(File, ReadableFileCanBeOpened)
{
    capu::File* f2 = new capu::File("test.txt");
    f2->open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(f2 != NULL);
    EXPECT_TRUE(f2->isOpen());
    delete f2;
}

TEST(File, WriteTest)
{
    capu::char_t buf1[15] = "This is a test";
    capu::status_t status;

    capu::File* f1 = new capu::File("test.txt");
    f1->open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(f1 != NULL);
    EXPECT_TRUE(f1->isOpen());

    // invalid params
    status = f1->write(NULL, 0);
    EXPECT_EQ(capu::CAPU_EINVAL, status);

    // write data
    status = f1->write(buf1, sizeof(buf1) - 1);
    EXPECT_EQ(capu::CAPU_OK, status);

    EXPECT_EQ(capu::CAPU_OK, f1->close());
    EXPECT_EQ(capu::CAPU_OK, f1->remove());

    delete f1;
}

TEST(File, WriteSubstring)
{
    capu::char_t bufWrite[40] = "This is a substring. This is a postfix.";
    capu::char_t bufRead[21];
    capu::uint_t substringSize = 20;
    capu::status_t status;

    capu::File* f1 = new capu::File("test.txt");
    ASSERT_TRUE(f1 != NULL);
    f1->open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(f1->isOpen());

    // write data
    status = f1->write(bufWrite, substringSize);
    EXPECT_EQ(capu::CAPU_OK, status);

    //reopen the file
    f1->close();
    status = f1->open(capu::READ_ONLY);
    EXPECT_EQ(capu::CAPU_OK, status);

    //read data back
    memset(bufRead, 0, sizeof(bufRead));
    capu::uint_t read;
    status = f1->read(bufRead, substringSize, read);
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ(substringSize, read);

    //extract substring
    capu::char_t subString[21];
    capu::StringUtils::Strncpy(subString, sizeof(subString), bufWrite);

    EXPECT_STREQ(subString, bufRead);

    EXPECT_EQ(capu::CAPU_OK, f1->close());
    EXPECT_EQ(capu::CAPU_OK, f1->remove());
    delete f1;
}

TEST(File, CantWriteToFileOpenedReadOnly)
{
    capu::char_t buf1[20] = "This is a test";
    capu::char_t buf2[20];

    capu::status_t status;

    //create the file
    capu::File f1("test.txt");
    f1.open(capu::READ_WRITE_OVERWRITE_OLD);
    f1.close();
    ASSERT_TRUE(f1.exists());

    // open file
    capu::File readOnlyFile("test.txt");
    readOnlyFile.open(capu::READ_ONLY);
    EXPECT_TRUE(readOnlyFile.isOpen());

    // file not opened for writing
    status = readOnlyFile.write(buf2, strlen(buf1));
    EXPECT_EQ(capu::CAPU_ERROR, status);

    //clean up the file
    EXPECT_EQ(capu::CAPU_OK, readOnlyFile.close());
    EXPECT_EQ(capu::CAPU_OK, readOnlyFile.remove());
}

TEST(File, ReadTest)
{
    capu::char_t buf1[20] = "This is a test";
    capu::char_t buf2[20];

    capu::status_t status;
    capu::uint_t read = 0;

    // write data
    capu::File* f2 = new capu::File("test.txt");
    f2->open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(f2 != NULL);
    EXPECT_TRUE(f2->isOpen());

    status = f2->write(buf1, strlen(buf1));
    EXPECT_EQ(capu::CAPU_OK, status);
    delete f2;

    // read data
    capu::File* f3 = new capu::File("test.txt");
    f3->open(capu::READ_ONLY);
    EXPECT_TRUE(f3 != NULL);
    EXPECT_TRUE(f3->isOpen());

    // invalid params
    status = f3->read(NULL, 0, read);
    EXPECT_EQ(capu::CAPU_EINVAL, status);

    read = 0;
    memset(buf2, 0, sizeof(buf2));
    status = f3->read(buf2, strlen(buf1), read);
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ((capu::uint_t)strlen(buf1), read);
    delete f3;

    // read data
    capu::File* f4 = new capu::File("test.txt");
    f4->open(capu::READ_ONLY);
    EXPECT_TRUE(f4 != NULL);
    EXPECT_TRUE(f4->isOpen());

    memset(buf2, 0, sizeof(buf2));
    status = f4->read(buf2, strlen(buf1), read);
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ((capu::uint_t)strlen(buf1), read);
    delete f4;

    // read data Eof
    capu::File* f5 = new capu::File("test.txt");
    f5->open(capu::READ_ONLY);
    EXPECT_TRUE(f5 != NULL);
    EXPECT_TRUE(f5->isOpen());

    read = 0;
    memset(buf2, 0, sizeof(buf2));
    status = f5->read(buf2, sizeof(buf2), read);
    EXPECT_EQ(capu::CAPU_EOF, status);
    EXPECT_EQ((capu::uint_t)strlen(buf1), read);
    EXPECT_EQ(0, strcmp(buf1, buf2));
    delete f5;
}

TEST(File, ReadWriteBinaryTest)
{
    capu::char_t buf1[5];
    capu::char_t buf2[5];
    buf1[0] = 4;
    buf1[1] = 2;
    buf1[2] = 6;
    buf1[3] = 6;
    buf1[4] = 6;
    capu::status_t status;

    // write data
    capu::File f2("test.txt");
    f2.open(capu::WRITE_EXISTING_BINARY);
    EXPECT_TRUE(f2.isOpen());

    status = f2.write(buf1, sizeof(buf1));
    EXPECT_EQ(capu::CAPU_OK, status);
    f2.close();

    // read data
    capu::File f3("test.txt");
    f3.open(capu::READ_EXISTING_BINARY);
    capu::uint_t bytes;
    status = f3.read(buf2, sizeof(buf2), bytes);
    EXPECT_EQ(capu::CAPU_OK, status);
    f3.close();

    EXPECT_EQ(sizeof(buf1), bytes);
    for (capu::uint32_t i = 0; i < sizeof(buf1); i++)
    {
        EXPECT_EQ(buf1[i], buf2[i]);
    }
}

TEST(File, TestCreateAndDelete)
{
    capu::File file("something");

    EXPECT_EQ(capu::CAPU_OK, file.createFile());
    EXPECT_TRUE(file.exists());

    EXPECT_EQ(capu::CAPU_OK, file.remove());
    EXPECT_FALSE(file.exists());
}
TEST(File, TestExists)
{
    capu::File file(".");
    EXPECT_TRUE(file.exists());
}
TEST(File, TestRenameSuccess)
{
    capu::File file("temp");
    EXPECT_EQ(capu::CAPU_OK, file.createFile());
    capu::status_t status = file.renameTo("temp2");
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ(capu::String("temp2"), file.getFileName());

    capu::File file2("temp2");
    EXPECT_TRUE(file2.exists());

    status = file2.renameTo("temp");
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ(capu::String("temp"), file2.getFileName());

    EXPECT_EQ(capu::CAPU_OK, file2.remove());
}

TEST(File, TestRenameFileDoesNotExist)
{
    capu::File file("something");
    capu::status_t status = file.renameTo("somethingElse");
    EXPECT_EQ(capu::CAPU_ERROR, status);
    EXPECT_EQ(capu::String("something"), file.getFileName());
}

TEST(File, TestCopy)
{
    capu::File file("something2");
    capu::File fileDest("something3");
    EXPECT_EQ(capu::CAPU_OK, file.createFile());

    EXPECT_FALSE(fileDest.exists());
    file.copyTo(fileDest.getFileName());
    EXPECT_TRUE(fileDest.exists());

    file.remove();
    fileDest.remove();
}

TEST(File, TestCopyOnExisitingFile)
{
    capu::File file("something2");
    capu::File fileDest("something3");
    EXPECT_EQ(capu::CAPU_OK, file.createFile());
    EXPECT_EQ(capu::CAPU_OK, fileDest.createFile());
    fileDest.open(capu::WRITE_EXISTING_BINARY);
    fileDest.write("hello", 5u);
    fileDest.close();

    file.open(capu::WRITE_EXISTING_BINARY);
    file.write("overridden", 10u);
    file.close();

    EXPECT_EQ(capu::CAPU_OK, file.copyTo(fileDest.getFileName())); // will override the hello bytes.

    EXPECT_TRUE(fileDest.exists());
    capu::uint_t size;
    fileDest.getSizeInBytes(size);
    EXPECT_EQ(10u, size);

    file.remove();
    fileDest.remove();
}

TEST(File, TestCopyOnNotAccessibleDestFile)
{
    capu::File file("something2");
    capu::File fileDest("/foo/something3");
    EXPECT_EQ(capu::CAPU_OK, file.createFile());

    EXPECT_EQ(capu::CAPU_ERROR, file.copyTo(fileDest.getPath()));
    EXPECT_FALSE(fileDest.exists());

    file.remove();
    fileDest.remove();
}

TEST(File, TestCopyOnNotExistingSourceFile)
{
    capu::File file("/foo/something2");
    capu::File fileDest("something3");

    EXPECT_EQ(capu::CAPU_ERROR, file.copyTo(fileDest.getPath()));
    EXPECT_FALSE(fileDest.exists());

    file.remove();
    fileDest.remove();
}

TEST(File, TestGetFilename)
{
    capu::File f1("filename");
    EXPECT_STREQ("filename", f1.getFileName().c_str());

    capu::File f2("parent/filename");
    EXPECT_STREQ("filename", f2.getFileName().c_str());

    capu::File f3("filename.ext");
    EXPECT_STREQ("filename.ext", f3.getFileName().c_str());

    capu::File f4("/parent/filename.ext");
    EXPECT_STREQ("filename.ext", f4.getFileName().c_str());
}
TEST(File, TestGetExtension)
{
    capu::File file("msdia80.dll");
    EXPECT_STREQ("dll", file.getExtension().c_str());

    capu::File file2("myVacation.jpg");
    EXPECT_STREQ("jpg", file2.getExtension().c_str());

    capu::File file3("noEnding");
    EXPECT_STREQ("", file3.getExtension().c_str());
}

TEST(File, TestGetPath)
{
    capu::File f1("./relative");
    EXPECT_STREQ("./relative", f1.getPath().c_str());

    capu::File f2("parent/filename");
    EXPECT_STREQ("parent/filename", f2.getPath().c_str());

    capu::File f3("filename.ext");
    EXPECT_STREQ("filename.ext", f3.getPath().c_str());

    capu::File f4("/absolute/filename.ext");
    EXPECT_STREQ("/absolute/filename.ext", f4.getPath().c_str());
}

TEST(File, TestGetFileSize)
{
    capu::char_t buf1[15] = "This is a test";
    capu::status_t status;
    // make a file with a size>0
    capu::File file("sizetest.txt");
    file.open(capu::READ_WRITE_OVERWRITE_OLD);
    EXPECT_TRUE(file.isOpen());

    status = file.write(buf1, strlen(buf1));
    EXPECT_EQ(capu::CAPU_OK, status);
    file.close();
    capu::uint_t byteSize;
    status = file.getSizeInBytes(byteSize);
    EXPECT_EQ(capu::CAPU_OK, status);
    EXPECT_EQ(14u, byteSize);
}

TEST(File, TestIsDirectory)
{
    capu::File tempFile("temp.txt");
    tempFile.createFile();
    EXPECT_FALSE(tempFile.isDirectory());

    capu::File file2(".");
    EXPECT_TRUE(file2.isDirectory());

    capu::File file3("..");
    EXPECT_TRUE(file3.isDirectory());

    tempFile.remove();
}

TEST(File, CreateAndRemoveDirectory)
{
    capu::File temp("temp");

    EXPECT_FALSE(temp.isDirectory());
    EXPECT_FALSE(temp.exists());
    temp.createDirectory();
    EXPECT_TRUE(temp.exists());
    EXPECT_TRUE(temp.isDirectory());

    temp.remove();
    EXPECT_FALSE(temp.isDirectory());
    EXPECT_FALSE(temp.exists());
}

TEST(File, GetParentFile1)
{
    bool success;
    capu::File temp123("temp1/temp2/temp3");

    capu::File temp12 = temp123.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp2", temp12.getFileName().c_str());

    capu::File temp1 = temp12.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp1", temp1.getFileName().c_str());

    // expect the working dir
    capu::File temp0 = temp1.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_TRUE(temp0.exists());
}

TEST(File, GetParentFile2)
{
    bool success;
    capu::File temp123("/temp1/temp2/temp3");
    capu::File temp12 = temp123.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp2", temp12.getFileName().c_str());

    capu::File temp1 = temp12.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp1", temp1.getFileName().c_str());

    capu::File temp = temp1.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_TRUE(temp.exists());

    capu::File temp0 = temp.getParentFile(success);
    EXPECT_FALSE(success);
}

#ifdef OS_WINDOWS

TEST(File, GetParentFile3)
{
    bool success;
    capu::File temp1234("d:/temp1\\temp2/temp3\\temp4");
    capu::File temp123 = temp1234.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp3", temp123.getFileName().c_str());

    capu::File temp12 = temp123.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp2", temp12.getFileName().c_str());

    capu::File temp1 = temp12.getParentFile(success);
    EXPECT_TRUE(success);
    EXPECT_STREQ("temp1", temp1.getFileName().c_str());

    capu::File temp0 = temp1.getParentFile(success);
    EXPECT_TRUE(success);

    capu::File temp00 = temp0.getParentFile(success);
    EXPECT_FALSE(success);
}

TEST(File, GetParentFile4)
{
    bool success;
    capu::File temp("d:/");
    capu::File temp0 = temp.getParentFile(success);
    EXPECT_FALSE(success);
}

#endif
