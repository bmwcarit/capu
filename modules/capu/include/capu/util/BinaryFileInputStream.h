/*
 * Copyright 2013 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAPU_BINARYFILEINPUTSTREAM_H
#define CAPU_BINARYFILEINPUTSTREAM_H


#include <capu/os/File.h>
#include <capu/util/BinaryInputStream.h>

namespace capu
{
    class BinaryFileInputStream: public BinaryInputStream
    {
    public:
        BinaryFileInputStream(File& file);
        ~BinaryFileInputStream();

        /**
         * @see BinaryInputStream
         * @{
         */
        virtual IInputStream& read(char_t* data, const uint32_t size);
        /**
         * @}
         */

    protected:
    private:
        File& m_file;
    };

    inline
    BinaryFileInputStream::BinaryFileInputStream(File& file)
        : BinaryInputStream(0)  // no buffer is needed to read from since we read direct from file
        , m_file(file)
    {
        setState(m_file.open(READ_EXISTING_BINARY));
    }

    inline
    BinaryFileInputStream::~BinaryFileInputStream()
    {
        m_file.close();
    }

}

#endif // CAPU_FILEOUTPUTSTREAM_H
