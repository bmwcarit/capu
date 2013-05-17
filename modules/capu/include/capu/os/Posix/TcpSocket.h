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

#include <capu/os/Socket.h>
#include <cstring>
#include <netinet/tcp.h>

namespace capu
{
    namespace posix
    {
        typedef int32_t SocketDescription;

        class TcpSocket
        {
        public:
            TcpSocket();
            TcpSocket(const SocketDescription& socketDescription);
            ~TcpSocket();

            status_t send(const char_t* buffer, int32_t length, int32_t& sentBytes);
            status_t receive(char_t* buffer, int32_t length, int32_t& numBytes);
            status_t close();
            status_t connect(const char_t* dest_addr, uint16_t port);
            status_t setBufferSize(int32_t bufferSize);
            status_t setLingerOption(bool_t isLinger, int32_t linger);
            status_t setNoDelay(bool_t noDelay);
            status_t setKeepAlive(bool_t keepAlive);
            status_t setTimeout(int32_t timeout);
            status_t getBufferSize(int32_t& bufferSize);
            status_t getLingerOption(bool_t& isLinger, int32_t& linger);
            status_t getNoDelay(bool_t& noDelay);
            status_t getKeepAlive(bool_t& keepAlive);
            status_t getTimeout(int32_t& timeout);
            status_t getRemoteAddress(char_t** address);

        protected:
            int32_t mSocket;
            int32_t mTimeout;
        private:
            struct sockaddr_in mServerAddress;
            struct hostent* mServer;

            int32_t mBufferSize;
            bool_t  mIsLinger;
            int32_t mLinger;
            bool_t  mNoDelay;
            bool_t  mKeepAlive;


            status_t setBufferSizeInternal();
            status_t setLingerOptionInternal();
            status_t setNoDelayInternal();
            status_t setKeepAliveInternal();
            status_t setTimeoutInternal();
        };

        // cppcheck-suppress uninitMemberVar
        inline TcpSocket::TcpSocket()
            : mSocket(-1)
            , mTimeout(-1)
            , mServer(NULL)
            , mBufferSize(-1)
            , mIsLinger(false)
            , mLinger(0)
            , mNoDelay(false)
            , mKeepAlive(false)
        {

        }

        // cppcheck-suppress uninitMemberVar
        inline TcpSocket::TcpSocket(const SocketDescription& socketDescription)
            : mSocket(socketDescription)
            , mTimeout(-1)
            , mServer(NULL)
            , mBufferSize(-1)
            , mIsLinger(false)
            , mLinger(0)
            , mNoDelay(false)
            , mKeepAlive(false)
        {
        }

        inline
        TcpSocket::~TcpSocket()
        {
            close();
        }

