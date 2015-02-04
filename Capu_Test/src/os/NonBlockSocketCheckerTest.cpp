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
#include "capu/os/Thread.h"
#include "capu/os/TcpSocket.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-generated-matchers.h"
#include "capu/container/HashTable.h"
#include "capu/os/NonBlockSocketChecker.h"


class RandomPort
{
public:
    /**
    * Gets a Random Port between 1024 and 10024
    */
    static capu::uint16_t get()
    {
        return (rand() % 10000) + 40000; // 0-1023 = Well Known, 1024-49151 = User, 49152 - 65535 = Dynamic
    }
};

class AsyncSocketHandler
{
public:

    AsyncSocketHandler(const capu::uint32_t numberOfClients, capu::uint16_t port)
        : m_socketInfos(numberOfClients*2)
        , m_receiveCount(0)
    {
        m_serverSocket.bind(port, "0.0.0.0");
        m_serverSocket.listen(10);

        m_socketInfos.push_back(capu::os::SocketInfoPair(m_serverSocket.getSocketDescription(), capu::os::SocketDelegate::Create<AsyncSocketHandler, &AsyncSocketHandler::acceptConnectionCallback>(*this)));
    }

    ~AsyncSocketHandler()
    {
        capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*>::Iterator current = m_clientSockets.begin();
        const capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*>::Iterator end = m_clientSockets.end();

        for (; current != end; ++current)
        {
            current->value->close();
            delete current->value;
        }
    }

    void acceptConnectionCallback(const capu::os::SocketDescription& socketDescription)
    {
        UNUSED(socketDescription);
        capu::TcpSocket* clientSocket = m_serverSocket.accept();
        EXPECT_TRUE(0 != clientSocket);
        m_clientSockets.put(clientSocket->getSocketDescription(), clientSocket);
        m_socketInfos.push_back(capu::os::SocketInfoPair(clientSocket->getSocketDescription(), capu::os::SocketDelegate::Create<AsyncSocketHandler, &AsyncSocketHandler::receiveDataCallback>(*this)));
    }

    void sendSomeData()
    {
        capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*>::Iterator current = m_clientSockets.begin();
        capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*>::Iterator end = m_clientSockets.end();

        for (; current != end; ++current)
        {
            capu::uint32_t sendValue = 42;
            capu::int32_t sendBytes;
            current->value->send(reinterpret_cast<capu::char_t*>(&sendValue), sizeof(sendValue), sendBytes);
        }
    }

    void receiveDataCallback(const capu::os::SocketDescription& socketDescription)
    {
        capu::int32_t data;
        capu::int32_t numbytes = 0;

        capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*>::Iterator entry = m_clientSockets.find(socketDescription);
        if (entry != m_clientSockets.end())
        {
            EXPECT_EQ(capu::CAPU_OK, entry->value->receive(reinterpret_cast<capu::char_t*>(&data), sizeof(data), numbytes));
            if (numbytes != 0)
            {
                EXPECT_EQ(42, data);
                ++m_receiveCount;
            }
        }
    }

    capu::Vector<capu::os::SocketInfoPair> m_socketInfos;
    capu::HashTable<capu::os::SocketDescription, capu::TcpSocket*> m_clientSockets;
    capu::TcpServerSocket m_serverSocket;
    capu::uint32_t m_receiveCount;
};


class AsyncClient : public capu::Runnable
{
public:
    AsyncClient()
        : m_port(0)
        , m_send(false)
        , m_receiveCount(0)
    {

    }
    void receiveSomeData(const capu::os::SocketDescription&)
    {
        capu::int32_t data;
        capu::int32_t numbytes = 0;

        EXPECT_EQ(capu::CAPU_OK, m_clientSocket.receive(reinterpret_cast<capu::char_t*>(&data), sizeof(data), numbytes));
        if (numbytes!=0)
        {
            EXPECT_EQ(42, data);
            ++m_receiveCount;
        }
    }

    void run()
    {
        m_clientSocket.setTimeout(1000);

        while (m_clientSocket.connect("localhost", m_port) != capu::CAPU_OK){
        }

        m_socketInfos.push_back(capu::os::SocketInfoPair(m_clientSocket.getSocketDescription(), capu::os::SocketDelegate::Create<AsyncClient, &AsyncClient::receiveSomeData>(*this)));

        if (m_send)
        {
            capu::uint32_t sendValue = 42;
            capu::int32_t sendBytes = -1;
            m_clientSocket.send(reinterpret_cast<capu::char_t*>(&sendValue), sizeof(sendValue), sendBytes);
            EXPECT_EQ(static_cast<capu::int32_t>(sizeof(sendValue)), sendBytes);
        }
    }
     


    void start(capu::uint16_t port, capu::bool_t send)
    {
        m_port = port;
        m_send = send;
        m_thread.start(*this);
    }

    void stop()
    {
        m_thread.cancel();
        m_thread.join();
        m_clientSocket.close();
    }

    capu::TcpSocket* getSocket()
    {
        return &m_clientSocket;
    }

    capu::uint16_t m_port;
    capu::bool_t m_send;
    capu::TcpSocket m_clientSocket;
    capu::Thread m_thread;
    capu::Vector<capu::os::SocketInfoPair> m_socketInfos;
    capu::uint32_t m_receiveCount;
};

TEST(NonBlockSocketCheckerTest, AcceptALotOfClients)
{
    static const capu::uint32_t clientcount = 50;

    capu::uint16_t port = RandomPort::get();

    AsyncSocketHandler asyncSocketHandler(clientcount, port);

    AsyncClient asyncClient[clientcount];

    for (capu::uint32_t i = 0; i < clientcount; ++i)
    {
        asyncClient[i].start(port, false);
    }

    while (asyncSocketHandler.m_clientSockets.count() < clientcount)
    {
        capu::NonBlockSocketChecker::CheckSocketsForIncomingData(asyncSocketHandler.m_socketInfos, 10);
    }

    for (capu::uint32_t i = 0; i < clientcount; ++i)
    {
        asyncClient[i].stop();
    }
}

TEST(NonBlockSocketCheckerTest, DISABLED_ReceiveDataFromALotOfClients)
{
    static const capu::uint32_t clientcount = 50;

    capu::uint16_t port = RandomPort::get();

    AsyncSocketHandler asyncSocketHandler(clientcount, port);


    AsyncClient asyncClient[clientcount];

    for (capu::uint32_t i = 0; i < clientcount; ++i)
    {
        asyncClient[i].start(port, true);
    }

    while (asyncSocketHandler.m_receiveCount < clientcount)
    {
        capu::NonBlockSocketChecker::CheckSocketsForIncomingData(asyncSocketHandler.m_socketInfos, 10);
    }

    for (capu::uint32_t i = 0; i < clientcount; ++i)
    {
        asyncClient[i].stop();
    }
}

TEST(NonBlockSocketCheckerTest, DISABLED_ReceiveDataOnClientSide)
{
    capu::uint16_t port = RandomPort::get();

    AsyncSocketHandler asyncSocketHandler(1, port);

    AsyncClient asyncClient;
    asyncClient.start(port, false);
    
    while (asyncSocketHandler.m_clientSockets.count() < 1)
    {
        capu::NonBlockSocketChecker::CheckSocketsForIncomingData(asyncSocketHandler.m_socketInfos, 0);
    }

    asyncSocketHandler.sendSomeData();

    while (asyncClient.m_receiveCount < 1)
    {
        capu::NonBlockSocketChecker::CheckSocketsForIncomingData(asyncClient.m_socketInfos, 1000);
    }

    asyncClient.stop();
}
