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
#include "capu/os/UdpSocket.h"
#include "capu/os/Thread.h"
#include "capu/os/Mutex.h"
#include "capu/os/CondVar.h"
#include "capu/os/Math.h"

capu::Mutex mutex2;
capu::CondVar cv2;
bool cond2;

class RandomPort
{
public:
    /**
     * Gets a Random Port between 1024 and 10024
     */
    static uint16_t get()
    {
        return (rand() % 10000) + 30000; // 0-1023 = Well Known, 1024-49151 = User, 49152 - 65535 = Dynamic
    }
};

class ThreadClientUdpTest : public capu::Runnable
{
    uint16_t port;
public:
    //client thread to test data exchange between client and server
    ThreadClientUdpTest(uint16_t port) : port(port) {}
    void run()
    {
        int32_t communication_variable;
        int32_t numBytes = 0;
        capu::UdpSocket* clientSocket = new capu::UdpSocket();

        int32_t i = 5;
        //try to connect to ipv6
        //EXPECT_EQ(capu::CAPU_SOCKET_EADDR, clientSocket->send((char*) &i, sizeof(int32_t), "::1", port) );

        //wait for other side to start up
        mutex2.lock();
        while (!cond2)
        {
            cv2.wait(mutex2);
        }
        cond2 = false;
        mutex2.unlock();
        //send data
        EXPECT_EQ(capu::CAPU_OK, clientSocket->send((char*) &i, sizeof(int32_t), "127.0.0.1", port));

        //receive
        capu::status_t result = capu::CAPU_ERROR;

        result = clientSocket->receive((char*) &communication_variable, sizeof(int32_t), numBytes, 0);
        EXPECT_EQ(capu::CAPU_OK, result);

        //check value
        EXPECT_EQ(6, communication_variable);

        mutex2.lock();
        cond2 = true;
        cv2.signal();
        mutex2.unlock();

        //socket close
        EXPECT_EQ(capu::CAPU_OK, clientSocket->close());

        delete clientSocket;
    }
};

class ThreadTimeoutClientUdpTest : public capu::Runnable
{
    uint16_t port;
public:
    //timeout test
    ThreadTimeoutClientUdpTest(uint16_t port) : port(port) {}
    void run()
    {
        int32_t communication_variable;
        int32_t numBytes = 0;
        capu::Thread::Sleep(1000);
        //ALLOCATION AND SYNCH OF cient and  server
        capu::UdpSocket* cli_socket = new capu::UdpSocket();
        //timeout is 2 second;
        cli_socket->setTimeout(2);
        int32_t i = 5;

        //wait for other side to start up
        mutex2.lock();
        while (!cond2)
        {
            cv2.wait(mutex2);
        }
        cond2 = false;
        mutex2.unlock();

        //send data
        EXPECT_EQ(capu::CAPU_OK, cli_socket->send((char*) &i, sizeof(int32_t), "127.0.0.1", port));

        //receive
        EXPECT_EQ(capu::CAPU_ETIMEOUT, cli_socket->receive((char*) &communication_variable, sizeof(int32_t), numBytes, 0));

        mutex2.lock();
        cond2 = true;
        cv2.signal();
        mutex2.unlock();

        //socket close
        EXPECT_EQ(capu::CAPU_OK, cli_socket->close());
        //deallocating
        delete cli_socket;
    }
};

class ThreadServerUdpTest : public capu::Runnable
{
    uint16_t port;
public:
    //SERVER thread to test data exchange between client and server
    ThreadServerUdpTest(uint16_t port) : port(port) {}
    void run()
    {
        int32_t communication_variable;
        int32_t numBytes = 0;
        //server socket allocation
        capu::UdpSocket* serverSocket = new capu::UdpSocket();

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, serverSocket->bind(port, "127.0.0.1"));

        //receive data
        capu::SocketAddrInfo remoteSocket;
        //receive
        capu::status_t result = capu::CAPU_ERROR;

        //send signal that server is ready to receive
        mutex2.lock();
        cond2 = true;
        cv2.signal();
        mutex2.unlock();
        result = serverSocket->receive((char*) &communication_variable, sizeof(int32_t), numBytes, &remoteSocket);
        EXPECT_EQ(capu::CAPU_OK, result);

        EXPECT_STREQ("127.0.0.1", remoteSocket.addr.c_str());

        //check value
        EXPECT_EQ(5, communication_variable);

        //update data
        communication_variable++;

        //send it back
        EXPECT_EQ(capu::CAPU_OK, serverSocket->send((char*) &communication_variable, sizeof(int32_t), remoteSocket));

        mutex2.lock();
        while (!cond2)
        {
            cv2.wait(mutex2);
        }
        cond2 = false;
        mutex2.unlock();

        //close session
        EXPECT_EQ(capu::CAPU_OK, serverSocket->close());
        delete serverSocket;
    }
};

class ThreadTimeoutServerUdpTest : public capu::Runnable
{
    uint16_t port;
public:
    //timeout test
    ThreadTimeoutServerUdpTest(uint16_t port) : port(port) {}
    void run()
    {
        int32_t communication_variable;
        int32_t numBytes = 0;
        //server socket allocation
        capu::UdpSocket* serverSocket = new capu::UdpSocket();

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, serverSocket->bind(port));

