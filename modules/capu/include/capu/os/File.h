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

#ifndef CAPU_FILE_H
#define CAPU_FILE_H

#include "capu/Config.h"
#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>

namespace capu
{
    /**
     * Modes for opening files
     */
    enum FileMode
    {
        READ_ONLY,                // opens file for reading
        WRITE_NEW,                // opens file as an empty file for writing
        READ_WRITE_EXISTING,      // opens file for reading and writing. The file must exist
        READ_WRITE_OVERWRITE_OLD, // opens file for reading and writing. Create a new file also of old one exists
        WRITE_EXISTING_BINARY,    // opens file for writing in binary mode
        READ_EXISTING_BINARY      // opens file for reading in binary mode
    };
}

#include CAPU_PLATFORM_INCLUDE(File)

namespace capu
{
    /**
     * Abstract file representation.
     */
    class File: private capu::os::arch::File
    {
    public:

        /**
         * Create a new instance for a file.
         * @param filepath Path of the file
         */
        File(const String& filepath);

        /**
         * Create a new instance for a file.
         * @param parent Parent directory of the file.
         * @param path The path of the file.
         */
        File(const File& parent, const capu::String& path);

        /**
         * try to open the file
         * @param mode to specify the file mode
              READ_ONLY  opens file for reading.
              WRITE_NEW  opens file as an empty file for writing.
              READ_WRITE_EXISTING opens file for reading and writing. The file must exist.
              READ_WRITE_OVERWRITE_OLD opens file for reading and writing. Create a new file also of old one exists.
         */
        status_t open(const FileMode& mode);
        /**
         * create the file
         * @return CAPU_OK if file was created successfully
         */
        status_t createFile();

        /**
         * Create the directory.
         * @return CAPU_OK if directory was created successfully
         */
        status_t createDirectory();

        /**
         * Find out if the file described exists
         * @return true, if file exists otherwise false
         */
        bool_t exists() const;
        /**
         * Delete the file.
         * @return true, if deletion was successfull.
         */
        status_t remove();
        /**
         * Rename the file.
         * @param newPath The new path/filename.
         * @return return CAPU_OK if file was renamed
         *        CAPU_ERROR otherwise
         */
        status_t renameTo(const String& newPath);

        /**
         * Copy the file.
         * @param newPath The filename of the copied file.
         * @return CAPU_OK if file was copied
         *        CAPU_ERROR otherwise
         */
        status_t copyTo(const String& newPath);

        /**
         * return true if file is open else false
         */
        bool_t isOpen();

        /**
         * return true if file end was reached
         */
        bool_t isEof();
        /**
         * Find out if object is a directory.
         * @return True, if File object is a directory.
         */
        bool_t isDirectory() const;

        /**
         * Get the size of the file.
         * @return The size of the file in Bytes
         */
        status_t getSizeInBytes(uint_t&) const;

        /**
         * Reads data from the stream and store it into the buffer.
         * @param buffer elements to be read
         * @param length of the buffer
         * @param numBytes of bytes read from the stream
         * @return CAPU_EINVAL if params are wrong
                  CAPU_EOF    if end of stream
                  CAPU_ERROR  if invalid state or file not open
         */
        status_t read(char_t* buffer, uint_t length, uint_t& numBytes);

        /**
         * Writes the given byte buffer to the stream.
         * @param buffer elements to be written
         * @param length of the buffer
         * @return CAPU_OK buffer could be written to the stream
         *        CAPU_ERROR otherwise
         */
        status_t write(const char_t* buffer, uint_t length);

        /**
         * Writes any unwritten data to the file.
         */
        status_t flush();

        /**
         * Close the stream.
         *@return
         */
        status_t close();

        /**
         * Gets the plain name of the file (the part after the last \\ or /)
         * @return The plain name of the file.
         */
        const capu::String getFileName() const;

        /**
         * Get the current path or filename of the file.
         * @return The current path or filename of the file.
         */
        const capu::String& getPath() const;

        /**
         * Get file extension.
         * @return The filename extension of the file.
         */
        const capu::String getExtension() const;

        /**
        * Get the parent file of the current file.
        * @param success Indicates if a parent file was found.
        * @return The parent file. Returns an invalid file if no parent file was found.
        */
        File getParentFile(bool_t& success) const;

        /**
         * Destruct current instance.
         */
        ~File();
    };

    inline
    File::File(const String& filepath)
        : capu::os::arch::File(filepath)
    {
    }

    inline
    File::File(const File& parent, const capu::String& filepath)
        : capu::os::arch::File(parent, filepath)
    {
    }

    inline
    status_t File::open(const FileMode& mode)
    {
        return capu::os::arch::File::open(mode);
    }

    inline
    File::~File()
    {
    }

    inline
    bool_t
    File::isOpen()
    {
        return capu::os::arch::File::isOpen();
    }

    inline bool_t
    File::isEof()
    {
        return capu::os::arch::File::isEof();
    }

    inline
    status_t
    File::read(char_t* buffer, uint_t length, uint_t& numBytes)
    {
        return capu::os::arch::File::read(buffer, length, numBytes);
    }

    inline
    status_t
    File::write(const char_t* buffer, uint_t length)
    {
        return capu::os::arch::File::write(buffer, length);
    }

    inline
    status_t
    File::flush()
    {
        return capu::os::arch::File::flush();
    }

    inline
    status_t
    File::close()
    {
        return capu::os::arch::File::close();
    }

    inline
    status_t
    File::createFile()
    {
        return capu::os::arch::File::createFile();
    }

    inline
    status_t
    File::createDirectory()
    {
        return capu::os::arch::File::createDirectory();
    }

    inline
    status_t
    File::remove()
    {
        return capu::os::arch::File::remove();
    }

    inline
    bool_t
    File::exists() const
    {
        return capu::os::arch::File::exists();
    }

    inline
    status_t
    File::renameTo(const capu::String& newPath)
    {
        return capu::os::arch::File::renameTo(newPath);
    }

    inline
    status_t
    File::copyTo(const capu::String& newPath)
    {
        return capu::os::arch::File::copyTo(newPath);
    }

    inline
    const capu::String File::getFileName() const
    {
        return capu::os::arch::File::getFileName();
    }

    inline
    const capu::String& File::getPath() const
    {
        return capu::os::arch::File::getPath();
    }

    inline
    capu::status_t File::getSizeInBytes(capu::uint_t& size) const
    {
        return capu::os::arch::File::getSizeInBytes(size);
    }

    inline
    const capu::String File::getExtension() const
    {
        return capu::os::arch::File::getExtension();
    }

    inline
    File File::getParentFile(bool_t& success) const
    {
        capu::String parentPath = capu::os::arch::File::getParentPath(success);
        return File(parentPath);
    }

    inline
    capu::bool_t File::isDirectory() const
    {
        return capu::os::arch::File::isDirectory();
    }
}

#endif // CAPU_FILE_H
