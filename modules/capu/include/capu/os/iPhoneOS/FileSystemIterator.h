/*
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

#ifndef CAPU_IPHONEOS_FILESYSTEMITERATOR_H
#define CAPU_IPHONEOS_FILESYSTEMITERATOR_H

#include <capu/os/Posix/FileSystemIterator.h>

namespace capu
{
    namespace os
    {
        class FileSystemIterator: private capu::posix::FileSystemIterator
        {
        public:
            FileSystemIterator(capu::File root);

            using capu::posix::FileSystemIterator::next;
            using capu::posix::FileSystemIterator::operator*;
            using capu::posix::FileSystemIterator::operator->;
            using capu::posix::FileSystemIterator::isValid;
            using capu::posix::FileSystemIterator::setStepIntoSubdirectories;
        };

        inline FileSystemIterator::FileSystemIterator(capu::File root)
            : capu::posix::FileSystemIterator(root)
        {
        }

    }
}
#endif // CAPU_IPHONEOS_FILESYSTEMITERATOR_H
