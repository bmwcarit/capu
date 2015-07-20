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
#include "capu/os/Mutex.h"
#include "capu/os/CondVar.h"
#include "capu/util/TcpSocketOutputStream.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-generated-matchers.h"

capu::Mutex mutex;
capu::CondVar cv;
bool cond = false;
capu::uint16_t chosenPort = 0;

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

class ThreadClientTest : public capu::Runnable
{
    capu::uint16_t port;
public:
    //client thread to test data exchange between client and server
    ThreadClientTest(capu::uint16_t port) : port(port) {}

    void run()
    {
        capu::int32_t communication_variable;
        capu::int32_t numBytes = 0;
        //ALLOCATION AND SYNCH OF cient and  server
        capu::TcpSocket* cli_socket = new capu::TcpSocket();

        //wait for server to start up
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        cond = false;
        if (0 == port)
        {
            port = chosenPort;
        }
        mutex.unlock();
        EXPECT_LT(0, port);

        //TRY TO CONNECT TO IPV6
        EXPECT_EQ(capu::CAPU_SOCKET_EADDR, cli_socket->connect("::1", port));

        //connects to he given id
        capu::status_t result = capu::CAPU_ERROR;
        capu::int32_t attemps = 0;
        while (result != capu::CAPU_OK && attemps < 100)
        {
            result = cli_socket->connect("localhost", port);
            attemps++;
            capu::Thread::Sleep(50);
        }
        EXPECT_EQ(capu::CAPU_OK, result);

        capu::int32_t i = 5;
        capu::int32_t sendData;
        //send data
        EXPECT_EQ(capu::CAPU_OK, cli_socket->send((capu::char_t*) &i, sizeof(capu::int32_t), sendData));

        //receive
        capu::status_t res = cli_socket->receive((capu::char_t*) &communication_variable, sizeof(capu::int32_t), numBytes);
        EXPECT_EQ(capu::CAPU_OK, res);

        //CHECK VALUE
        EXPECT_EQ(6, communication_variable);

        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();

        //socket close
        EXPECT_EQ(capu::CAPU_OK, cli_socket->close());
        //deallocating
        delete cli_socket;
    }
};

class ThreadTimeoutOnReceiveClientTest : public capu::Runnable
{
    capu::uint16_t port;
public:
    //timeout test
    ThreadTimeoutOnReceiveClientTest(capu::uint16_t port) : port(port) {}

    void run()
    {
        capu::int32_t communication_variable;
        capu::int32_t numBytes = 0;
        capu::TcpSocket* cli_socket = new capu::TcpSocket();

        cli_socket->setTimeout(2);

        //connects to he given id
        capu::status_t result = capu::CAPU_ERROR;
        //wait for server to start up
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        if (0 == port)
        {
            port = chosenPort;
        }
        cond = false;
        mutex.unlock();
        result = cli_socket->connect("localhost", port);
        ASSERT_TRUE(result == capu::CAPU_OK);

        capu::int32_t i = 5;

        capu::int32_t sentBytes;
        //send data
        EXPECT_EQ(capu::CAPU_OK, cli_socket->send((capu::char_t*) &i, sizeof(capu::int32_t), sentBytes));

        //receive
        EXPECT_EQ(capu::CAPU_ETIMEOUT, cli_socket->receive((capu::char_t*) &communication_variable, sizeof(capu::int32_t), numBytes));

        //client has received timeout, server can close socket
        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();
        //socket close
        EXPECT_EQ(capu::CAPU_OK, cli_socket->close());
        //deallocating
        delete cli_socket;
    }
};

class ThreadTimeoutOnSendClientTest : public capu::Runnable
{
    capu::uint16_t port;
public:
    //timeout test
    ThreadTimeoutOnSendClientTest(capu::uint16_t port) : port(port) {}

private:
    capu::TcpSocket cli_socket;

    /*
    TCP stacks on different platforms behave differently in case a timeout
    on send occurred. This function ensures that the combination of states
    are valid.
    */
    void testSendOnASocketWherePreviousSendTimedOut(capu::status_t previousErrorOnSend)
    {

        //try to send again on broken socket
        capu::char_t data = 5;
        capu::int32_t sentBytes = 0;
        capu::status_t currentErrorOnSend = cli_socket.send(&data, sizeof(capu::int32_t), sentBytes);
        if (previousErrorOnSend == capu::CAPU_ERROR)
        {
            EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, currentErrorOnSend);
        }

