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

#ifndef CAPU_UNIXBASED_FILE_H
#define CAPU_UNIXBASED_FILE_H

#include "capu/os/Generic/File.h"
#include "capu/os/FileMode.h"
#include <sys/stat.h>
#include <unistd.h>
#include <climits>
#include <libgen.h>

namespace capu
{
    namespace posix
    {
        class File : private generic::File
        {
        public:
            File(const String& path);
            File(const File& parent, const capu::String& path);
            status_t open(const FileMode& mode);
            bool_t isOpen();
            bool_t isEof();
            status_t read(char_t* buffer, uint_t length, uint_t& numBytes);
            status_t write(const char_t* buffer, uint_t length);
            status_t flush();
            status_t close();
            status_t renameTo(const capu::String& newName);
            status_t createFile();
            status_t createDirectory();
            status_t remove();
            using generic::File::getFileName;
            using generic::File::getExtension;
            using generic::File::getPath;
            String getParentPath(bool_t& success) const;
            status_t getSizeInBytes(uint_t& size) const;
            bool_t isDirectory() const;
            bool_t exists() const;
            status_t copyTo(const capu::String& newPath);
            ~File();
        protected:
            using generic::File::mPath;
        private:
            static String GetParentPathPrivate(String path, bool_t& success);
            static String StripLastPathComponent(const String& path);
            bool_t  mIsOpen;
            FILE*   mHandle;
        };

        inline
        File::File(const String& path)
            : generic::File(path)
            , mIsOpen(false)
            , mHandle(NULL)
        {
        }

        inline
        File::File(const File& parent, const capu::String& path)
            : generic::File(parent, path)
            , mIsOpen(false)
            , mHandle(NULL)
        {
        }

        inline
        File::~File()
        {
            if (mHandle != NULL)
            {
                fclose(mHandle);
            }
        }

        inline
        status_t File::getSizeInBytes(uint_t& size) const
        {
            struct stat tmp;
            int_t status = stat(mPath.c_str(), &tmp);
            if (status != 0)
            {
                return CAPU_ERROR;
            }
            size = tmp.st_size;
            return CAPU_OK;
        }

        inline
        bool_t File::isDirectory() const
        {
            struct stat tmp;
            return stat(mPath.c_str(), &tmp) == 0 && S_ISDIR(tmp.st_mode);
        }

        inline
        status_t File::createFile()
        {
            FILE* handle = fopen(mPath.c_str(), "w");
            if (handle == NULL)
            {
                return CAPU_ERROR;
            }

            fclose(handle);
            return CAPU_OK;
        }

