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

#include <capu/util/BinaryOutputStream.h>

#include <capu/os/Memory.h>
#include <capu/os/StringUtils.h>
#include <capu/util/Swap.h>

namespace capu
{
    BinaryOutputStream::BinaryOutputStream(const uint32_t capacity)
        : mBuffer(capacity)
        , mSize(0)
        , mCapacity(capacity)

    {
    }

    BinaryOutputStream::~BinaryOutputStream()
    {
    }

    void
    BinaryOutputStream::clear()
    {
        mSize = 0;
    }

    IOutputStream&
    BinaryOutputStream::write(const void* data, const uint32_t size)
    {
        requestSize(size);
        Memory::Copy(mBuffer.getRawData() + mSize, data, size);
        mSize += size;
        return *this;
    }

    void
    BinaryOutputStream::resize(const uint32_t minSize)
    {
        while (mCapacity < minSize)
        {
            mCapacity *= 2;
        }

        Array<char_t> newBuffer(mCapacity);
        Memory::Copy(newBuffer.getRawData(), mBuffer.getRawData(), mSize);
        swap(mBuffer, newBuffer);
    }

    void
    BinaryOutputStream::requestSize(const uint32_t size)
    {
        if (mSize + size > mCapacity)
        {
            resize(mSize + size);
        }
    }
}
