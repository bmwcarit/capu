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

#include <capu/container/Array.h>
#include <capu/util/SocketInputStream.h>

namespace capu
{

    IInputStream&
    SocketInputStream::operator>>(String& value)
    {
        uint32_t strLen = 0;

        operator>>(strLen);
        Array<char> buffer(strLen + 1);

        read(buffer.getRawData(), strLen);
        buffer[strLen] = 0;

        value = buffer.getRawData();

        return *this;
    }
}
