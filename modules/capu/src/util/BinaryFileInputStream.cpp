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

#include <capu/util/BinaryFileInputStream.h>

namespace capu
{
    IInputStream& BinaryFileInputStream::read(char_t* data, const uint32_t size)
    {
        if (CAPU_OK == m_fileState)
        {
            uint_t readBytes = 0;
            uint_t numBytes = 0;
            while (readBytes < size)
            {
                status_t retVal = m_file.read(data + readBytes, size, numBytes);
                if (retVal != CAPU_OK && retVal != CAPU_EOF)
                {
                    // error reading file, abort read method
                    // EOF is no error, but a valid return value, so we need a special handling here
                    m_fileState = retVal;
                    break;
                }
                readBytes += numBytes;
            }
        }
        return *this;
    }
}