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

#ifndef CAPU_QNX_UDP_SOCKET_H
#define CAPU_QNX_UDP_SOCKET_H

#include <sys/time.h>
#include <capu/os/Posix/UdpSocket.h>

namespace capu
{
    namespace os
    {
        class UdpSocket: private capu::posix::UdpSocket
        {
        public:
            using capu::posix::UdpSocket::bind;
            using capu::posix::UdpSocket::send;
            using capu::posix::UdpSocket::receive;
            using capu::posix::UdpSocket::close;
            using capu::posix::UdpSocket::setBufferSize;
            status_t setTimeout(const int32_t timeout);
            using capu::posix::UdpSocket::getBufferSize;
            using capu::posix::UdpSocket::getTimeout;
            using capu::posix::UdpSocket::getSocketAddrInfo;
        private:
            using capu::posix::UdpSocket::mSocket;
        };

        inline
        status_t
        UdpSocket::setTimeout(const int32_t timeout)
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct timeval soTimeout;
            soTimeout.tv_sec = (timeout + 1) / 1000;
            soTimeout.tv_usec = ((timeout + 1) % 1000) * 1000;

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&soTimeout, sizeof(soTimeout)) == -1)
            {
                return CAPU_ERROR;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&soTimeout, sizeof(soTimeout)) == -1)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }
    }
}
#endif // CAPU_QNX_UDP_SOCKET_H