        inline
        status_t
        TcpSocket::setBufferSize(int32_t bufferSize)
        {
            mBufferSize = bufferSize;
            if (-1 != mSocket)
            {
                return setBufferSizeInternal();
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setLingerOption(bool_t isLinger, int32_t linger)
        {
            mIsLinger = isLinger;
            mLinger   = linger;
            if (-1 != mSocket)
            {
                return setLingerOptionInternal();
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setNoDelay(bool_t noDelay)
        {
            mNoDelay = noDelay;
            if (-1 != mSocket)
            {
                return setNoDelayInternal();
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setKeepAlive(bool_t keepAlive)
        {
            mKeepAlive = keepAlive;
            if (-1 != mSocket)
            {
                return setKeepAliveInternal();
            }
            return CAPU_OK;
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
                return CAPU_ERROR;
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

            int32_t res = ::recv(mSocket, buffer, length, 0);

            if (res == -1)
            {
                numBytes = 0;
                if (errno == EAGAIN)
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
                setBufferSizeInternal();
                setLingerOptionInternal();
                setNoDelayInternal();
                setKeepAliveInternal();
                setTimeoutInternal();

                mServer = gethostbyname(dest_addr);
                if (mServer == NULL)
                {
                    return CAPU_SOCKET_EADDR;
                }
                // for padding
                memset((char*) &mServerAddress, 0x00, sizeof(mServerAddress));
                mServerAddress.sin_family = AF_INET;
                //destination ip is set
                memcpy((char*) &mServerAddress.sin_addr.s_addr, (char*) mServer->h_addr_list[0], mServer->h_length);
                //port is set
                mServerAddress.sin_port = htons(port);

                int32_t res = ::connect(mSocket, (const sockaddr*) &mServerAddress, sizeof(mServerAddress));
                if (res == -1)
                {
                    return CAPU_SOCKET_ECONNECT;
                }
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setBufferSizeInternal()
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            if (mBufferSize < 0)
            {
                return CAPU_EINVAL;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, &mBufferSize, sizeof(mBufferSize)) < 0)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setLingerOptionInternal()
        {
            if (mLinger < 0)
            {
                return CAPU_EINVAL;
            }
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct linger so_linger;
            if (mIsLinger)
            {
                so_linger.l_onoff = 1;
                so_linger.l_linger = mLinger;
            }
            else
            {
                so_linger.l_onoff = 0;
                so_linger.l_linger = 0;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) < 0)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::setNoDelayInternal()
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            int32_t opt;
            if (mNoDelay)
            {
                opt = 1;
            }
            else
            {
                opt = 0;
            }
            if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::setKeepAliveInternal()
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }
            int32_t opt;
            if (mKeepAlive)
            {
                opt = 1;
            }
            else
            {
                opt = 0;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0)
            {
                return CAPU_ERROR;
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
            _timeout.tv_usec = ( mTimeout % 1000 ) * 1000;

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

        inline status_t TcpSocket::getBufferSize(int32_t& bufferSize)
        {
            if (mSocket == -1)
            {
                bufferSize = mBufferSize;
                return CAPU_OK;
            }

            int32_t size;
            socklen_t len = sizeof(size);
            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, &size, &len) < 0)
            {
                return CAPU_ERROR;
            }
            bufferSize = size;
            return CAPU_OK;
        }

        inline status_t TcpSocket::getLingerOption(bool_t& isLinger, int32_t& _linger)
        {
            if (mSocket == -1)
            {
                isLinger = mIsLinger;
                _linger  = mLinger;
                return CAPU_OK;
            }
            struct linger so_linger;

            socklen_t len = sizeof(so_linger);
            if (getsockopt(mSocket, SOL_SOCKET, SO_LINGER, &so_linger, &len) < 0)
            {
                return CAPU_ERROR;
            }
            _linger = so_linger.l_linger;
            if (so_linger.l_onoff > 0)
            {
                isLinger = true;
            }
            else
            {
                isLinger = false;
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::getNoDelay(bool_t& noDelay)
        {
            if (mSocket == -1)
            {
                noDelay = mNoDelay;
                return CAPU_OK;
            }
            int32_t opt;
            socklen_t len = sizeof(opt);
            if (getsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &opt, &len) < 0)
            {
                return CAPU_ERROR;
            }

            if (opt > 0)
            {
                noDelay = true;
            }
            else
            {
                noDelay = false;
            }

            return CAPU_OK;
        }

        inline status_t TcpSocket::getKeepAlive(bool_t& keepAlive)
        {
            if (mSocket == -1)
            {
                keepAlive = mKeepAlive;
                return CAPU_OK;
            }

            int32_t opt;

            socklen_t len = sizeof(opt);
            if (getsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, &len) < 0)
            {
                return CAPU_ERROR;
            }

            if (opt > 0)
            {
                keepAlive = true;
            }
            else
            {
                keepAlive = false;
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

            timeout = _timeout.tv_sec*1000;
            timeout += _timeout.tv_usec/1000;

            return CAPU_OK;
        }

        inline status_t TcpSocket::getRemoteAddress(char_t** remoteAddress)
        {
            if (mSocket == -1)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            sockaddr_in client_address = {0};
            socklen_t size = sizeof(client_address);
            if (getpeername(mSocket, (sockaddr*)&client_address, &size) < 0)
            {
                return CAPU_ERROR;
            }

            if ( 0 != remoteAddress)
            {
                char *remoteIP = inet_ntoa(client_address.sin_addr);
                uint_t stringLength = StringUtils::Strlen(remoteIP) + 1;
                *remoteAddress = new char_t[stringLength];
                StringUtils::Strncpy(*remoteAddress, stringLength, remoteIP);
            }
            return CAPU_OK;
        }
    }
}

#endif // CAPU_UNIXBASED_TCP_SOCKET_H

