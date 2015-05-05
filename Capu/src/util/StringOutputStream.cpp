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

#include <capu/util/StringOutputStream.h>

#include <capu/container/ConstString.h>

namespace capu
{



    StringOutputStream::StringOutputStream()
        : mBuffer(16, 0)
        , mSize(0)
        , mFloatingPointType(NORMAL)
        , mDecimalDigits(6)
    {
    }

    void
    StringOutputStream::resize(const uint32_t minSize)
    {
        uint_t newCapacity = mBuffer.size();
        while (newCapacity < minSize)
        {
            newCapacity *= 2;
        }

        Array<char_t> newBuffer(newCapacity);
        Memory::Copy(newBuffer.getRawData(), mBuffer.getRawData(), mSize);
        swap(mBuffer, newBuffer);
    }

    void
    StringOutputStream::requestSize(const uint32_t size)
    {
        if (mSize + size > mBuffer.size())
        {
            resize(mSize + size);
        }
    }
}
