/*
 * Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_MACOSX_TCP_SOCKET_H
#define CAPU_MACOSX_TCP_SOCKET_H

#include <capu/os/Posix/TcpSocket.h>

namespace capu
{
    namespace os
    {
        class TcpSocket: private capu::posix::TcpSocket
        {
        public:
            TcpSocket();
            TcpSocket(const SocketDescription& socketDescription);

            status_t connect(const char* dest_addr, uint16_t port);

            using capu::posix::TcpSocket::send;
            using capu::posix::TcpSocket::receive;
            using capu::posix::TcpSocket::close;
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
            using capu::posix::TcpSocket::getSocketDescription;

        private:
            status_t setNoSigPipe();

        };

        inline
        TcpSocket::TcpSocket()
        {
        }

        inline
        TcpSocket::TcpSocket(const SocketDescription& socketDescription)
            : capu::posix::TcpSocket(socketDescription)
        {

        }

        inline
        status_t
        TcpSocket::connect(const char* dest_addr, uint16_t port)
        {
            if ((dest_addr == NULL) || (port == 0))
            {
                return CAPU_EINVAL;
            }
            mSocket = socket(AF_INET, SOCK_STREAM, 0);

            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            else
            {
                status_t status = setPosixSocketParams();
                if (status != CAPU_OK && status != CAPU_EINVAL)
                {
                    return status;
                }

                status = setNoSigPipe();
                if (status != CAPU_OK)
                {
                    return status;
                }

                struct sockaddr_in serverAddress;
                status = getSocketAddr(dest_addr, port, serverAddress);
                if (status != CAPU_OK)
                {
                    return status;
                }

                int32_t res = ::connect(mSocket, (const sockaddr*) &serverAddress, sizeof(serverAddress));
                if (res == -1)
                {
                    return CAPU_SOCKET_ECONNECT;
                }

                return CAPU_OK;
            }
        }

        inline
        status_t
        TcpSocket::setNoSigPipe()
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            int32_t opt = 1;

            if (setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }
    }
}
#endif // CAPU_MACOSX_TCP_SOCKET_H
