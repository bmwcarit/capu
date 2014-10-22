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


#ifndef CAPU_UNIXBASED_TCP_SOCKET_H
#define CAPU_UNIXBASED_TCP_SOCKET_H

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <cstring>
#include <capu/os/Socket.h>
#include <capu/os/Generic/TcpSocket.h>


namespace capu
{
    namespace posix
    {

      class TcpSocket: private capu::generic::TcpSocket
        {
        public:
            TcpSocket();
            TcpSocket(const capu::os::SocketDescription& socketDescription);
            ~TcpSocket();

            status_t send(const char_t* buffer, int32_t length, int32_t& sentBytes);
            status_t receive(char_t* buffer, int32_t length, int32_t& numBytes);
            status_t close();
            status_t connect(const char_t* dest_addr, uint16_t port);

            status_t setTimeout(int32_t timeout);
            status_t getTimeout(int32_t& timeout);

            using capu::generic::TcpSocket::setBufferSize;
            using capu::generic::TcpSocket::setLingerOption;
            using capu::generic::TcpSocket::setNoDelay;
            using capu::generic::TcpSocket::setKeepAlive;
            using capu::generic::TcpSocket::getBufferSize;
            using capu::generic::TcpSocket::getLingerOption;
            using capu::generic::TcpSocket::getNoDelay;
            using capu::generic::TcpSocket::getKeepAlive;
            using capu::generic::TcpSocket::getRemoteAddress;
            using capu::generic::TcpSocket::getSocketDescription;

        protected:
            status_t setPosixSocketParams();
            int32_t mTimeout;
            using capu::generic::TcpSocket::mSocket;
            using capu::generic::TcpSocket::getSocketAddr;

        private:
            status_t setTimeoutInternal();

        };

        inline
        TcpSocket::TcpSocket()
        : mTimeout(-1)
        {
        }

        inline
        TcpSocket::TcpSocket(const capu::os::SocketDescription& socketDescription)
        : capu::generic::TcpSocket()
        , mTimeout(-1)
        {
            mSocket = socketDescription;
        }

        inline
        TcpSocket::~TcpSocket()
        {
            close();
        }

        inline
        status_t
        TcpSocket::send(const char_t* buffer, int32_t length, int32_t& sentBytes)
        {

            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            const int32_t res = ::send(mSocket, buffer, length, 0);

            if (res < 0)
            {
                if (errno == EAGAIN) {
                    return CAPU_ETIMEOUT;
                } else {
                    return CAPU_ERROR;
                }
            }

            sentBytes = res;
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::receive(char_t* buffer, int32_t length, int32_t& numBytes)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            const int32_t res = ::recv(mSocket, buffer, length, 0);

            if (res == -1)
            {
                numBytes = 0;
                if (errno == EAGAIN || errno == EINTR)
                {
                    return CAPU_ETIMEOUT;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }
            numBytes = res;

            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::close()
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            else
            {
                shutdown(mSocket, SHUT_RDWR);
                if (::close(mSocket) < 0)
                {
                    return CAPU_SOCKET_ECLOSE;
                }
                mSocket = -1;
                return CAPU_OK;
            }
        }

        inline
        status_t
        TcpSocket::connect(const char_t* dest_addr, uint16_t port)
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

                struct sockaddr_in serverAddress;
                status = getSocketAddr(dest_addr, port, serverAddress);
                if (status != CAPU_OK) 
                {
                    return status;
                }

                int32_t res = ::connect(mSocket, (const sockaddr*) &serverAddress, sizeof(serverAddress));
                if (res == -1)
                {
                    close();
                    return CAPU_SOCKET_ECONNECT;
                }
            }
            return CAPU_OK;
        }

        inline
        status_t TcpSocket::setPosixSocketParams()
        {
            status_t returnStatus = CAPU_OK;
            status_t status = setSocketParameters();
            if (status != CAPU_OK) {
                returnStatus = status;
            }

            status = setTimeoutInternal();
            if (status != CAPU_OK) {
                returnStatus = status;
            }

            return returnStatus;
        }

        inline
        status_t
        TcpSocket::setTimeout(int32_t timeout)
        {
            mTimeout = timeout;
            if (-1 != mSocket)
            {
                return setTimeoutInternal();
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::setTimeoutInternal()
        {

            if (mTimeout < 0)
            {
                return CAPU_EINVAL;
            }

            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct timeval _timeout;
            _timeout.tv_sec = mTimeout / 1000;
            _timeout.tv_usec = (mTimeout % 1000) * 1000;

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &_timeout, sizeof(_timeout)) < 0)
            {
                return CAPU_ERROR;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, &_timeout, sizeof(_timeout)) < 0)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline status_t TcpSocket::getTimeout(int32_t& timeout)
        {
            if (mSocket == -1)
            {
                timeout = mTimeout;
                return CAPU_OK;
            }

            struct timeval _timeout;

            socklen_t len = sizeof(_timeout);

            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &_timeout, &len) < 0)
            {
                return CAPU_ERROR;
            }

            timeout = _timeout.tv_sec * 1000;
            timeout += _timeout.tv_usec / 1000;

            return CAPU_OK;
        }
    }


}

#endif // CAPU_UNIXBASED_TCP_SOCKET_H

