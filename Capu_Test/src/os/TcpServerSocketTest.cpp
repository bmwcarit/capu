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

#include <gtest/gtest.h>
#include "capu/os/TcpServerSocket.h"
#include "capu/Error.h"
#include "capu/os/Time.h"
#include "capu/os/Math.h"


TEST(TcpServerSocket, CloseTest)
{
    capu::TcpServerSocket* socket = new capu::TcpServerSocket();
    //try to close serversocket
    EXPECT_EQ(capu::CAPU_OK, socket->close());
    //try to close already closed socket
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->close());
    delete socket;
}

TEST(TcpServerSocket, BindTest)
{
    capu::TcpServerSocket* socket = new capu::TcpServerSocket();
    //try to bind on port 0
    EXPECT_EQ(capu::CAPU_OK, socket->bind(0, "0.0.0.0"));

    //call bind twice
    EXPECT_EQ(capu::CAPU_ERROR, socket->bind(0, "127.0.0.1"));

    delete socket;

    socket = new capu::TcpServerSocket();
    //expect true if address is wrong
    EXPECT_EQ(capu::CAPU_SOCKET_EADDR, socket->bind(0, "0.0.0.600"));
    EXPECT_EQ(capu::CAPU_OK, socket->close());
    //deallocate socket
    delete socket;
}

TEST(TcpServerSocket, ListenTest)
{
    capu::TcpServerSocket* socket = new capu::TcpServerSocket();

    //try to start listening on a unbound socket
    EXPECT_EQ(capu::CAPU_EINVAL, socket->listen(3));

    //faulty bind and listen
    EXPECT_EQ(capu::CAPU_SOCKET_EADDR, socket->bind(0, "0.0.0.2564"));
    EXPECT_EQ(capu::CAPU_EINVAL, socket->listen(3));
    //bind and listen in a correct way
    EXPECT_EQ(capu::CAPU_OK, socket->bind(0, "0.0.0.0"));
    EXPECT_EQ(capu::CAPU_OK, socket->listen(3));
    delete socket;
}

TEST(TcpServerSocket, AcceptTimeoutTest)
{
    capu::TcpServerSocket socket;
    EXPECT_EQ(capu::CAPU_OK, socket.bind(0, "0.0.0.0"));
    EXPECT_EQ(capu::CAPU_OK, socket.listen(3));
    uint32_t timeout = 1500;
    uint64_t start = capu::Time::GetMilliseconds();
    socket.accept(timeout);
    uint64_t dur = capu::Time::GetMilliseconds() - start;

    uint64_t diff = dur - timeout;
    EXPECT_GE(1000, capu::Math::Abs(static_cast<capu::int_t>(diff)));
}

TEST(TcpServerSocket, FreePortTest)
{
    capu::TcpServerSocket server;
    ASSERT_EQ(capu::CAPU_OK, server.bind(0, "0.0.0.0"));
    EXPECT_NE(0u, server.port());
}

