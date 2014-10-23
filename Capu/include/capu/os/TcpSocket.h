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

#ifndef CAPU_TCP_SOCKET_H
#define CAPU_TCP_SOCKET_H

#include "capu/Error.h"
#include <capu/os/PlatformInclude.h>
#include "capu/os/Mutex.h"
#include CAPU_PLATFORM_INCLUDE(TcpSocket)

namespace capu
{
    /**
     * Tcp socket for IPv4
     */
    class TcpSocket : private capu::os::arch::TcpSocket
    {
    public:

        /**
         * Default Constructor
         */
        inline TcpSocket();

        /**
         * Constructor with initial socket description
         * @param socketDescription Object describing the socket to create
         */
        TcpSocket(const capu::os::SocketDescription& socketDescription);

        /**
         * Destructor
         */
        inline ~TcpSocket();

        /**
         * Send the messages
         * @param buffer    the content of message that will be sent to destination
         * @param length    the length of message that will be sent to destination
         * @param sentBytes reference which will contain the number of bytes sent
         * @return CAPU_OK if the sent is successful
         *         CAPU_EINVAL if the buffer is NULL
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t send(const char_t* buffer, int32_t length, int32_t& sentBytes);

        /**
         * Receive message
         * @param buffer    buffer that will be used to store incoming message
         * @param length    buffer size
         * @param numBytes  number of bytes on socket
         * @return CAPU_OK if the receive is successfully executed
         *         CAPU_TIMEOUT if there has been a timeout
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t receive(char_t* buffer, int32_t length, int32_t& numBytes);

        /**
         * close the socket
         * @return CAPU_OK if the socket is correctly closed
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t close();

        /**
         * connect to the given address
         * @param dest_addr destination address of server
         * @param port      port number of service
         * @return  CAPU_OK if correctly connects to specified address
         *          CAPU_SOCKET_ECONNECT if the connection is not successful
         *          CAPU_SOCKET_EADDR if the given address is not resolved.
         *          CAPU_EINVAL if the dest_addr is NULL
         *          CAPU_SOCKET_ESOCKET if the socket is not created
         */
        inline status_t connect(const char_t* dest_addr, uint16_t port);

        /**
         * Sets the maximum socket buffer in bytes. The kernel doubles this value (to allow space for bookkeeping overhead)
         * Set the receive buffer size
         * Sets buffer size information.
         * @return CAPU_OK if the buffer is successfully set for both receive and send operations
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t setBufferSize(int32_t bufferSize);

        /**
         * Set the linger option for socket
         * Specifies whether the socket lingers on close() if data is present.
         * @return CAPU_OK if the LingerOption is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t setLingerOption(bool_t isLinger, uint16_t linger);


        /**
         * Set no delay option
         * Specifies whether the Nagle algorithm used by TCP for send coalescing is to be disabled.
         * @return CAPU_OK if the NoDelay is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t setNoDelay(bool_t noDelay);

        /**
         * Set Keep Alive option
         * Keeps connections active by enabling periodic transmission of messages, if this is supported by the protocol.
         * @return CAPU_OK if the KeepAlive is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t setKeepAlive(bool_t keepAlive);

        /**
         * Set Timeout
         * Sets the timeout value that specifies the maximum amount of time an input function waits until it completes
         * @return CAPU_OK if the Timeout for receive operation is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t setTimeout(int32_t timeout);

        /**
         * get the send and receive buffer size
         * gets buffer size information.
         * @return CAPU_OK if the buffer is successfully set for both receive and send operations
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t getBufferSize(int32_t& bufferSize);

        /**
         * get the linger option for socket
         * @return CAPU_OK if the LingerOption is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t getLingerOption(bool_t& isLinger, uint16_t& linger);

        /**
         * Get no delay option
         * @return CAPU_OK if the NoDelay is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t getNoDelay(bool_t& noDelay);

        /**
         * Get Keep Alive option
         * @return CAPU_OK if the KeepAlive is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t getKeepAlive(bool_t& keepAlive);


        /**
         * Get Timeout
         * @return CAPU_OK if the Timeout for receive operation is successfully set
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        inline status_t getTimeout(int32_t& timeout);

        /**
         * Get remote ip address this socket is connected to
         * @return CAPU_OK if the address was successfully obtained
         * @return CAPU_SOCKET_ESOCKET if the socket is not valid
         * @return CAPU_ERROR otherwise
         **/
        inline status_t getRemoteAddress(char_t** address);

