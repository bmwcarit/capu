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

#ifndef CAPU_UDPSOCKETOUTPUTSTREAM_H
#define CAPU_UDPSOCKETOUTPUTSTREAM_H

#include <capu/util/SocketOutputStream.h>
#include <capu/os/UdpSocket.h>

namespace capu
{
    template<uint16_t SNDBUFSIZE = 1450>
    class UdpSocketOutputStream: public SocketOutputStream<SNDBUFSIZE>
    {
    public:
        UdpSocketOutputStream(UdpSocket& socket, const String& ip, const uint16_t port);
        ~UdpSocketOutputStream();

        SocketAddrInfo& getAddrInfo();
    protected:

        status_t writeToSocket(const char* buffer, const uint32_t size, int32_t&  numBytes);

    private:
        UdpSocket& m_socket;
        SocketAddrInfo m_addrInfo;
    };

    template<uint16_t SNDBUFSIZE>
    inline
    UdpSocketOutputStream<SNDBUFSIZE>::UdpSocketOutputStream(UdpSocket& socket, const String& ip, const uint16_t port)
        : m_socket(socket)
    {
        m_addrInfo.addr = ip;
        m_addrInfo.port = port;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    UdpSocketOutputStream<SNDBUFSIZE>::~UdpSocketOutputStream()
    {
    }

    template<uint16_t SNDBUFSIZE>
    inline
    status_t
    UdpSocketOutputStream<SNDBUFSIZE>::writeToSocket(const char* buffer, const uint32_t size, int32_t&  numBytes)
    {
        status_t status = m_socket.send(buffer, size, m_addrInfo);
        if (status == CAPU_OK)
        {
            numBytes = size;
        }
        return status;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    SocketAddrInfo&
    UdpSocketOutputStream<SNDBUFSIZE>::getAddrInfo()
    {
        return m_addrInfo;
    }
}

#endif // CAPU_UDPSOCKETOUTPUTSTREAM_H
