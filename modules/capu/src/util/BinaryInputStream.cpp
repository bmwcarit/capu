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

#include <capu/util/BinaryInputStream.h>
#include <capu/os/Memory.h>


namespace capu
{
    BinaryInputStream::BinaryInputStream(const char_t* buffer)
        : mBuffer(buffer)
        , mCurrent(mBuffer)
    {
    }

    BinaryInputStream::~BinaryInputStream()
    {
    }

    IInputStream& BinaryInputStream::operator>>(int32_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(int32_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(uint32_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint32_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(uint64_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint64_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(int64_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(int64_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(String& value)
    {
        uint32_t length = 0;
        operator>>(length); // first read the length of the string

        char_t* buffer = new char_t[length + 1];

        read(buffer, length);
        buffer[length] = 0; // terminate string

        value = buffer;

        delete[] buffer;
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(bool_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(bool_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(float_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(float_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(uint16_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint16_t));
        return *this;
    }

    IInputStream& BinaryInputStream::operator>>(Guid& value)
    {
        generic_uuid_t fromStream;
        read(reinterpret_cast<char_t*>(&fromStream), sizeof(generic_uuid_t));
        value = fromStream;
        return *this;
    }

    IInputStream& BinaryInputStream::read(char_t* buffer, const uint32_t size)
    {
        Memory::Copy(buffer, mCurrent, size);
        mCurrent += size;
        return *this;
    }

    void BinaryInputStream::reset()
    {
        mCurrent = mBuffer;
    }
}
