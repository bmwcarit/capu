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

#ifndef CAPU_TCPSOCKETINPUTSTREAM_H
#define CAPU_TCPSOCKETINPUTSTREAM_H

#include <capu/util/SocketInputStream.h>
#include <capu/os/TcpSocket.h>

namespace capu
{
    class TcpSocketInputStream: public SocketInputStream
    {
    public:
        TcpSocketInputStream(TcpSocket& socket);
        ~TcpSocketInputStream();

        IInputStream& read(char_t* data, const uint32_t size);

    protected:


    private:
        TcpSocket& m_socket;
    };
}

#endif // CAPU_TCPSOCKETINPUTSTREAM_H