        if (previousErrorOnSend == capu::CAPU_ETIMEOUT)
        {
            EXPECT_EQ(capu::CAPU_ETIMEOUT, currentErrorOnSend);
        }

    }

    void testCloseSocketOnASocketWherePreviousSendTimedOut(capu::status_t previousErrorOnSend)
    {
        if (previousErrorOnSend == capu::CAPU_ERROR)
        {
            EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, cli_socket.close());
        }

        if (previousErrorOnSend == capu::CAPU_ETIMEOUT)
        {
            EXPECT_EQ(capu::CAPU_OK, cli_socket.close());
        }
    }

public:

    void run()
    {
        cli_socket.setTimeout(200);

        //wait for server to start up
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        if (0 == port)
        {
            port = chosenPort;
        }
        cond = false;
        mutex.unlock();

        capu::status_t connectStatus = capu::CAPU_ERROR;
        while (connectStatus != capu::CAPU_OK) {
            connectStatus = cli_socket.connect("localhost", port);
            if (connectStatus != capu::CAPU_OK) {
                capu::Thread::Sleep(100);
            }
        }

        const capu::uint32_t dataSize = 1024 * 1024 / 8;
        capu::uint32_t messageCount = 100;

        capu::char_t* data = new capu::char_t[dataSize];

        capu::Memory::Set(data, 0x00000000, dataSize);

        capu::int32_t sentBytes;
        capu::status_t status;

        while(messageCount--)
        {
            status = cli_socket.send(data, dataSize, sentBytes);
            if (status == capu::CAPU_ERROR || status == capu::CAPU_ETIMEOUT) {
                break;
            }
        }

        //Some TCP stacks a timeout on send returns a CAPU_ERROR as the connection has been closed.
        //Other stacks return a CAPU_ETIMEOUT and the socket can be still used.
        EXPECT_THAT(status, testing::AnyOf(capu::CAPU_ERROR, capu::CAPU_ETIMEOUT));

        testSendOnASocketWherePreviousSendTimedOut(status);

        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();

        testCloseSocketOnASocketWherePreviousSendTimedOut(status);

        delete[] data;
    }
};

class ThreadServerTest : public capu::Runnable
{
    capu::uint16_t port;
public:
    //SERVER thread to test data exchange between client and server
    ThreadServerTest(capu::uint16_t port) : port(port) {}

    void run()
    {
        capu::int32_t communication_variable;
        capu::int32_t numBytes = 0;
        //server socket allocation
        capu::TcpServerSocket* socket = new capu::TcpServerSocket();

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, socket->bind(port, "0.0.0.0"));

        if (0 == port)
        {
            port = socket->port();
            chosenPort = port;
        }

        //start listening
        EXPECT_EQ(capu::CAPU_OK, socket->listen(5));
        //accept connection

        //server is ready to accept clients
        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();
        capu::TcpSocket* new_socket = socket->accept();

        //receive data
        capu::status_t result = capu::CAPU_ERROR;

        result = new_socket->receive((capu::char_t*) &communication_variable, sizeof(capu::int32_t), numBytes);
        EXPECT_EQ(capu::CAPU_OK, result);
        //CHECK VALUE
        EXPECT_EQ(5, communication_variable);
        //update data
        communication_variable++;


        capu::int32_t sentBytes;
        //send it back
        EXPECT_EQ(capu::CAPU_OK, new_socket->send((capu::char_t*) &communication_variable, sizeof(capu::int32_t), sentBytes));

        //wait with close until client has received data
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        cond = false;
        mutex.unlock();
        //close session
        EXPECT_EQ(capu::CAPU_OK, new_socket->close());
        //deallocate session identifier
        delete new_socket;
        EXPECT_EQ(capu::CAPU_OK, socket->close());
        delete socket;
    }
};

class ThreadTimeoutOnReceiveServerTest : public capu::Runnable
{
    capu::uint16_t mPort;

public:
    //timeout test
    ThreadTimeoutOnReceiveServerTest(capu::uint16_t port) : mPort(port) {}
    inline void run()
    {
        capu::int32_t communication_variable;
        capu::int32_t numBytes = 0;
        //server socket allocation
        capu::TcpServerSocket* socket = new capu::TcpServerSocket();

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, socket->bind(mPort, "0.0.0.0"));

