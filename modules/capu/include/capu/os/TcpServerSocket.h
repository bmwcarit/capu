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

#ifndef CAPU_TCPSERVERSOCKET_H
#define CAPU_TCPSERVERSOCKET_H

#include <capu/os/PlatformInclude.h>
#include "capu/os/TcpSocket.h"
#include "capu/Error.h"

#include CAPU_PLATFORM_INCLUDE(TcpServerSocket)

namespace capu
{
    /**
     * Server side socket. Creates socket connections by waiting for connections.
     */
    class TcpServerSocket: private capu::os::arch::TcpServerSocket
    {
    public:

        /**
         * The program flow will be blocked until a connection arrives
         * Programmer is responsible for deallocating memory of returning socket.
         *
         * @param timeoutMillis accept timeout. Default value is 0, which means to block forever.
         * @return TcpSocket if a connection is accepted
         *         NULL otherwise
         */
        TcpSocket* accept(capu::uint32_t timeoutMillis = 0);

        /**
         * Close the socket which is used for accepting connection
         *
         * @return CAPU_OK if the socket is successfully closed
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         */
        status_t close();

        /**
         * Binds the server to the given address and port.
         *
         * @param port indicates port number. 0 means the OS chooses a random free port.
         *             The chosen port can be determined by calling port().
         * @param address address to bind if it is not given it accepts all connection from any address
         * @return CAPU_OK  if the server socket is successfully bound
         *         CAPU_SOCKET_EADDR if the addr is faulty
         *         CAPU_ERROR  if the socket is already bound
         *         CAPU_EINVAL if the addr is NULL or port is equal to 0
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         */
        status_t bind(uint16_t port, const char_t* address = NULL);

        /**
         *
         * @param backlog (maximum length of the queue of pending connections)
         * @return CAPU_OK if the listen is successful
         *         CAPU_SOCKET_ESOCKET if the socket is not created
         *         CAPU_ERROR otherwise
         */
        status_t listen(uint8_t backlog);

        /**
         * Returns the port to which the server socket is bound.
         *
         * @return The port to which the server socket is bound or 0 if the server socket
         *         is not bound yet.
         */
        capu::uint16_t port();
    };

    inline
    TcpSocket*
    TcpServerSocket::accept(capu::uint32_t timeoutMillis)
    {
        return capu::os::arch::TcpServerSocket::accept(timeoutMillis);
    }

    inline
    status_t
    TcpServerSocket::close()
    {
        return capu::os::arch::TcpServerSocket::close();
    }

    inline
    status_t
    TcpServerSocket::bind(uint16_t port, const char_t* addr)
    {
        return capu::os::arch::TcpServerSocket::bind(port, addr);
    }

    inline
    status_t
    TcpServerSocket::listen(uint8_t backlog)
    {
        return capu::os::arch::TcpServerSocket::listen(backlog);
    }

    inline
    uint16_t
    TcpServerSocket::port()
    {
        return capu::os::arch::TcpServerSocket::port();
    }

}
#endif //CAPU_TCPSERVERSOCKET_H

