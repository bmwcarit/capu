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

#ifndef CAPU_BINARYOUTPUTSTREAM_H
#define CAPU_BINARYOUTPUTSTREAM_H

#include <capu/util/IOutputStream.h>
#include <capu/container/vector.h>

namespace capu
{
    /**
     * The BinaryOutputStream writes data directly to a stream.
     * The initial size is doubled each time the stream is full
     */
    class BinaryOutputStream: public IOutputStream
    {
    public:
        /**
         * Constructor with initial capacity for the stream
         * @param startSize the initial capacity of the stream
         */
        BinaryOutputStream(const uint32_t startSize = 16);

        /**
         * Write a float into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const float value);

        /**
         * Write an integer into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const int32_t value);

        /**
         * Write an integer into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const uint32_t value);

        /**
         * Write an integer into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const int64_t value);

        /**
         * Write an integer into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const uint64_t value);

        /**
         * Write a string into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const String& value);

        /**
         * Write a bool into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const bool  value);

        /**
         * Write a char into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const char* value);

        /**
         * Write a uint16_t into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const uint16_t value);

        /**
         * Write a int16_t into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const int16_t value);

        /**
         * Write a guid into the stream
         * @param value The variable to write to the stream
         */
        IOutputStream& operator<<(const Guid& value);

        /**
         * Write a number of bytes into the stream
         * @param data The data to write to the stream
         * @param size The number of bytes to write
         */
        IOutputStream& write(const void* data, const uint32_t size);

        /**
         * Returns a pointer to the raw data
         * @return a pointer to the raw data
         */
        const char* getData() const;

        /**
         * Returns the current size in bytes of the stream
         * @return the current size in bytes of the stream
         */
        uint32_t getSize() const;

        /**
         * Returns the current capacity in bytes of the stream
         * @return the current capacity in bytes of the stream
         */
        uint32_t getCapacity() const;

        /**
         * Clears the stream and resets the size to 0. The current capacity is not changed
         */
        void clear();

        /**
         * @see IOutputStream
         * @{
         */
        status_t flush();
        /**
         * @}
         */

    private:

        /**
         * Point to the internal stream data
         */
        vector<char> mBuffer;
    };

    inline
    const char*
    BinaryOutputStream::getData() const
    {
        return mBuffer.data();
    }

    inline
    uint32_t
    BinaryOutputStream::getSize() const
    {
        return static_cast<uint32_t>(mBuffer.size());
    }

    inline
    uint32_t
    BinaryOutputStream::getCapacity() const
    {
        return static_cast<uint32_t>(mBuffer.capacity());
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const float value)
    {
        return write(&value, sizeof(float));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const uint16_t value)
    {
        return write(&value, sizeof(uint16_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const int16_t value)
    {
        return write(&value, sizeof(int16_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const int32_t value)
    {
        return write(&value, sizeof(int32_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const uint32_t value)
    {
        return write(&value, sizeof(uint32_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const int64_t value)
    {
        return write(&value, sizeof(int64_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const uint64_t value)
    {
        return write(&value, sizeof(uint64_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const Guid& value)
    {
        return write(&value.getGuidData(), sizeof(generic_uuid_t));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const String& value)
    {
        operator<<(static_cast<uint32_t>(value.getLength())); // first write length of string
        return write(value.c_str(), static_cast<uint32_t>(value.getLength()));
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const char* value)
    {
        const uint32_t len = static_cast<uint32_t>(StringUtils::Strlen(value));
        operator<<(len); // first write length of string
        return write(value, len);
    }

    inline
    IOutputStream&
    BinaryOutputStream::operator<<(const bool value)
    {
        return write(reinterpret_cast<const char*>(&value), sizeof(bool));
    }

    inline
    IOutputStream&
    BinaryOutputStream::write(const void* data, const uint32_t size)
    {
        const char* dataCharptr = static_cast<const char*>(data);
        mBuffer.insert(mBuffer.end(), dataCharptr, dataCharptr+size);
        return *this;
    }

    inline
    status_t
    BinaryOutputStream::flush()
    {
        // no flushing necessary since the binary stream is always flushed
        return CAPU_OK;
    }

    inline
    void
    BinaryOutputStream::clear()
    {
        mBuffer.resize(0);
    }
}

#endif // CAPU_BINARYOUTPUTSTREAM_H
