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
#include <capu/container/vector.h>

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

        enum HexadecimalType
        {
            NO_HEXADECIMAL,
            HEXADECIMAL_NO_LEADING_ZEROS,
            HEXADECIMAL_LEADING_ZEROS
        };

        StringOutputStream();

        /**
         * @name StringOuputStream implementation
         * @see  IOutputStream
         * @{
         */
        virtual StringOutputStream& operator<<(const float value) override;
        virtual StringOutputStream& operator<<(const int32_t value) override;
        virtual StringOutputStream& operator<<(const uint32_t value) override;
        virtual StringOutputStream& operator<<(const int64_t value) override;
        virtual StringOutputStream& operator<<(const uint64_t value) override;
        virtual StringOutputStream& operator<<(const String& value) override;
        virtual StringOutputStream& operator<<(const bool  value) override;
        virtual StringOutputStream& operator<<(const char* value) override;
        virtual StringOutputStream& operator<<(const char value);
        virtual StringOutputStream& operator<<(const uint16_t value) override;
        virtual StringOutputStream& operator<<(const int16_t value) override;
        virtual StringOutputStream& operator<<(const Guid& value) override;

        virtual StringOutputStream& write(const void* data, const uint32_t size) override;

        virtual status_t flush() override;
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
         * Returns a const char pointer to the content of the stream
         * @return the const char pointer to the content of the stream
         * @{
         */
        const char* c_str() const;
        const char* c_str();
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

        /**
        * Sets the number of decimal digits (right of decimal separator) that are written
        * to stream for float values. If > 45, the value is limited to 45.
        * @param number of decimal digits
        */
        void setDecimalDigits(uint32_t digits);

        /**
        * Tells StringOutputStream to print hex according to integer size.
        * 4 hex digits for u/int32, 8 hex digits for u/int64_t and 2 hex digits for u/int16.
        * @param hexFormat Kind of hexadecimal format to use for printing.
        */
        void setHexadecimalOutputFormat(HexadecimalType hexFormat);

    protected:
    private:

        /**
         * The internal buffer of the stream
         */
        vector<char> mBuffer;

        FloatingPointType mFloatingPointType;

        /**
         * Format of integer numbers
         */
        HexadecimalType mHexadecimalFormat;

        uint32_t mDecimalDigits;

        void increaseSize(const uint32_t size);

    };

    inline
    const char*
    StringOutputStream::c_str()
    {
        return mBuffer.data();
    }

    inline
    const char*
    StringOutputStream::c_str() const
    {
        return mBuffer.data();
    }


    inline
    uint32_t
    StringOutputStream::length() const
    {
        assert(mBuffer.size() > 0);
        return static_cast<uint32_t>(mBuffer.size()) - 1;
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const float value)
    {
        /* Maximum length float value
           biggest/smallest: ~ +/-1e38
           closest to zero:  ~ 1e-38

           required maximum buffer length
           - 1 sign
           - 38 digits left of .
           - 1 for .
           - 38 digits right of dot + maximum 7 more relevant digits
           - 1 for terminating \0
           => 1+38+1+38+7+1 = 86
        */
        char buffer[86];
        switch(mFloatingPointType)
        {
        case NORMAL:
            StringUtils::Sprintf(buffer, sizeof(buffer), "%.*f", mDecimalDigits, value);
            break;
        case FIXED:
            StringUtils::Sprintf(buffer, sizeof(buffer), "%.4f", value);
            break;
        }

        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const int32_t value)
    {
        char buffer[12];

        if(mHexadecimalFormat != NO_HEXADECIMAL)
        {
            uint32_t conv = static_cast<uint32_t>(value);
            return operator<<(conv);
        }
        else
        {
            StringUtils::Sprintf(buffer, sizeof(buffer), "%d", value);
            return operator<<(buffer);
        }
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint32_t value)
    {
        char buffer[11];

        switch(mHexadecimalFormat)
        {
            case HEXADECIMAL_NO_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%X", value);
                break;
            case HEXADECIMAL_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%08X", value);
                break;
            case NO_HEXADECIMAL:
            default:
                StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
                break;
        }

        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const int64_t value)
    {
        char buffer[21];

        if(mHexadecimalFormat != NO_HEXADECIMAL)
        {
            uint64_t conv = static_cast<uint64_t>(value);
            return operator<<(conv);
        }
        else
        {
            StringUtils::Sprintf(buffer, sizeof(buffer), "%lld", value);
            return operator<<(buffer);
        }
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint64_t value)
    {
        char buffer[21];

        switch(mHexadecimalFormat)
        {
            case HEXADECIMAL_NO_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%llX", value);
                break;
            case HEXADECIMAL_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%016llX", value);
                break;
            case NO_HEXADECIMAL:
            default:
                StringUtils::Sprintf(buffer, sizeof(buffer), "%llu", value);
                break;
        }

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
    StringOutputStream::operator<<(const bool  value)
    {
        char buffer[2];
        StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const char* value)
    {
        const uint_t length = ConstString(value).length();
        return write(value, static_cast<uint32_t>(length));
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const char value)
    {
        const uint32_t sizeOfOneCharacter = 1u;
        return write(&value, sizeOfOneCharacter);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const uint16_t value)
    {
        char buffer[7];
        switch(mHexadecimalFormat)
        {
            case HEXADECIMAL_NO_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%hX", value);
                break;
            case HEXADECIMAL_LEADING_ZEROS:
                StringUtils::Sprintf(buffer, sizeof(buffer), "0x%04hX", value);
                break;
            case NO_HEXADECIMAL:
            default:
                StringUtils::Sprintf(buffer, sizeof(buffer), "%u", value);
                break;
        }

        return operator<<(buffer);
    }

    inline
    StringOutputStream&
    StringOutputStream::operator<<(const int16_t value)
    {
        char buffer[7];

        if(mHexadecimalFormat != NO_HEXADECIMAL)
        {
            uint16_t conv = static_cast<uint16_t>(value);
            return operator<<(conv);
        }
        else
        {
            StringUtils::Sprintf(buffer, sizeof(buffer), "%d", value);
            return operator<<(buffer);
        }
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
        const uint_t writeIdx = mBuffer.size() - 1;
        increaseSize(size);
        Memory::Copy(mBuffer.data() + writeIdx, data, size);
        return *this;
    }

    inline
    void
    StringOutputStream::clear()
    {
        mBuffer.resize(1);
        mBuffer[0] = '\0';
    }

    inline
    status_t
    StringOutputStream::flush()
    {
        return CAPU_OK;
    }

    inline
    void
    StringOutputStream::setFloatingPointType(FloatingPointType type)
    {
        mFloatingPointType = type;
    }

    inline
    void StringOutputStream::setDecimalDigits(uint32_t digits)
    {
        if (digits > 45)
        {
            // limit to maximum useful range for IEEE-754 single precision floats
            mDecimalDigits = 45;
        }
        else
        {
            mDecimalDigits = digits;
        }
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
        const char c = '\n';
        outputStream.write(&c, sizeof(char));
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

    inline
    void
    StringOutputStream::setHexadecimalOutputFormat(StringOutputStream::HexadecimalType hexFormat)
    {
        mHexadecimalFormat = hexFormat;
    }

    inline
    void StringOutputStream::increaseSize(const uint32_t increment)
    {
        mBuffer.resize(mBuffer.size() + increment);
    }
}


#endif // CAPU_STRINGOUTPUTSTREAM_H
