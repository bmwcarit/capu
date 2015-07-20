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

#include "capu/util/TcpSocketInputStream.h"

namespace capu
{
    TcpSocketInputStream::TcpSocketInputStream(TcpSocket& socket)
        : m_socket(socket)
    {
    }

    TcpSocketInputStream::~TcpSocketInputStream()
    {
    }

    IInputStream& TcpSocketInputStream::read(char* data, const uint32_t size)
    {
        int32_t length = 0;
        uint32_t receivedBytes = 0;
        char* buffer = static_cast<char*>(data);
        while (receivedBytes < size)
        {
            mState =  m_socket.receive(&buffer[receivedBytes], size - receivedBytes, length);
            if (mState != CAPU_OK)
            {
                if (mState == CAPU_ERROR)
                {
                    m_socket.close();
                }
                break;
            }

            if (0 == length && mState == CAPU_OK) // other side closed connection
            {
                mState = CAPU_ERROR;
                break;
            }

            receivedBytes += length;
        }
        return *this;
    }
}
