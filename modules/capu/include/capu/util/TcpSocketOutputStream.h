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

#ifndef CAPU_TCPSOCKETOUTPUTSTREAM_H
#define CAPU_TCPSOCKETOUTPUTSTREAM_H

#include <capu/util/SocketOutputStream.h>
#include <capu/os/TcpSocket.h>


namespace capu
{
    template<uint16_t SNDBUFSIZE = 1450>
    class TcpSocketOutputStream: public SocketOutputStream<SNDBUFSIZE>
    {
    public:
        TcpSocketOutputStream(TcpSocket& socket);
        ~TcpSocketOutputStream();
    protected:

        status_t writeToSocket(const char_t* buffer, const uint32_t size, int32_t&  numBytes);
    private:
        TcpSocket& m_socket;
    };

    template<uint16_t SNDBUFSIZE>
    inline
    TcpSocketOutputStream<SNDBUFSIZE>::TcpSocketOutputStream(TcpSocket& socket)
        : m_socket(socket)
    {
    }

    template<uint16_t SNDBUFSIZE>
    inline
    TcpSocketOutputStream<SNDBUFSIZE>::~TcpSocketOutputStream()
    {
    }

    template<uint16_t SNDBUFSIZE>
    inline
    status_t
    TcpSocketOutputStream<SNDBUFSIZE>::writeToSocket(const char_t* buffer, const uint32_t size, int32_t&  numBytes)
    {
        return m_socket.send(buffer, size, numBytes);
    }
}

#endif // CAPU_TCPSOCKETOUTPUTSTREAM_H
