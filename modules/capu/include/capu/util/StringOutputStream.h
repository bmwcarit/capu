/*
 * Copyright (C) 2013 BMW Car IT GmbH
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

#ifndef CAPU_STRINGOUTPUTSTREAM_H
#define CAPU_STRINGOUTPUTSTREAM_H

#include <capu/util/IOutputStream.h>

#include <capu/container/Array.h>

namespace capu
{
    /**
     * Streams data to a string
     * The result is only valid after a call of flush()
     * The size of the internal buffer is doubled if the buffer is full
     */
    class StringOutputStream: public IOutputStream
    {
    public:
        StringOutputStream();

        /**
         * @name StringOuputStream implementation
         * @see  IOutputStream
         * @{
         */
        virtual IOutputStream& operator<<(const float_t value);
        virtual IOutputStream& operator<<(const int32_t value);
        virtual IOutputStream& operator<<(const uint32_t value);
        virtual IOutputStream& operator<<(const String& value);
        virtual IOutputStream& operator<<(const bool_t  value);
        virtual IOutputStream& operator<<(const char_t* value);
        virtual IOutputStream& operator<<(const uint16_t value);
        virtual IOutputStream& operator<<(const Guid& value);
        virtual IOutputStream& write(const void* data, const uint32_t size);
        virtual status_t flush();
        /**
         * @}
         */

        /**
         * Returns a const char_t pointer to the content of the stream
         * @return the const char_t pointer to the content of the stream
         */
        const char_t* c_str() const;

        /**
         * Returns the current length of the stream without terminating 0
         * @return length of the stream
         */
        const uint32_t length() const;

    protected:
    private:

        /**
         * The internal buffer of the stream
         */
        Array<char_t> mBuffer;

        /**
         * The current size of the internal string
         */
        uint32_t      mSize;

        /**
         * Resizes the internal buffer to minSize
         * @param minSize to resize the buffer
         */
        void resize(const uint32_t minSize);

        /**
         * Requests a buffer size on the stream
         * If the internal buffer is to small, the buffer will be resized
         * @param size to request on the stream
         */
        void requestSize(const uint32_t size);
    };

    inline
    const char_t*
    StringOutputStream::c_str() const
    {
        return mBuffer.getRawData();
    }

    inline
    const uint32_t
    StringOutputStream::length() const
    {
        return mSize;
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const float_t value)
    {
        char_t buffer[16];
        StringUtils::Sprintf(buffer, 16, "%f", value);
        return operator<<(buffer);
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const int32_t value)
    {
        char_t buffer[11];
        StringUtils::Sprintf(buffer, 11, "%d", value);
        return operator<<(buffer);
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const uint32_t value)
    {
        char_t buffer[11];
        StringUtils::Sprintf(buffer, 11, "%u", value);
        return operator<<(buffer);
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const String& value)
    {
        return write(value.c_str(), static_cast<uint32_t>(value.getLength()));
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const bool_t  value)
    {
        char_t buffer[2];
        StringUtils::Sprintf(buffer, 2, "%u", value);
        return operator<<(buffer);
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const char_t* value)
    {
        const uint_t length = ConstString(value).length();
        return write(value, static_cast<uint32_t>(length));
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const uint16_t value)
    {
        char_t buffer[6];
        StringUtils::Sprintf(buffer, 6, "%u", value);
        return operator<<(buffer);
        return *this;
    }

    inline
    IOutputStream&
    StringOutputStream::operator<<(const Guid& value)
    {
        return operator<<(value.toString());
    }

    inline
    IOutputStream&
    StringOutputStream::write(const void* data, const uint32_t size)
    {
        requestSize(size + 1); // Terminating 0
        Memory::Copy(mBuffer.getRawData() + mSize, data, size);
        mSize += size;
        return *this;
    }

    inline
    status_t
    StringOutputStream::flush()
    {
        mBuffer[mSize] = '\0';
        return CAPU_OK;
    }
}


#endif // CAPU_STRINGOUTPUTSTREAM_H