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

#ifndef CAPU_IPHONESIMULATOROS_X86_64_FILESYSTEMITERATOR_H
#define CAPU_IPHONESIMULATOROS_X86_64_FILESYSTEMITERATOR_H

#include <capu/os/iPhoneOS/FileSystemIterator.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class FileSystemIterator: private capu::iphoneos::FileSystemIterator
            {
            public:
                FileSystemIterator(capu::File root);

                using capu::iphoneos::FileSystemIterator::next;
                using capu::iphoneos::FileSystemIterator::operator*;
                using capu::iphoneos::FileSystemIterator::operator->;
                using capu::iphoneos::FileSystemIterator::isValid;
                using capu::iphoneos::FileSystemIterator::setStepIntoSubdirectories;
            };

            inline FileSystemIterator::FileSystemIterator(capu::File root)
                : capu::iphoneos::FileSystemIterator(root)
            {
            }

        }
    }
}
#endif
