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

#ifndef CAPU_INTEGRITY_TCP_SOCKET_H
#define CAPU_INTEGRITY_TCP_SOCKET_H

#include <capu/os/Posix/TcpSocket.h>

namespace capu
{
    namespace os
    {
        class TcpSocket: private capu::posix::TcpSocket
        {
        public:
            TcpSocket();
            TcpSocket(const capu::os::SocketDescription& socketDescription);
            using capu::posix::TcpSocket::send;
            using capu::posix::TcpSocket::receive;
            using capu::posix::TcpSocket::close;
            using capu::posix::TcpSocket::connect;
            using capu::posix::TcpSocket::setBufferSize;
            using capu::posix::TcpSocket::setLingerOption;
            using capu::posix::TcpSocket::setNoDelay;
            using capu::posix::TcpSocket::setKeepAlive;
            using capu::posix::TcpSocket::setTimeout;
            using capu::posix::TcpSocket::getBufferSize;
            using capu::posix::TcpSocket::getLingerOption;
            using capu::posix::TcpSocket::getNoDelay;
            using capu::posix::TcpSocket::getKeepAlive;
            using capu::posix::TcpSocket::getTimeout;
            using capu::posix::TcpSocket::getRemoteAddress;

        };

        inline
        TcpSocket::TcpSocket()
        {
        }

        inline
        TcpSocket::TcpSocket(const capu::os::SocketDescription& socketDescription)
            : capu::posix::TcpSocket(socketDescription)
        {

        }
    }
}
#endif // CAPU_INTEGRITY_TCP_SOCKET_H