        if (0 == mPort)
        {
            mPort = socket->port();
            chosenPort = mPort;
        }

        //start listening
        EXPECT_EQ(capu::CAPU_OK, socket->listen(5));

        //server is ready to accept clients
        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();
        //accept connection
        capu::TcpSocket* new_socket = socket->accept();
        capu::status_t result = capu::CAPU_ERROR;

        result = new_socket->receive((capu::char_t*) &communication_variable, sizeof(capu::int32_t), numBytes);
        EXPECT_EQ(capu::CAPU_OK, result);
        //CHECK VALUE
        EXPECT_EQ(5, communication_variable);

        //wait for timeout on client side
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        cond = false;
        mutex.unlock();

        //close session
        EXPECT_EQ(capu::CAPU_OK, new_socket->close());
        //deallocate session identifier
        delete new_socket;
        EXPECT_EQ(capu::CAPU_OK, socket->close());
        delete socket;
    }
};

class ThreadTimeoutOnSendServerTest : public capu::Runnable
{
    capu::uint16_t mPort;

public:
    //timeout test
    ThreadTimeoutOnSendServerTest(capu::uint16_t port) : mPort(port) {}
    inline void run()
    {
        //server socket allocation
        capu::TcpServerSocket* socket = new capu::TcpServerSocket();

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, socket->bind(mPort, "0.0.0.0"));

        if (0 == mPort)
        {
            mPort = socket->port();
            chosenPort = mPort;
        }

        //start listening
        EXPECT_EQ(capu::CAPU_OK, socket->listen(5));

        //server is ready to accept clients
        mutex.lock();
        cond = true;
        cv.signal();
        mutex.unlock();
        //accept connection
        capu::TcpSocket* new_socket = socket->accept();

        //do not call receive to cause a timeout on sender side

        //wait for timeout on client side
        mutex.lock();
        while (!cond)
        {
            cv.wait(&mutex);
        }
        cond = false;
        mutex.unlock();

        //close session
        EXPECT_EQ(capu::CAPU_OK, new_socket->close());
        //deallocate session identifier
        delete new_socket;
        EXPECT_EQ(capu::CAPU_OK, socket->close());
        delete socket;
    }
};

class ThreadReconnectServerTest : public capu::Runnable
{
    capu::uint16_t mPort;
    capu::TcpServerSocket mSocket;
public:
    //timeout test
    ThreadReconnectServerTest(capu::uint16_t port) : mPort(port)
    {

        //bind to given address
        EXPECT_EQ(capu::CAPU_OK, mSocket.bind(mPort, "0.0.0.0"));
        if (0 == mPort)
        {
            mPort = mSocket.port();
        }

        //start listening
        EXPECT_EQ(capu::CAPU_OK, mSocket.listen(5));
    }

    ~ThreadReconnectServerTest()
    {
        mSocket.close();
    }

    inline void run()
    {
        capu::int32_t communication_variable;
        capu::int32_t numBytes = 0;

        capu::TcpSocket* new_socket = mSocket.accept();
        capu::status_t result = capu::CAPU_ERROR;

        result = new_socket->receive((capu::char_t*) &communication_variable, sizeof(capu::int32_t), numBytes);
        EXPECT_EQ(capu::CAPU_OK, result);
        //CHECK VALUE
        EXPECT_EQ(5, communication_variable);

        //close session
        EXPECT_EQ(capu::CAPU_OK, new_socket->close());
        //deallocate session identifier
        delete new_socket;

    }

    inline capu::uint16_t port()
    {
        return mPort;
    }
};

TEST(TcpSocket, ConnectTest)
{
    capu::TcpSocket* socket = new capu::TcpSocket();
    //pass null
    capu::uint16_t port = RandomPort::get();
    EXPECT_EQ(capu::CAPU_EINVAL, socket->connect(NULL, port));

    EXPECT_EQ(capu::CAPU_SOCKET_EADDR, socket->connect("www.test", port));

    delete socket;
}

