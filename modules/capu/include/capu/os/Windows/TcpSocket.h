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

#ifndef CAPU_WINDOWS_TCP_SOCKET_H
#define CAPU_WINDOWS_TCP_SOCKET_H

#include "capu/os/Socket.h"

namespace capu
{
    namespace os
    {
        typedef SOCKET SocketDescription;

        class TcpSocket
        {
        public:
            friend class TcpServerSocket;

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
            status_t getRemoteAddress(char_t** remoteAddress);

        protected:
        private:
            int32_t mBufferSize;
            bool_t  mIsLinger;
            int32_t mLinger;
            bool_t  mNoDelay;
            bool_t  mKeepAlive;
            int32_t mTimeout;

            SOCKET mSocket;
            WSADATA mWsaData;

            status_t setBufferSizeInternal();
            status_t setLingerOptionInternal();
            status_t setNoDelayInternal();
            status_t setKeepAliveInternal();
            status_t setTimeoutInternal();
        };

        inline
        TcpSocket::TcpSocket()
            : mBufferSize(-1)
            , mIsLinger(false)
            , mLinger(0)
            , mNoDelay(false)
            , mKeepAlive(false)
            , mTimeout(-1)
            , mSocket(INVALID_SOCKET)
        {
        }

        inline
        TcpSocket::TcpSocket(const SocketDescription& socketDescription)
            : mBufferSize(-1)
            , mIsLinger(false)
            , mLinger(0)
            , mNoDelay(false)
            , mKeepAlive(false)
            , mTimeout(-1)
        {
            //Initialize Winsock
            int32_t result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
            if (result == 0)
            {
                //create the socket which is used to connect the server
                mSocket = socketDescription;
                if (mSocket == INVALID_SOCKET)
                {
                    WSACleanup();
                }
            }
            else
            {
                mSocket = INVALID_SOCKET;
            }
        }

