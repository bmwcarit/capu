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

#ifndef CAPU_FILEUTILS_H
#define CAPU_FILEUTILS_H

#include "capu/os/File.h"
#include "capu/util/FileTraverser.h"

namespace capu
{
    /**
    * Helper visitor to delete files recursively.
    */
    class RecursiveFileDeleter: public IFileVisitor
    {
    public:
        status_t visit(File& file, bool_t& stepIntoDirectory)
        {
            stepIntoDirectory = false; // not stepping into, because we do our own recursion!
            if (file.isDirectory())
            {
                // first delete directory contents
                RecursiveFileDeleter deleter;
                status_t retVal = FileTraverser::accept(file, deleter);
                if (retVal != CAPU_OK)
                {
                    return retVal;
                }
            }
            return file.remove(); // stop if file/directory was not removed
        }
    };

    /**
    * Helper methods to work with files.
    */
    class FileUtils
    {
    public:
        /**
        * Removes a directory and all subfiles and subdirectories.
        * @param directory The directory which should get removed.
        * @return CAPU_OK is removal was successful.
        */
        static status_t removeDirectory(File& directory);

        /**
         * Creates the directory and if necessary all parent directories.
         * @return CAPU_OK if directory was created successfully.
         */
        static status_t createDirectories(File& directory);

        /**
        * Reads all text from a file.
        * @param file The file containing the text.
        * @return The file content as string.
        */
        static String readAllText(File& file);

        /**
        * Writes all given text in a file. Existing content will get overwritten.
        * @param file The file into which to content will get written.
        * @param content The content that should go into the file.
        * @return The return value.
        */
        static status_t writeAllText(File& file, const String& content);
    };

    inline status_t FileUtils::removeDirectory(File& directory)
    {
        // first delete all subfiles and folders
        RecursiveFileDeleter deleter;
        FileTraverser::accept(directory, deleter);

        // then delete the directory itself
        return directory.remove();
    }

    inline status_t FileUtils::createDirectories(File& directory)
    {
        if (directory.exists())
        {
            return directory.isDirectory() ? CAPU_OK : CAPU_ERROR;
        }

        // non recursive traversing up until we find an existing directory.
        Stack<String> fileStack;
        bool_t success;
        File parent = directory.getParentFile(success);
        while (success && !parent.exists())
        {
            // bubble up the directories until we find a directory that exists
            fileStack.push(parent.getPath());
            parent = parent.getParentFile(success);
        }
        if (!success)
        {
            // wrong path where no parent exists. This is an error
            return CAPU_ERROR;
        }

        // now bubble down and create all the directories one after the other
        String dirName;
        while (fileStack.pop(&dirName) == CAPU_OK)
        {
            status_t retVal = capu::File(dirName).createDirectory();
            if (retVal != CAPU_OK)
            {
                return retVal;
            }
        }

        // finally, create the directory
        return directory.createDirectory();
    }

    inline String FileUtils::readAllText(File& file)
    {
        // read the file
        capu::uint_t fileSize;
        if (file.getSizeInBytes(fileSize) != capu::CAPU_OK)
        {
            // error determining file size
            return "";
        }
        capu::uint_t readBytes = 0;
        capu::char_t* buffer = new capu::char_t[fileSize + 1];
        capu::Memory::Set(buffer, 0, fileSize + 1);
        file.open(capu::READ_ONLY);
        capu::uint_t totalBytes = 0;
        status_t retVal = CAPU_ERROR;
        while (totalBytes < fileSize)
        {
            retVal = file.read(&buffer[totalBytes], fileSize - totalBytes, readBytes);
            if (retVal == capu::CAPU_EOF)
            {
                // read to end
                break;
            }
            if (retVal != capu::CAPU_OK)
            {
                // an error occurred
                readBytes = 0;
                break;
            }
            if (readBytes <= 0)
            {
                // read 0 bytes and no EOF (possible?)
                break;
            }
            totalBytes += readBytes;
        }
        file.close();
        if (retVal == CAPU_OK || retVal == CAPU_EOF) // read all content
        {
            String retVal = String(buffer);
            delete[] buffer;
            return retVal;
        }
        else
        {
            delete[] buffer;
            return "";
        }
    }

    inline status_t FileUtils::writeAllText(File& file, const String& content)
    {
        if (file.exists())
        {
            file.remove();
        }
        file.open(capu::READ_WRITE_OVERWRITE_OLD);
        file.write(content, content.getLength());
        file.flush();
        file.close();
        return CAPU_OK;
    }
}

#endif /* CAPU_FILEUTILS_H */
