/*
 * Copyright (C) 2014 jambit GmbH
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

#ifndef CAPU_IPHONESIMULATOROS_X86_64_TCP_SOCKET_H
#define CAPU_IPHONESIMULATOROS_X86_64_TCP_SOCKET_H

#include <capu/os/iPhoneOS/TcpSocket.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            typedef capu::iphoneos::SocketDescription SocketDescription;

            class TcpSocket: private capu::iphoneos::TcpSocket
            {
            public:
               TcpSocket();
               TcpSocket(const SocketDescription& socketDescription);

                using capu::iphoneos::TcpSocket::send;
                using capu::iphoneos::TcpSocket::receive;
                using capu::iphoneos::TcpSocket::close;
                using capu::iphoneos::TcpSocket::connect;
                using capu::iphoneos::TcpSocket::setBufferSize;
                using capu::iphoneos::TcpSocket::setLingerOption;
                using capu::iphoneos::TcpSocket::setNoDelay;
                using capu::iphoneos::TcpSocket::setKeepAlive;
                using capu::iphoneos::TcpSocket::setTimeout;
                using capu::iphoneos::TcpSocket::getBufferSize;
                using capu::iphoneos::TcpSocket::getLingerOption;
                using capu::iphoneos::TcpSocket::getNoDelay;
                using capu::iphoneos::TcpSocket::getKeepAlive;
                using capu::iphoneos::TcpSocket::getTimeout;
                using capu::iphoneos::TcpSocket::getRemoteAddress;
                using capu::iphoneos::TcpSocket::getSocketDescription;

            };

            inline
            TcpSocket::TcpSocket()
            {
            }

            inline
            TcpSocket::TcpSocket(const SocketDescription& socketDescription)
                : capu::iphoneos::TcpSocket(socketDescription)
            {

            }

        }
    }
}
#endif // CAPU_IPHONESIMULATOROS_X86_64_TCP_SOCKET_H