        //receive data
        capu::status_t result = capu::CAPU_ERROR;

        //send signal that server is ready to receive
        mutex2.lock();
        cond2 = true;
        cv2.signal();
        mutex2.unlock();
        result = serverSocket->receive((char*) &communication_variable, sizeof(int32_t), numBytes, NULL);
        EXPECT_EQ(capu::CAPU_OK, result);

        //check value
        EXPECT_EQ(5, communication_variable);

        mutex2.lock();
        while (!cond2)
        {
            cv2.wait(mutex2);
        }
        cond2 = false;
        mutex2.unlock();

        //close session
        EXPECT_EQ(capu::CAPU_OK, serverSocket->close());

        delete serverSocket;
    }
};

TEST(UdpSocket, CloseReceiveAndSendTest)
{
    uint16_t port = RandomPort::get();
    capu::UdpSocket* socket = new capu::UdpSocket();
    int32_t i = 0;
    int32_t numBytes = 0;
    EXPECT_EQ(capu::CAPU_OK, socket->close());
    //try to send data via closed socket
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->send((char*) "asda", 4, "127.0.0.1", port));
    //try to receive data from closed socket
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->receive((char*) &i, 4, numBytes, NULL));
    //Deallocation of socket
    delete socket;
}

TEST(UdpSocket, SetAndGetPropertiesTest)
{
    capu::UdpSocket* socket = new capu::UdpSocket();
    //TRY TO CHANGE THE PROPERTIES OF NOT CONNECTED SOCKET
    EXPECT_EQ(capu::CAPU_OK, socket->setBufferSize(1024));
    EXPECT_EQ(capu::CAPU_OK, socket->setTimeout(90));

    int32_t int_tmp;

    //CHECK THE PROPERTIES ARE CORRECTLY SET
    EXPECT_EQ(capu::CAPU_OK, socket->getBufferSize(int_tmp));
    //On Linux the kernel adjust the buffer size and set it to doubles of given size (at least)
    //therefore we have to check here for >=
    EXPECT_TRUE(int_tmp >= 1024);

    EXPECT_EQ(capu::CAPU_OK, socket->getTimeout(int_tmp));

    // some systems seem to round the timeout to the next multiple of 4
    EXPECT_GE(3, capu::Math::Abs(static_cast<capu::int_t>(int_tmp - 90)));

    socket->close();
    //TRY TO CHANGE THE PROPERTIES OF CLOSED SOCKET
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setBufferSize(1024));
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setTimeout(90));
    //TRY TO GET PROPERTIES OF CLOSED SOCKET
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getBufferSize(int_tmp));
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getTimeout(int_tmp));

    delete socket;
}

TEST(UdpSocketAndUdpServerSocket, CommunicationTest)
{
    mutex2.lock();
    cond2 = false;
    mutex2.unlock();
    uint16_t port = RandomPort::get();
    ThreadServerUdpTest server(port);
    ThreadClientUdpTest client(port);
    capu::Thread* server_thread = new capu::Thread();
    server_thread->start(server);
    capu::Thread* client_thread = new capu::Thread();
    client_thread->start(client);
    //Create two threads which will behave like client and server to test functionality
    server_thread->join();
    client_thread->join();

    delete client_thread;
    delete server_thread;
}

TEST(UdpSocketAndUdpServerSocket, TimeoutTest)
{
    mutex2.lock();
    cond2 = false;
    mutex2.unlock();
    uint16_t port = RandomPort::get();
    ThreadTimeoutServerUdpTest server(port);
    ThreadTimeoutClientUdpTest client(port);
    capu::Thread* server_thread = new capu::Thread();
    server_thread->start(server);
    capu::Thread* client_thread = new capu::Thread();
    client_thread->start(client);
    //Create two threads which will behave like client and server to test functionality
    server_thread->join();
    client_thread->join();

    delete client_thread;
    delete server_thread;
}

TEST(UdpSocketAndUdpServerSocket, RandomPortTest)
{
    capu::UdpSocket socket;
    socket.bind(0, 0);

    const capu::SocketAddrInfo& sockInfo = socket.getSocketAddrInfo();

    EXPECT_STREQ("0.0.0.0", sockInfo.addr.c_str());
    EXPECT_TRUE(1024 < sockInfo.port);  // port should be bigger than standard ports
}

TEST(UdpSocketAndUdpServerSocket, BroadcastTest)
{
    capu::UdpSocket socket;
    socket.bind(0, 0);
    uint16_t port = socket.getSocketAddrInfo().port;  // send broadcast to self

    EXPECT_EQ(capu::CAPU_ERROR, socket.send("test", 4, "255.255.255.255", port)); // check for standard behaviour

    EXPECT_EQ(capu::CAPU_OK, socket.allowBroadcast(true));
    EXPECT_EQ(capu::CAPU_OK, socket.send("test", 4, "255.255.255.255", port)); // check for behaviour after enable

    EXPECT_EQ(capu::CAPU_OK, socket.allowBroadcast(false));
    EXPECT_EQ(capu::CAPU_ERROR, socket.send("test", 4, "255.255.255.255", port)); // check for behaviour after disable
}

