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

#ifndef CAPU_WINDOWS_FILESYSTEMITERATOR_H
#define CAPU_WINDOWS_FILESYSTEMITERATOR_H

#include "capu/os/Generic/FileSystemIterator.h"
#include "capu/os/File.h"

namespace capu
{
    namespace os
    {
        class FileSystemIterator : private generic::FileSystemIterator<HANDLE>
        {
        public:
            FileSystemIterator(capu::File root);
            ~FileSystemIterator();

            capu::File& operator*();
            capu::File* operator->();
            bool_t next();
            bool_t isValid();

            using generic::FileSystemIterator<HANDLE>::setStepIntoSubdirectories;
        private:

            bool_t oneLevelUp(bool_t& found);

            bool_t stepIntoDirectory(capu::File directory);

            bool_t readEntry();

            bool_t mValid;

            WIN32_FIND_DATA mFindFileData;
        };

        inline FileSystemIterator::FileSystemIterator(capu::File root)
            : mValid(false)
        {
            // initialize the search
            stepIntoDirectory(root);
        }

        inline FileSystemIterator::~FileSystemIterator()
        {
            // cleanup remaining open directories
            HANDLE dir;
            while (mDirectoryStack.pop(&dir) == CAPU_OK)
            {
                FindClose(dir);
            }
        }

        inline bool_t FileSystemIterator::stepIntoDirectory(capu::File directory)
        {
            String searchPattern(directory.getPath());
            searchPattern.append("/*.*");
            HANDLE currFileHandle = FindFirstFile(searchPattern.c_str(), &mFindFileData);
            if (currFileHandle == INVALID_HANDLE_VALUE)
            {
                mValid = false;
                return false;
            }

            mDirectoryStack.push(currFileHandle);
            mCurrentDirectory = directory;

            if (StringUtils::Strcmp(mFindFileData.cFileName, ".") == 0 ||
                    StringUtils::Strcmp(mFindFileData.cFileName, "..") == 0)
            {
                // search further
                if (readEntry())
                {
                    mCurrentFile = capu::File(directory, mFindFileData.cFileName);
                    mValid = true;
                }
                else
                {
                    mCurrentFile = capu::File("");
                    mValid = false;
                }
            }
            else
            {
                mCurrentFile = capu::File(directory, mFindFileData.cFileName);
                mValid = true;
            }
            return mValid;
        }

        inline bool_t FileSystemIterator::oneLevelUp(bool_t& found)
        {
            if (mDirectoryStack.isEmpty())
            {
                return false;
            }

            // close current directory
            HANDLE dir;
            mDirectoryStack.pop(&dir);
            FindClose(dir);

            bool_t success;
            mCurrentDirectory = mCurrentDirectory.getParentFile(success);

            // read upper directory
            found = readEntry();

            return true;
        }

        inline bool_t FileSystemIterator::readEntry()
        {
            if (mDirectoryStack.isEmpty())
            {
                // no directory in stack -> nothing to do
                return 0;
            }

            bool_t found = false;
            do
            {
                HANDLE dir;
                mDirectoryStack.peek(dir);

                BOOL result = FindNextFile(dir, &mFindFileData);
                found = result != 0;
            }
            while (found && (StringUtils::Strcmp(mFindFileData.cFileName, ".") == 0 ||
                             StringUtils::Strcmp(mFindFileData.cFileName, "..") == 0));

            return found;
        }

        inline bool_t FileSystemIterator::next()
        {
            if (!mValid || mDirectoryStack.isEmpty())
            {
                // if current is not valid, next also will be not valid
                mValid = false;
                return false;
            }

            // if current file is a directory and we should traverse
            // sub directories then first go into the directory
            bool_t found;
            if (mRecurseSubDirectories && mCurrentFile.isDirectory())
            {
                found = stepIntoDirectory(mCurrentFile);
            }
            else
            {
                found = readEntry();
            }

            if (!found)
            {
                // no further file found in this directory -> go up one level
                while (oneLevelUp(found))
                {
                    if (found)
                    {
                        // entry found, stop going up
                        break;
                    }
                }
            }

            if (!found)
            {
                // no further files found -> finished
                mCurrentFile = capu::File("");
                mValid = false;
            }
            else
            {
                mCurrentFile = capu::File(mCurrentDirectory, mFindFileData.cFileName);
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

#endif //CAPU_WINDOWS_FILESYSTEMITERATOR_H