TEST(TcpSocket, ConnectTwiceTest)
{
    capu::TcpSocket socket;
    socket.setTimeout(50);
    EXPECT_EQ(capu::CAPU_SOCKET_ECONNECT, socket.connect("127.0.0.1", 6556));
    EXPECT_EQ(capu::CAPU_SOCKET_ECONNECT, socket.connect("127.0.0.1", 6556));
}

TEST(TcpSocket, UnconnectedSocketCloseReceiveAndSendTest)
{
    capu::TcpSocket* socket = new capu::TcpSocket();
    capu::int32_t i = 0;
    capu::int32_t numBytes = 0;
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->close());
    //try to send data via closed socket
    capu::int32_t sentBytes;
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->send((capu::char_t*) "asda", 4, sentBytes));
    //try to receive data from closed socket
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->receive((capu::char_t*) &i, 4, numBytes));
    //Deallocation of socket
    delete socket;
}

TEST(TcpSocket, SetAndGetPropertiesTest)
{
    capu::TcpSocket* socket = new capu::TcpSocket();
    capu::TcpServerSocket* serverSocket = new capu::TcpServerSocket();
    //TRY TO CHANGE THE PROPERTIES OF NOT CONNECTED SOCKET
    EXPECT_EQ(capu::CAPU_OK, socket->setBufferSize(1024));
    EXPECT_EQ(capu::CAPU_OK, socket->setKeepAlive(true));
    EXPECT_EQ(capu::CAPU_OK, socket->setLingerOption(true, 90));
    EXPECT_EQ(capu::CAPU_OK, socket->setNoDelay(false));
    EXPECT_EQ(capu::CAPU_OK, socket->setTimeout(90));

    capu::int32_t int_tmp = 0;
    capu::uint16_t short_tmp = 0;
    bool boolmp = false;
    capu::char_t* remoteIP = 0;

    //CHECK THE PROPERTIES ARE CORRECTLY SET
    EXPECT_EQ(capu::CAPU_OK, socket->getBufferSize(int_tmp));

    //On Linux the kernel adjusts the buffer size and set it to doubles of given size (at least)
    //therefore we have to check here for >=
    EXPECT_TRUE(int_tmp >= 1024);
    EXPECT_EQ(capu::CAPU_OK, socket->getKeepAlive(boolmp));
    EXPECT_TRUE(boolmp);
    EXPECT_EQ(capu::CAPU_OK, socket->getLingerOption(boolmp, short_tmp));
    EXPECT_EQ(90, short_tmp);
    EXPECT_TRUE(boolmp);
    EXPECT_EQ(capu::CAPU_OK, socket->getNoDelay(boolmp));
    EXPECT_FALSE(boolmp);
    EXPECT_EQ(capu::CAPU_OK, socket->getTimeout(int_tmp));
    EXPECT_EQ(90, int_tmp);

    serverSocket->bind(0, "0.0.0.0");
    capu::uint16_t port = serverSocket->port();
    serverSocket->listen(3);

    socket->connect("127.0.0.1", port);

    //TRY TO CHANGE THE PROPERTIES OF CONNECTED SOCKET
    EXPECT_EQ(capu::CAPU_OK, socket->setBufferSize(2024));
    EXPECT_EQ(capu::CAPU_OK, socket->setKeepAlive(false));

    EXPECT_EQ(capu::CAPU_OK, socket->setLingerOption(false, 0));
    EXPECT_EQ(capu::CAPU_OK, socket->setNoDelay(true));
    EXPECT_EQ(capu::CAPU_OK, socket->setTimeout(100));

    //CHECK THE PROPERTIES ARE CORRECTLY SET
    EXPECT_EQ(capu::CAPU_OK, socket->getBufferSize(int_tmp));
    EXPECT_EQ(capu::CAPU_OK, socket->getRemoteAddress(&remoteIP));
    EXPECT_STREQ("127.0.0.1", remoteIP);
    
    //kernel adjusts the buffer size and set it to doubles of given size (at least)
    EXPECT_TRUE(int_tmp >= 2024);
    EXPECT_EQ(capu::CAPU_OK, socket->getKeepAlive(boolmp));
    EXPECT_FALSE(boolmp);
    EXPECT_EQ(capu::CAPU_OK, socket->getLingerOption(boolmp, short_tmp));
    EXPECT_FALSE(boolmp);
    EXPECT_EQ(short_tmp, 0);
    EXPECT_EQ(capu::CAPU_OK, socket->getNoDelay(boolmp));
    EXPECT_TRUE(boolmp);
    EXPECT_EQ(capu::CAPU_OK, socket->getTimeout(int_tmp));
    EXPECT_GE(int_tmp, 100);
    EXPECT_LT(int_tmp, 110);

    socket->close();
    //TRY TO CHANGE THE PROPERTIES OF CLOSED SOCKET
    /*
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setBufferSize(1024) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setKeepAlive(true) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setLingerOption(true, 90) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setNoDelay(false) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->setTimeout(90) );
    //TRY TO GET PROPERTIES OF CLOSED SOCKET
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getBufferSize(int_tmp) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getKeepAlive(boolmp) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getLingerOption(boolmp, int_tmp) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getNoDelay(boolmp) );
    EXPECT_EQ(capu::CAPU_SOCKET_ESOCKET, socket->getTimeout(int_tmp) );
     */

    delete socket;
    delete serverSocket;
    delete[] remoteIP;
}