        inline
        status_t TcpSocket::connect(const char_t* dest_addr, uint16_t port)
        {

            int32_t result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
            if (0 == result)
            {
                //create the socket which is used to connect the server
                mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (mSocket == INVALID_SOCKET)
                {
                    WSACleanup();
                }
                else
                {
                    setBufferSizeInternal();
                    setLingerOptionInternal();
                    setNoDelayInternal();
                    setKeepAliveInternal();
                    setTimeoutInternal();
                }
            }
            else
            {
                mSocket = INVALID_SOCKET;
            }

            //check parameters
            if ((dest_addr == NULL) || (port == 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct hostent* serverHost = gethostbyname((const char_t*) dest_addr);
            if (serverHost == NULL)
            {
                return CAPU_SOCKET_EADDR;
            }

            struct sockaddr_in serverAddress;
            memset((char_t*) &serverAddress, 0x00, sizeof(serverAddress));

            serverAddress.sin_family = AF_INET;
            memcpy((char_t*) &serverAddress.sin_addr.s_addr, (char_t*) serverHost->h_addr_list[0], serverHost->h_length);
            serverAddress.sin_port = htons(port);

            if (::connect(mSocket, (sockaddr*) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
            {
                int32_t errorCode = WSAGetLastError();
                close();
                return CAPU_SOCKET_ECONNECT;
            }
            return CAPU_OK;
        }

        inline TcpSocket::~TcpSocket()
        {
            close();
        }

        inline
        status_t
        TcpSocket::setBufferSize(int32_t bufferSize)
        {
            mBufferSize = bufferSize;
            if (INVALID_SOCKET != mSocket)
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
            if (INVALID_SOCKET != mSocket)
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
            if (INVALID_SOCKET != mSocket)
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
            if (INVALID_SOCKET != mSocket)
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
            if (INVALID_SOCKET != mSocket)
            {
                return setTimeoutInternal();
            }
            return CAPU_OK;
        }

        inline status_t TcpSocket::send(const char_t* buffer, int32_t length, int32_t& sentBytes)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            const int32_t result = ::send(mSocket, buffer, length, 0);
            if (result == SOCKET_ERROR)
            {
                close();
                return CAPU_ERROR;
            }

            sentBytes = result;
            return CAPU_OK;
        }

        inline status_t TcpSocket::receive(char_t* buffer, int32_t length, int32_t& numBytes)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            int32_t result = recv(mSocket, buffer, length, 0);
            if (result == SOCKET_ERROR)
            {
                numBytes = 0;
                result = WSAGetLastError();
                if (result == WSAETIMEDOUT)
                {
                    return CAPU_ETIMEOUT;
                }
                else
                {
                    return CAPU_ERROR;
                }
            }
            numBytes = result;
            return CAPU_OK;
        }

        inline status_t TcpSocket::close()
        {
            int32_t returnValue = CAPU_OK;
            if (mSocket == INVALID_SOCKET)
            {
                returnValue = CAPU_SOCKET_ESOCKET;
            }
            else
            {
                shutdown(mSocket, SD_BOTH);
                int32_t result = closesocket(mSocket);
                if (result != 0)
                {
                    result = WSAGetLastError();
                    if (result != WSANOTINITIALISED)  //socket has already been closed
                    {
                        returnValue = CAPU_SOCKET_ECLOSE;
                    }
                }
                WSACleanup();
            }
            mSocket = INVALID_SOCKET;
            return returnValue;
        }



        inline
        status_t
        TcpSocket::setBufferSizeInternal()
        {
            if (mBufferSize < 0)
            {
                return CAPU_EINVAL;
            }
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&mBufferSize, sizeof(mBufferSize)) == SOCKET_ERROR)
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
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            struct linger soLinger;

            soLinger.l_onoff  = mIsLinger ? 1 : 0;
            soLinger.l_linger = mIsLinger ? mLinger : 0;

            if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char_t*)&soLinger, sizeof(soLinger)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setNoDelayInternal()
        {
            if (mSocket == INVALID_SOCKET)
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
            if (setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (char_t*)&opt, sizeof(opt)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setKeepAliveInternal()
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            int32_t opt = mKeepAlive ? 1 : 0;

            if (setsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, (char_t*)&opt, sizeof(opt)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        TcpSocket::setTimeoutInternal()
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char_t*)&mTimeout, sizeof(mTimeout)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char_t*)&mTimeout, sizeof(mTimeout)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline status_t TcpSocket::getBufferSize(int32_t& bufferSize)
        {
            if (mSocket == INVALID_SOCKET)
            {
                bufferSize = mBufferSize;
                return CAPU_OK;
            }

            socklen_t len = sizeof(bufferSize);
            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&bufferSize, &len) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline status_t TcpSocket::getLingerOption(bool_t& isLinger, int32_t& _linger)
        {
            if (mSocket == INVALID_SOCKET)
            {
                isLinger = mIsLinger;
                _linger = mLinger;
                return CAPU_OK;
            }

            struct linger soLinger;
            int32_t len = sizeof(soLinger);

            if (getsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char_t*)&soLinger, &len) == SOCKET_ERROR)
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
            if (mSocket == INVALID_SOCKET)
            {
                noDelay = mNoDelay;
                return CAPU_OK;
            }

            int32_t opt = 0;
            socklen_t len = sizeof(opt);

            if (getsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (char_t*)&opt, &len) == SOCKET_ERROR)
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
            if (mSocket == INVALID_SOCKET)
            {
                keepAlive  = mKeepAlive;
                return CAPU_OK;
            }

            int32_t opt = 0;
            int32_t len = sizeof(opt);

            if (getsockopt(mSocket, SOL_SOCKET, SO_KEEPALIVE, (char_t*)&opt, &len) == SOCKET_ERROR)
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

        inline status_t TcpSocket::getTimeout(int32_t& timeout)
        {
            if (mSocket == INVALID_SOCKET)
            {
                timeout = mTimeout;
                return CAPU_OK;
            }

            struct timeval soTimeout;
            socklen_t len = sizeof(soTimeout);

            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char_t*)&soTimeout, &len) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            timeout = soTimeout.tv_sec;

            return CAPU_OK;
        }

        inline status_t TcpSocket::getRemoteAddress(char_t** remoteAddress)
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            sockaddr_in client_address = {0};
            int32_t size = sizeof(client_address);
            auto ret = getpeername(mSocket, (sockaddr*)&client_address, &size); 

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

#endif //CAPU_WINDOWS_TCP_SOCKET_H
