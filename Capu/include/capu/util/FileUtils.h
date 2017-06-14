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
#include "capu/container/vector.h"

#include CAPU_PLATFORM_INCLUDE(FileUtils)

namespace capu
{
    /**
    * Helper visitor to delete files recursively.
    */
    class RecursiveFileDeleter: public IFileVisitor
    {
    public:
        status_t visit(File& file, bool& stepIntoDirectory)
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
        * Reads all bytes from a file.
        * @param file The file containing the bytes.
        * @param result A vector which the resulting bytes will be appended to.
        * @return CAPU_OK if the file was read successfully.
        */
        static status_t readAllBytes(File& file, vector<Byte>& result);

        /**
        * Writes all given text in a file. Existing content will get overwritten.
        * @param file The file into which to content will get written.
        * @param content The content that should go into the file.
        * @return The return value.
        */
        static status_t writeAllText(File& file, const String& content);

        /**
        * Writes all bytes to a file. Existing content will be overwritten.
        * @param file The file which the content will get written to.
        * @param buffer A pointer to an input buffer with byte data.
        * @param numberOfBytesToWrite The number of bytes to write.
        * @return CAPU_OK if the file was written successfully.
        */
        static status_t writeAllBytes(File& file, const Byte* buffer, uint32_t numberOfBytesToWrite);

        /**
        * Retrieves the current working directory for the calling process
        * @return File object with current working directory
        */
        static File getCurrentWorkingDirectory();

        /**
        * Sets the current working directory for the process
        * @param directory the new working directory
        * @return CAPU_OK if working directory changed, CAPU_ERROR otherwise
        */
        static status_t setCurrentWorkingDirectory(const File& directory);
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
        bool success;
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

    inline status_t FileUtils::readAllBytes(File& file, vector<Byte>& result)
    {
        capu::uint_t fileSize;
        if (file.getSizeInBytes(fileSize) != capu::CAPU_OK)
        {
            // error determining file size
            return CAPU_ERROR;
        }

        result.resize(fileSize);
        file.open(capu::READ_ONLY_BINARY);
        capu::uint_t totalBytesRead = 0;
        status_t retVal = CAPU_ERROR;

        // Read directly into the vector
        char* vectorPtr = reinterpret_cast<char*>(result.data());

        while (totalBytesRead < fileSize)
        {
            capu::uint_t bytesRead = 0;
            retVal = file.read(vectorPtr, fileSize - totalBytesRead, bytesRead);

            if (retVal == capu::CAPU_EOF)
            {
                // read to end
                break;
            }
            if (retVal != capu::CAPU_OK)
            {
                // an error occurred
                break;
            }
            if (bytesRead == 0)
            {
                // read 0 bytes and no EOF
                break;
            }

            totalBytesRead += bytesRead;
            vectorPtr += bytesRead; // Bump the pointer in case end-of-file is not reached yet
        }

        file.close();
        return retVal;
    }

    inline status_t FileUtils::writeAllBytes(File& file, const Byte* buffer, uint32_t numberOfBytesToWrite)
    {
        if (file.exists())
        {
            file.remove();
        }
        file.open(capu::READ_WRITE_OVERWRITE_OLD_BINARY);
        file.write(reinterpret_cast<const char*>(buffer), numberOfBytesToWrite);
        file.flush();
        file.close();
        return CAPU_OK;
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
        char* buffer = new char[fileSize + 1];
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
            String result = String(buffer);
            delete[] buffer;
            return result;
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
        file.write(content.c_str(), content.getLength());
        file.flush();
        file.close();
        return CAPU_OK;
    }

    inline File FileUtils::getCurrentWorkingDirectory()
    {
        return capu::os::arch::FileUtils::getCurrentWorkingDirectory();
    }

    inline status_t FileUtils::setCurrentWorkingDirectory(const File& directory)
    {
        return capu::os::arch::FileUtils::setCurrentWorkingDirectory(directory);
    }
}

#endif /* CAPU_FILEUTILS_H */