        inline
        status_t File::createDirectory()
        {
            int_t status = mkdir(mPath.c_str(), 0777);
            if (status != 0)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t File::remove()
        {
            int_t status = -1;
            if (isDirectory())
            {
                status = rmdir(mPath.c_str());
            }
            else
            {
                status = ::remove(mPath.c_str());
            }

            if (status == 0)
            {
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }

        inline
        bool_t File::exists() const
        {
            struct stat fileStats;
            int_t status = stat(mPath.c_str(), &fileStats);
            if (status != 0)
            {
                return false;
            }
            return true;
        }

        inline
        status_t File::renameTo(const capu::String& newName)
        {
            int_t status = rename(mPath.c_str(), newName.c_str());
            if (status != 0)
            {
                return CAPU_ERROR;
            }
            mPath = newName;
            return CAPU_OK;
        }

        inline
        status_t File::open(const FileMode& mode)
        {
            // try to open file
            const char_t* flags = "";
            switch (mode)
            {
            case READ_ONLY:
                flags = "r";
                break;
            case WRITE_NEW:
                flags = "r+";
                break;
            case READ_WRITE_EXISTING:
                flags = "w";
                break;
            case READ_WRITE_OVERWRITE_OLD:
                flags = "w+";
                break;
            case WRITE_EXISTING_BINARY:
                flags = "w+b";
                break;
            case READ_EXISTING_BINARY:
                flags = "r+b";
                break;
            default:
                flags = "";
            }
            mHandle  = fopen(mPath.c_str(), flags);
            if (mHandle != NULL)
            {
                mIsOpen = true;
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }

        inline
        bool_t
        File::isOpen()
        {
            return mIsOpen;
        }

        inline
        bool_t
        File::isEof()
        {
            if (mHandle == NULL)
            {
                return false;
            }
            return (feof(mHandle) != 0);
        }

        inline
        status_t
        File::read(char* buffer, uint_t length, uint_t& numBytes)
        {
            if (buffer == NULL)
            {
                return CAPU_EINVAL;
            }
            if (mHandle == NULL)
            {
                return CAPU_ERROR;
            }

            size_t result = fread(buffer, 1, length, mHandle);

            if (result == length)
            {
                numBytes = result;
                return CAPU_OK;
            }

            if (feof(mHandle))
            {
                numBytes = result;
                return CAPU_EOF;
            }
            return CAPU_ERROR;
        }

        inline
        status_t
        File::write(const char* buffer, uint_t length)
        {
            if (buffer == NULL)
            {
                return CAPU_EINVAL;
            }
            if (mHandle == NULL)
            {
                return CAPU_ERROR;
            }

            size_t result = fwrite(buffer, 1, length, mHandle);

            if (result != length)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        File::flush()
        {
            if (mHandle != NULL)
            {
                int_t error = fflush(mHandle);
                if (error == 0)
                {
                    return CAPU_OK;
                }
            }
            return CAPU_ERROR;
        }

        inline
        status_t
        File::close()
        {
            if (mHandle != NULL)
            {
                fclose(mHandle);
                mHandle = NULL;
                mIsOpen = false;
                return CAPU_OK;
            }
            return CAPU_ERROR;
        }

        inline
        String
        File::GetParentPathPrivate(String path, bool_t& success)
        {
            // trim tailing slashes
            while (path.getLength() >= 2 &&
                    path.rfind('/') == static_cast<int_t>(path.getLength() - 1))
            {
                String newPath(path.c_str(), 0, path.getLength() - 1);
                swap(path, newPath);
            }

            // treat empty path as working directory
            if (path.getLength() == 0)
            {
                path = ".";
            }

            // check for some special cases
            if (path == String("/"))
            {
                // path is root -> parent doesn't exist
                success = false;
                return "";
            }
            else if (path == String("."))
            {
                // path is .
                // -> replace with current working directory for dirname to work correctly
                char_t buffer[PATH_MAX];
                success = getcwd(buffer, sizeof(buffer)) != NULL;
                if (!success)
                {
                    return "";
                }
                path = buffer;
            }
            else if (path == String(".."))
            {
                // path is ..
                // -> replace with parent of current working directory
                path = GetParentPathPrivate(".", success);
                if (!success)
                {
                    return "";
                }
            }

            success = true;
            return StripLastPathComponent(path);
        }

        inline
        String
        File::getParentPath(bool_t& success) const
        {
            return GetParentPathPrivate(getPath(), success);
        }

        inline status_t File::copyTo(const capu::String& newPath)
        {
            FILE* handleFrom = fopen(mPath.c_str(), "r");
            if (!handleFrom)
            {
                return CAPU_ERROR;
            }

            // make sure file is not present
            File destFile(newPath);
            destFile.remove();

            FILE* handleTo = fopen(newPath.c_str(), "w");
            if (!handleTo)
            {
                fclose(handleFrom);
                return CAPU_ERROR;
            }
            capu::char_t copybuffer[1024];
            size_t bytesRead = 0;
            while ((bytesRead = fread(copybuffer, sizeof(capu::char_t), sizeof(copybuffer), handleFrom)) > 0)
            {
                if (fwrite(copybuffer, sizeof(capu::char_t), bytesRead, handleTo) != bytesRead)
                {
                    fclose(handleFrom);
                    fclose(handleTo);
                    return CAPU_ERROR;
                }
            }
            fclose(handleFrom);
            fclose(handleTo);
            return (bytesRead == 0) ? CAPU_OK : CAPU_ERROR;
        }

        inline String File::StripLastPathComponent(const String& path)
        {
            capu::String tmp = path;
            capu::int_t lastSlashIndex = path.rfind('/');
            if (lastSlashIndex == -1)
            {
                return capu::String(".");
            }

            if (lastSlashIndex == 0)
            {
                lastSlashIndex = 1; // found the root folder
            }
            tmp.truncate(lastSlashIndex);
            return tmp;
        }
    }
}

#endif // CAPU_UNIXBASED_FILE_H
