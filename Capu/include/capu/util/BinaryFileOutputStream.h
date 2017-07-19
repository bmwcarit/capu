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

#ifndef CAPU_BINARYFILEOUTPUTSTREAM_H
#define CAPU_BINARYFILEOUTPUTSTREAM_H


#include <capu/os/File.h>
#include <capu/util/BinaryOutputStream.h>

namespace capu
{
    class BinaryFileOutputStream: public BinaryOutputStream
    {
    public:
        BinaryFileOutputStream(File& file, FileMode mode = WRITE_NEW_BINARY);
        ~BinaryFileOutputStream();

        /**
         * @see IOutputStream
         * @{
         */
        virtual IOutputStream& write(const void* data, const uint32_t size) override;
        /**
         * @}
         */

        status_t getState() const;

    protected:
    private:
        File& m_file;
        status_t m_fileState;

        BinaryFileOutputStream operator=(const BinaryFileOutputStream bfos);
    };

    inline
    BinaryFileOutputStream::BinaryFileOutputStream(File& file, FileMode mode)
        : m_file(file)
        , m_fileState(CAPU_OK)
    {
        m_fileState = m_file.open(mode);
    }

    inline
    BinaryFileOutputStream::~BinaryFileOutputStream()
    {
        m_file.flush();
        m_file.close();
    }

    inline status_t BinaryFileOutputStream::getState() const
    {
        return m_fileState;
    }
}

#endif // CAPU_FILEOUTPUTSTREAM_H
