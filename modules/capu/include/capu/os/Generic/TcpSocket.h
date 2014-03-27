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

#ifndef CAPU_GENERIC_TCPSOCKET_H
#define CAPU_GENERIC_TCPSOCKET_H

#include "capu/Config.h"
#include "capu/Error.h"
#include "capu/os/Socket.h"

namespace capu
{

    namespace generic
    {
        class TcpSocket
        {
        public:
            status_t setBufferSize(int32_t bufferSize);
            status_t setLingerOption(bool_t isLinger, int32_t linger);
            status_t setNoDelay(bool_t noDelay);
            status_t setKeepAlive(bool_t keepAlive);
            status_t getBufferSize(int32_t& bufferSize);
            status_t getLingerOption(bool_t& isLinger, int32_t& linger);
            status_t getNoDelay(bool_t& noDelay);
            status_t getKeepAlive(bool_t& keepAlive);
            status_t getRemoteAddress(char_t** remoteAddress);

        protected:
            TcpSocket();

            status_t setSocketParameters();
            status_t getSocketAddr(const char_t* dest_addr, uint16_t port, struct sockaddr_in& socketAddressInfo);

        private:
            int32_t mBufferSize;
            bool_t  mIsLinger;
            int32_t mLinger;
            bool_t  mNoDelay;
            bool_t  mKeepAlive;

        protected:
            capu::os::SocketDescription mSocket;

        private:
            status_t setBufferSizeInternal();
            status_t setLingerOptionInternal();
            status_t setNoDelayInternal();
            status_t setKeepAliveInternal();
        };

        inline
        TcpSocket::TcpSocket()
            : mBufferSize(-1)
            , mIsLinger(false)
            , mLinger(0)
            , mNoDelay(false)
            , mKeepAlive(false)
            , mSocket(CAPU_INVALID_SOCKET)
        {

        }

        inline
        status_t TcpSocket::setSocketParameters()
        {
            status_t returnStatus = CAPU_OK;
            status_t status = setBufferSizeInternal();
            if (status != CAPU_OK) {
                returnStatus = status;
            }
            status = setNoDelayInternal();
            if (status != CAPU_OK) {
                returnStatus = status;
            }
            status = setKeepAliveInternal();
            if (status != CAPU_OK) {
                returnStatus = status;
            }

            return returnStatus;
        }

        inline
            status_t TcpSocket::getSocketAddr(const char_t* dest_addr, uint16_t port, struct sockaddr_in& socketAddressInfo)
        {
            struct hostent* serverHost = gethostbyname((const char_t*) dest_addr);
            if (serverHost == NULL)
            {
                return CAPU_SOCKET_EADDR;
            }

            struct sockaddr_in serverAddress;
            capu::Memory::Set((char_t*) &serverAddress, 0x00, sizeof(serverAddress));

            serverAddress.sin_family = AF_INET;
            capu::Memory::Copy((char_t*) &serverAddress.sin_addr.s_addr, (char_t*) serverHost->h_addr_list[0], serverHost->h_length);
            serverAddress.sin_port = htons(port);

            socketAddressInfo = serverAddress;
            return CAPU_OK;

        }

        inline status_t TcpSocket::getRemoteAddress(char_t** remoteAddress)
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            sockaddr_in client_address;
            socklen_t size = sizeof(client_address);
            capu::Memory::Set(&client_address, 0, size);

            int32_t ret = getpeername(mSocket, (sockaddr*)&client_address, &size);

            if (ret != 0)
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

        inline
        status_t
            TcpSocket::setBufferSize(int32_t bufferSize)
        {
            mBufferSize = bufferSize;
            if (CAPU_INVALID_SOCKET != mSocket)
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
            if (CAPU_INVALID_SOCKET != mSocket)
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
            if (CAPU_INVALID_SOCKET != mSocket)
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
            if (CAPU_INVALID_SOCKET != mSocket)
            {
                return setKeepAliveInternal();
            }
            return CAPU_OK;
        }

        inline
            status_t
            TcpSocket::setBufferSizeInternal()
        {
            if (mBufferSize < 0)
            {
                return CAPU_EINVAL;
            }
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&mBufferSize, sizeof(mBufferSize)) <= CAPU_SOCKET_ERROR)
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
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct linger soLinger;

            soLinger.l_onoff  = mIsLinger ? 1 : 0;
            soLinger.l_linger = mIsLinger ? mLinger : 0;

            if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char_t*)&soLinger, sizeof(soLinger)) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
            status_t
            TcpSocket::setNoDelayInternal()
        {
            if (mSocket == CAPU_INVALID_SOCKET)
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
            if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (char_t*)&opt, sizeof(opt)) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
            status_t
            TcpSocket::setKeepAliveInternal()
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            int32_t opt = mKeepAlive ? 1 : 0;

            if (setsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, (char_t*)&opt, sizeof(opt)) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::getBufferSize(int32_t& bufferSize)
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                bufferSize = mBufferSize;
                return CAPU_OK;
            }

            socklen_t len = sizeof(bufferSize);
            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&bufferSize, &len) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline status_t TcpSocket::getLingerOption(bool_t& isLinger, int32_t& _linger)
        {
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                isLinger = mIsLinger;
                _linger = mLinger;
                return CAPU_OK;
            }

            struct linger soLinger;
            socklen_t len = sizeof(soLinger);

            if (getsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char_t*)&soLinger, &len) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            _linger = soLinger.l_linger;

            if (soLinger.l_onoff == 1)
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
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                noDelay = mNoDelay;
                return CAPU_OK;
            }

            int32_t opt = 0;
            socklen_t len = sizeof(opt);

            if (getsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (char_t*)&opt, &len) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            if (opt == 1)
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
            if (mSocket == CAPU_INVALID_SOCKET)
            {
                keepAlive  = mKeepAlive;
                return CAPU_OK;
            }

            int32_t opt = 0;
            socklen_t len = sizeof(opt);

            if (getsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, (char_t*)&opt, &len) <= CAPU_SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            if (opt == 1)
            {
                keepAlive = true;
            }
            else
            {
                keepAlive = false;
            }

            return CAPU_OK;
        }

    }
}

#endif //CAPU_GENERIC_TCPSOCKET_H
