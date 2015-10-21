/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_INTEGRITY_X86_64_TCP_SOCKET_H
#define CAPU_INTEGRITY_X86_64_TCP_SOCKET_H

#include <capu/os/Integrity/TcpSocket.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class TcpSocket: private capu::os::TcpSocket
            {
            public:
                TcpSocket();
                TcpSocket(const capu::os::SocketDescription& socketDescription);
                using capu::os::TcpSocket::send;
                using capu::os::TcpSocket::receive;
                using capu::os::TcpSocket::close;
                using capu::os::TcpSocket::connect;
                using capu::os::TcpSocket::setBufferSize;
                using capu::os::TcpSocket::setLingerOption;
                using capu::os::TcpSocket::setNoDelay;
                using capu::os::TcpSocket::setKeepAlive;
                using capu::os::TcpSocket::setTimeout;
                using capu::os::TcpSocket::getBufferSize;
                using capu::os::TcpSocket::getLingerOption;
                using capu::os::TcpSocket::getNoDelay;
                using capu::os::TcpSocket::getKeepAlive;
                using capu::os::TcpSocket::getTimeout;
                using capu::os::TcpSocket::getRemoteAddress;
                using capu::os::TcpSocket::getSocketDescription;

            };

            inline
            TcpSocket::TcpSocket()
            {
            }

            inline
            TcpSocket::TcpSocket(const capu::os::SocketDescription& socketDescription)
                : capu::os::TcpSocket(socketDescription)
            {

            }



        }
    }
}
#endif // CAPU_INTEGRITY_X86_64_TCP_SOCKET_H
