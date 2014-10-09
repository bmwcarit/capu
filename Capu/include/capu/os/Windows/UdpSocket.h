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
            status_t allowBroadcast(const bool_t broadcast);
            status_t getBufferSize(int32_t& bufferSize);
            status_t getTimeout(int32_t& timeout);
            const SocketAddrInfo& getSocketAddrInfo() const;
            const capu::os::SocketDescription& getSocketDescription() const;

        private:
            SocketDescription mSocket;
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
            , mSocket(INVALID_SOCKET)
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
            else
            {
                const int32_t result = inet_pton(AF_INET, addr, &(socketAddr.sin_addr.s_addr));
                if (result <= 0)
                {
                    return CAPU_SOCKET_EADDR;
                }
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
                char_t str[INET_ADDRSTRLEN] = { '\0' };
                inet_ntop(AF_INET, &(sin.sin_addr.s_addr), str, INET_ADDRSTRLEN);
                mAddrInfo.port = ntohs(sin.sin_port);
                mAddrInfo.addr = str;
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

            int32_t result = inet_pton(AF_INET, receiverAddr, &(receiverSockAddr.sin_addr.s_addr));
            if (result <= 0)
            {
                return CAPU_SOCKET_EADDR;
            }

            result = sendto(mSocket, buffer, length, 0, (sockaddr*) &receiverSockAddr, sizeof(receiverSockAddr));

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
            return send(buffer, length, receiverAddr.addr.c_str(), receiverAddr.port);
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
                    char_t buffer[INET_ADDRSTRLEN] = { '\0' };
                    const char_t* result = inet_ntop(AF_INET, &(remoteSocketAddr.sin_addr), buffer, INET_ADDRSTRLEN);
                    if (0 == result)
                    {
                        return CAPU_ERROR;
                    }

                    sender->port = ntohs(remoteSocketAddr.sin_port);
                    sender->addr = result;
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

            if (mIsBound)
            {
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
                WSACleanup();
                mIsBound = false;
                mIsInitilized = false;
            }
            mSocket = INVALID_SOCKET;
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
        UdpSocket::allowBroadcast(const bool_t broadcast)
        {
            if (mSocket == INVALID_SOCKET)
            {
                return CAPU_SOCKET_ESOCKET;
            }

            int32_t allow = 0;

            if (broadcast)
                allow = 1;

            if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char_t*)&allow, sizeof(int32_t)) == SOCKET_ERROR)
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

        inline
        const capu::os::SocketDescription& 
        UdpSocket::getSocketDescription() const
        {
            return mSocket;
        }
    }

}

#endif //CAPU_WINDOWS_UDP_SOCKET_H
