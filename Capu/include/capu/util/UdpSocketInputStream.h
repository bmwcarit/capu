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

#ifndef CAPU_UDPSOCKETINPUTSTREAM_H
#define CAPU_UDPSOCKETINPUTSTREAM_H

#include <capu/util/SocketInputStream.h>
#include <capu/os/UdpSocket.h>
#include <capu/util/BinaryInputStream.h>

namespace capu
{
    template<uint16_t RCVBUFSIZE = 1450>
    class UdpSocketInputStream: public SocketInputStream
    {
    public:
        UdpSocketInputStream(UdpSocket& socket);

        IInputStream& read(char_t* data, const uint32_t size);

        const SocketAddrInfo& getLastSenderInfo() const;
    protected:

    private:
        UdpSocket&     m_socket;
        SocketAddrInfo m_addrInfo;
        uint32_t       m_currentLeftDataSize;
        char_t*        m_currentPosition;
        char_t         m_receiveBuffer[RCVBUFSIZE];

        void readDataFromInternalBuffer(char_t* data, const uint32_t size);
    };

    template<uint16_t RCVBUFSIZE>
    inline
    const SocketAddrInfo&
    UdpSocketInputStream<RCVBUFSIZE>::getLastSenderInfo() const
    {
        return m_addrInfo;
    }

    template<uint16_t RCVBUFSIZE>
    UdpSocketInputStream<RCVBUFSIZE>::UdpSocketInputStream(UdpSocket& socket)
        : m_socket(socket)
        , m_currentLeftDataSize(0)
        , m_currentPosition(m_receiveBuffer)
    {

    }

    template<uint16_t RCVBUFSIZE>
    IInputStream& UdpSocketInputStream<RCVBUFSIZE>::read(char_t* data, const uint32_t size)
    {
        if (size <= m_currentLeftDataSize)
        {
            readDataFromInternalBuffer(data, size);
        }
        else
        {
            int32_t numBytes = 0;
            uint32_t receivedBytes = 0;

            while (receivedBytes < size)
            {
                mState = m_socket.receive(m_receiveBuffer + receivedBytes, RCVBUFSIZE - receivedBytes, numBytes, &m_addrInfo);
                if (mState != CAPU_OK)
                {
                    break;
                }
                receivedBytes += numBytes;
            }

            m_currentLeftDataSize = receivedBytes;
            m_currentPosition = m_receiveBuffer;

            if (mState == CAPU_OK)
            {
                readDataFromInternalBuffer(data, size);
            }
        }
        return *this;
    }

    template<uint16_t RCVBUFSIZE>
    inline
    void
    UdpSocketInputStream<RCVBUFSIZE>::readDataFromInternalBuffer(char_t* data, const uint32_t size)
    {
        Memory::Copy(data, m_currentPosition, size);
        m_currentPosition += size;
        m_currentLeftDataSize -= size;
    }

}

#endif // CAPU_UDPSOCKETINPUTSTREAM_H
