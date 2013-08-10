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

        enum FloatingPointType
        {
            NORMAL,
            FIXED
        };

        StringOutputStream();

        /**
         * @name StringOuputStream implementation
         * @see  IOutputStream
         * @{
         */
        virtual StringOutputStream& operator<<(const float_t value);
        virtual StringOutputStream& operator<<(const int32_t value);
        virtual StringOutputStream& operator<<(const uint32_t value);
        virtual StringOutputStream& operator<<(const int64_t value);
        virtual StringOutputStream& operator<<(const uint64_t value);
        virtual StringOutputStream& operator<<(const String& value);
        virtual StringOutputStream& operator<<(const bool_t  value);
        virtual StringOutputStream& operator<<(const char_t* value);
        virtual StringOutputStream& operator<<(const uint16_t value);
        virtual StringOutputStream& operator<<(const Guid& value);

        virtual StringOutputStream& write(const void* data, const uint32_t size);

        virtual status_t flush();
        /**
         * @}
         */

        /**
         * This method accepts pointer to method which itself accepts StringOuputStream
         * The method is used for endl and fixed
         * @param pointer to method
         * @return stream
         */
        StringOutputStream& operator<< (StringOutputStream& (*pf)(StringOutputStream&));


        /**
         * Returns a const char_t pointer to the content of the stream
         * @return the const char_t pointer to the content of the stream
         * @{
         */
        const char_t* c_str() const;
        const char_t* c_str();
        /**
         * @}
         */


        /**
         * Returns the current length of the stream without terminating 0
         * @return length of the stream
         */
        uint32_t length() const;

        /**
         * Clears the stream
         */
        void clear();

        void setFloatingPointType(FloatingPointType type);

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

        FloatingPointType mFloatingPointType;

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
    StringOutputStream::c_str()
    {
        flush();
        return mBuffer.getRawData();
    }

    inline
    const char_t*
    StringOutputStream::c_str() const
    {
        return mBuffer.getRawData();
    }


    inline
    uint32_t
    StringOutputStream::length() const
    {
        return mSize;
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const float_t value)
    {
        char_t buffer[16];
        switch(mFloatingPointType)
        {
        case NORMAL:
            StringUtils::Sprintf(buffer, sizeof(buffer), "%f", value);
            break;
        case FIXED:
            StringUtils::Sprintf(buffer, 16, "%.4f", value);
            break;
        }

        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const int32_t value)
    {
        char_t buffer[11];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%d", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint32_t value)
    {
        char_t buffer[11];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const int64_t value)
    {
        char_t buffer[21];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%lld", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint64_t value)
    {
        char_t buffer[21];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%llu", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const String& value)
    {
        return write(value.c_str(), static_cast<uint32_t>(value.getLength()));
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const bool_t  value)
    {
        char_t buffer[2];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const char_t* value)
    {
        const uint_t length = ConstString(value).length();
        return write(value, static_cast<uint32_t>(length));
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint16_t value)
    {
        char_t buffer[6];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
        return operator<<(buffer);
        return *this;
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const Guid& value)
    {
        return operator<<(value.toString());
    }

    inline
    StringOutputStream&
    StringOutputStream::write(const void* data, const uint32_t size)
    {
        requestSize(size + 1); // Terminating 0
        Memory::Copy(mBuffer.getRawData() + mSize, data, size);
        mSize += size;
        return *this;
    }

    inline
    void
    StringOutputStream::clear()
    {
        mSize = 0;
        mBuffer[0] = '\0';
    }

    inline
    status_t
    StringOutputStream::flush()
    {
        mBuffer[mSize] = '\0';
        return CAPU_OK;
    }

    inline
    void
    StringOutputStream::setFloatingPointType(FloatingPointType type)
    {
        mFloatingPointType = type;
    }

    inline
    StringOutputStream& StringOutputStream::operator<< (StringOutputStream& (*pf)(StringOutputStream&))
    {
        return pf(*this);
    }

    inline
    StringOutputStream&
    endl(StringOutputStream& outputStream)
    {
        const char_t c = '\n';
        outputStream.write(&c, sizeof(char_t));
        outputStream.flush();
        return outputStream;
    }

    inline
    StringOutputStream&
    fixed(StringOutputStream& outputStream)
    {
        outputStream.setFloatingPointType(StringOutputStream::FIXED);
        return outputStream;
    }
}


#endif // CAPU_STRINGOUTPUTSTREAM_H
