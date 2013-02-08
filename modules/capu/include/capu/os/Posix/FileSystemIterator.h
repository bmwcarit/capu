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

#ifndef CAPU_UNIXBASED_FILESYSTEMITERATOR_H
#define CAPU_UNIXBASED_FILESYSTEMITERATOR_H

#include "capu/os/Generic/FileSystemIterator.h"

#include <dirent.h>

namespace capu
{
    namespace posix
    {
        class FileSystemIterator : private generic::FileSystemIterator<DIR*>
        {
        public:
            FileSystemIterator(capu::File root);
            ~FileSystemIterator();

            capu::File& operator*();
            capu::File* operator->();
            bool_t next();
            bool_t isValid();

            using generic::FileSystemIterator<DIR*>::setStepIntoSubdirectories;
        private:

            bool_t oneLevelUp(dirent** direntry);

            dirent* readEntry();

            bool_t mValid;
        };

        inline FileSystemIterator::FileSystemIterator(capu::File root)
            : mValid(false)
        {
            // initialize the search
            DIR* dir = opendir(root.getPath().c_str());
            if (!dir)
            {
                // no directory, no traversal
                mValid = false;
                return;
            }

            // remember parent dirs in stack
            mDirectoryStack.push(dir);

            // read first entry
            mCurrentDirectory = root;
            mValid = true;
            next();
        }

        inline FileSystemIterator::~FileSystemIterator()
        {
            // cleanup remaining open directories
            DIR* dir;
            while (mDirectoryStack.pop(&dir) == CAPU_OK)
            {
                closedir(dir);
            }
        }

        inline bool_t FileSystemIterator::oneLevelUp(dirent** direntry)
        {
            if (mDirectoryStack.isEmpty())
            {
                return false;
            }

            // close current directory
            DIR* dir;
            mDirectoryStack.pop(&dir);
            closedir(dir);

            // correct reference directory
            bool_t success;
            mCurrentDirectory = mCurrentDirectory.getParentFile(success);

            // read upper directory
            *direntry = readEntry();

            return true;
        }

        inline dirent* FileSystemIterator::readEntry()
        {
            if (mDirectoryStack.isEmpty())
            {
                // no directory in stack -> nothing to do
                return 0;
            }

            dirent* entry = 0;
            do
            {
                DIR* dir = 0;
                mDirectoryStack.peek(dir);
                entry = readdir(dir);
            }
            while (entry &&
                    (
                        StringUtils::Strcmp(entry->d_name, ".") == 0 ||
                        StringUtils::Strcmp(entry->d_name, "..") == 0
                    )
                  ); // ignore . and ..

            return entry;
        }

        inline bool_t FileSystemIterator::next()
        {
            if (!mValid || mDirectoryStack.isEmpty())
            {
                // if current is not valid, next also will be not valid
                return false;
            }

            // if current file is a directory and we should traverse
            // sub directories then first go into the directory
            if (mRecurseSubDirectories && mCurrentFile.isDirectory())
            {
                DIR* dir = opendir(mCurrentFile.getPath().c_str());
                mCurrentDirectory = mCurrentFile;
                mDirectoryStack.push(dir);
            }

            dirent* direntry = readEntry();
            if (!direntry)
            {
                // no further file found in this directory -> go up one level
                while (oneLevelUp(&direntry))
                {
                    if (direntry)
                    {
                        // entry found, stop going up
                        break;
                    }
                }
            }

            if (!direntry)
            {
                // no further files found -> finished
                mCurrentFile = capu::File("");
                mValid = false;
            }
            else
            {
                mCurrentFile = capu::File(mCurrentDirectory, direntry->d_name);
                mValid = true;
            }
            return mValid;
        }

        inline bool_t FileSystemIterator::isValid()
        {
            return mValid;
        }

        inline capu::File& FileSystemIterator::operator*()
        {
            return mCurrentFile;
        }

        inline capu::File* FileSystemIterator::operator->()
        {
            return &mCurrentFile;
        }
    }
}

#endif // CAPU_UNIXBASED_FILESYSTEMITERATOR_H
