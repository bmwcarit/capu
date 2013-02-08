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

#ifndef CAPU_WINDOWS_UDP_SOCKET_H
#define CAPU_WINDOWS_UDP_SOCKET_H

#include "capu/os/Socket.h"
#include "capu/os/StringUtils.h"
#include "capu/os/Memory.h"

namespace capu
{
    namespace os
    {
        class UdpSocket
        {
        public:
            UdpSocket();
            ~UdpSocket();
            status_t bind(const uint16_t port, const char_t* addr = NULL);
            status_t send(const char_t* buffer, const int32_t length, const SocketAddrInfo& receiverAddr);
            status_t send(const char_t* buffer, const int32_t length, const char_t* receiverAddr, const uint16_t receiverPort);
            status_t receive(char_t* buffer, const int32_t length, int32_t& numBytes, SocketAddrInfo* sender);
            status_t close();
            status_t setBufferSize(const int32_t bufferSize);
            status_t setTimeout(const int32_t timeout);
            status_t getBufferSize(int32_t& bufferSize);
            status_t getTimeout(int32_t& timeout);
            const SocketAddrInfo& getSocketAddrInfo() const;

        private:
            SOCKET mSocket;
            WSADATA mWsaData;
            bool_t mIsBound;
            SocketAddrInfo mAddrInfo;
            bool_t mIsInitilized;

            void initialize();
        };

        inline
        UdpSocket::UdpSocket()
            : mIsBound(false)
            , mIsInitilized(false)
        {
            initialize();
        }

        inline
        UdpSocket::~UdpSocket()
        {
            close();
        }

        inline
        void UdpSocket::initialize()
        {
            if (!mIsInitilized)
            {
                int32_t result = WSAStartup(MAKEWORD(2, 2), &mWsaData);
                if (result == 0)
                {
                    //create the socket which is used to connect the server
                    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    mIsBound = false;
                    if (mSocket == INVALID_SOCKET)
                    {
                        WSACleanup();
                    }
                    else
                    {
                        int32_t optVal = 1;
                        setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char_t*)&optVal, sizeof(optVal));
                        mIsInitilized = true;
                    }
                }
                else
                {
                    mSocket = INVALID_SOCKET;
                }
            }
        }

        inline
        status_t
        UdpSocket::bind(const uint16_t port, const char_t* addr)
        {
            //check if the address is valid

            initialize();

            if (mIsBound)
            {
                return CAPU_ERROR;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            sockaddr_in socketAddr;
            memset((char_t*) &socketAddr, 0x00, sizeof(socketAddr));
            socketAddr.sin_family = AF_INET;
            if (addr == NULL)
            {
                socketAddr.sin_addr.s_addr = INADDR_ANY;
            }
            else if (inet_addr(addr) == INADDR_NONE)
            {
                return CAPU_SOCKET_EADDR;
            }
            else
            {
                socketAddr.sin_addr.s_addr = inet_addr(addr);
            }
            socketAddr.sin_port = htons(port);

            int32_t result = ::bind(mSocket, (sockaddr*) &socketAddr, sizeof(socketAddr));

            if (result == SOCKET_ERROR)
            {
                return CAPU_SOCKET_EBIND;
            }

            struct sockaddr_in sin;
            socklen_t len = sizeof(sin);
            if (getsockname(mSocket, (struct sockaddr*)&sin, &len) != -1)
            {
                // inet_ntop also supports IPv6, which is not supported under VS2005,
                // therefore we use inet_ntoa (IPv6 is not supported by capu anyway)
                //char_t str[INET_ADDRSTRLEN];
                //inet_ntop(AF_INET, &(sin.sin_addr.s_addr), str, INET_ADDRSTRLEN);
                mAddrInfo.addr = inet_ntoa(sin.sin_addr);
                mAddrInfo.port = ntohs(sin.sin_port);
            }

            mIsBound = true;
            return CAPU_OK;
        }

        inline
        const SocketAddrInfo& UdpSocket::getSocketAddrInfo() const
        {
            return mAddrInfo;
        }

        inline
        status_t
        UdpSocket::send(const char_t* buffer, const int32_t length, const char_t* receiverAddr, const uint16_t receiverPort)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            initialize();

            struct sockaddr_in receiverSockAddr;
            Memory::Set(&receiverSockAddr, 0 , sizeof(sockaddr_in));

            receiverSockAddr.sin_family = AF_INET;
            receiverSockAddr.sin_port = htons(receiverPort);
            receiverSockAddr.sin_addr.s_addr = inet_addr(receiverAddr);

            const int32_t result = sendto(mSocket, buffer, length, 0, (sockaddr*) &receiverSockAddr, sizeof(receiverSockAddr));

            if (result == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            return CAPU_OK;
        }

        inline
        status_t
        UdpSocket::send(const char_t* buffer, const int32_t length, const SocketAddrInfo& receiverAddr)
        {
            return send(buffer, length, receiverAddr.addr, receiverAddr.port);
        }

        inline
        status_t
        UdpSocket::receive(char_t* buffer, const int32_t length, int32_t& numBytes, SocketAddrInfo* sender)
        {
            if ((buffer == NULL) || (length < 0))
            {
                return CAPU_EINVAL;
            }

            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            sockaddr_in remoteSocketAddr;
            int32_t remoteSocketAddrSize = sizeof(remoteSocketAddr);

            int32_t result = recvfrom(mSocket, buffer, length, 0, (sockaddr*)&remoteSocketAddr, &remoteSocketAddrSize);
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
            else
            {
                if (sender != 0)
                {
                    sender->port = ntohs(remoteSocketAddr.sin_port);
                    char_t* addr = inet_ntoa(remoteSocketAddr.sin_addr);
                    sender->addr = addr;
                }
            }

            numBytes = result;
            return CAPU_OK;
        }

        inline
        status_t
        UdpSocket::close()
        {
            int32_t returnValue = CAPU_OK;
            if (mSocket == INVALID_SOCKET)
            {
                returnValue = CAPU_SOCKET_ESOCKET;
            }
            else
            {
                int32_t result = closesocket(mSocket);
                if (result != 0)
                {
                    result = WSAGetLastError();
                    if (result != WSANOTINITIALISED)  //socket has already been closed
                    {
                        returnValue = CAPU_SOCKET_ECLOSE;
                    }
                }
            }
            mSocket = INVALID_SOCKET;
            WSACleanup();
            mIsBound = false;
            mIsInitilized = false;
            return returnValue;
        }

        inline
        status_t
        UdpSocket::setBufferSize(const int32_t bufferSize)
        {
            if (bufferSize < 0)
            {
                return CAPU_EINVAL;
            }
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&bufferSize, sizeof(bufferSize)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline
        status_t
        UdpSocket::setTimeout(const int32_t timeout)
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            if (setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char_t*)&timeout, sizeof(int32_t)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }
            if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char_t*)&timeout, sizeof(int32_t)) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline
        status_t
        UdpSocket::getBufferSize(int32_t& bufferSize)
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            socklen_t len = sizeof(bufferSize);
            if (getsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char_t*)&bufferSize, &len) == SOCKET_ERROR)
            {
                return CAPU_ERROR;
            }

            return CAPU_OK;
        }

        inline
        status_t
        UdpSocket::getTimeout(int32_t& timeout)
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
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
    }
}

#endif //CAPU_WINDOWS_UDP_SOCKET_H