TEST(SocketAndTcpServerSocket, CommunicationTest)
{
    cond = false;
    ThreadServerTest server(0);
    capu::Thread* server_thread = new capu::Thread();
    server_thread->start(server);

    ThreadClientTest client(0);
    capu::Thread* client_thread = new capu::Thread();
    client_thread->start(client);

    //Create two threads which will behave like client and server to test functionality
    server_thread->join();
    client_thread->join();

    delete client_thread;
    delete server_thread;
}

TEST(SocketAndTcpServerSocket, ReconnectTest)
{
    cond = false;
    ThreadReconnectServerTest server(0);
    capu::uint16_t port = server.port();
    ASSERT_LT(0u, port);
    capu::Thread server_thread;

    capu::TcpSocket socket;
    capu::int32_t buffer = 5;
    capu::int32_t sentBytes;

    for (capu::int32_t i = 0 ; i < 5; ++i)
    {
        server_thread.start(server);
        socket.connect("127.0.0.1", port);
        socket.send((capu::char_t*)&buffer, sizeof(capu::int32_t), sentBytes);
        server_thread.join();
    }

}

TEST(SocketAndTcpServerSocket, TimeoutOnReceiveTest)
{
    cond = false;

    ThreadTimeoutOnReceiveServerTest server(0);
    capu::Thread* server_thread = new capu::Thread();
    server_thread->start(server);

    ThreadTimeoutOnReceiveClientTest client(0);
    capu::Thread* client_thread = new capu::Thread();
    client_thread->start(client);

    //client_thread two threads which will behave like client and server to test functionality
    server_thread->join();
    client_thread->join();

    delete client_thread;
    delete server_thread;
}

TEST(SocketAndTcpServerSocket, TimeoutOnSendTest)
{
    cond = false;

    ThreadTimeoutOnSendServerTest server(0);
    capu::Thread* server_thread = new capu::Thread();
    server_thread->start(server);

    ThreadTimeoutOnSendClientTest client(0);
    capu::Thread* client_thread = new capu::Thread();
    client_thread->start(client);

    //client_thread two threads which will behave like client and server to test functionality
    server_thread->join();
    client_thread->join();

    delete client_thread;
    delete server_thread;
}

class TestServer: public capu::Runnable
{
public:
    capu::TcpServerSocket server;
    capu::uint16_t port;
    capu::int32_t receivedLength;
    capu::status_t receivedRetVal;
    capu::Thread t;

    TestServer() : server(), port(0), receivedLength(0), receivedRetVal(0)
    {
        server.bind(port);
        port = server.port();
        server.listen(6);

        t.start(*this);
    }

    void waitForReceived()
    {
        t.join();
    }

    void run()
    {
        capu::TcpSocket* client = server.accept();
        capu::char_t buffer[1000];
        receivedRetVal = client->receive(buffer, sizeof(buffer), receivedLength);
        delete client;
    }
};

TEST(SocketAndTcpServerSocket, ReceiveReturnsOkOnClose)
{
    TestServer server;
    capu::TcpSocket client;
    client.connect("127.0.0.1", server.port);
    client.close();
    server.waitForReceived();
    EXPECT_EQ(capu::CAPU_OK, server.receivedRetVal);
    EXPECT_EQ(0, server.receivedLength);
}