        /**
         * Returns the internal socket descriptor. This can be different on 
         * different os
         * 
         * @return the internal descriptor of the socket
         */
        inline const capu::os::SocketDescription& getSocketDescription() const;
        
    };

    inline
    TcpSocket::TcpSocket()
    {
    }

    inline
    TcpSocket::~TcpSocket()
    {
    }


    inline
    TcpSocket::TcpSocket(const capu::os::SocketDescription& socketDescription)
        : capu::os::arch::TcpSocket(socketDescription)
    {

    }

    inline
    status_t
    TcpSocket::send(const char_t* buffer, int32_t length, int32_t& sentBytes)
    {
        status_t result = capu::os::arch::TcpSocket::send(buffer, length, sentBytes);
        return result;

    }

    inline
    status_t
    TcpSocket::receive(char_t* buffer, int32_t length, int32_t& numBytes)
    {
        status_t result = capu::os::arch::TcpSocket::receive(buffer, length, numBytes);
        return result;
    }

    inline
    status_t
    TcpSocket::close()
    {
        return capu::os::arch::TcpSocket::close();
    }

    inline
    status_t
    TcpSocket::connect(const char_t* dest_addr, uint16_t port)
    {
        return capu::os::arch::TcpSocket::connect(dest_addr, port);
    }

    inline
    status_t
    TcpSocket::setBufferSize(int32_t bufferSize)
    {
        return capu::os::arch::TcpSocket::setBufferSize(bufferSize);
    }

    inline
    status_t
    TcpSocket::setLingerOption(bool_t isLinger, uint16_t linger)
    {
        return capu::os::arch::TcpSocket::setLingerOption(isLinger, linger);
    }

    inline
    status_t
    TcpSocket::setNoDelay(bool_t noDelay)
    {
        return capu::os::arch::TcpSocket::setNoDelay(noDelay);
    }

    inline
    status_t
    TcpSocket::setKeepAlive(bool_t keepAlive)
    {
        return capu::os::arch::TcpSocket::setKeepAlive(keepAlive);
    }

    inline
    status_t
    TcpSocket::setTimeout(int32_t timeout)
    {
        return capu::os::arch::TcpSocket::setTimeout(timeout);
    }

    inline
    status_t
    TcpSocket::getBufferSize(int32_t& bufferSize)
    {
        return capu::os::arch::TcpSocket::getBufferSize(bufferSize);
    }

    inline
    status_t
    TcpSocket::getLingerOption(bool_t& isLinger, uint16_t& linger)
    {
        return capu::os::arch::TcpSocket::getLingerOption(isLinger, linger);
    }

    inline
    status_t
    TcpSocket::getNoDelay(bool_t& noDelay)
    {
        return capu::os::arch::TcpSocket::getNoDelay(noDelay);
    }

    inline
    status_t
    TcpSocket::getKeepAlive(bool_t& keepAlive)
    {
        return capu::os::arch::TcpSocket::getKeepAlive(keepAlive);
    }

    inline
    status_t
    TcpSocket::getTimeout(int32_t& timeout)
    {
        return capu::os::arch::TcpSocket::getTimeout(timeout);
    }

    inline
    status_t
    TcpSocket::getRemoteAddress(char_t** address)
    {
        return capu::os::arch::TcpSocket::getRemoteAddress(address);
    }
    inline 
    const capu::os::SocketDescription& 
    TcpSocket::getSocketDescription() const
    {
        return capu::os::arch::TcpSocket::getSocketDescription();
    }
}

#endif /* CAPU_TCP_SOCKET_H */

