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

#include "TcpSocketOutputStreamTest.h"
#include <capu/os/TcpSocket.h>
#include <capu/os/TcpServerSocket.h>
#include <capu/os/Thread.h>
#include <capu/util/Guid.h>
#include <capu/util/TcpSocketOutputStream.h>
#include <capu/os/Memory.h>
#include <capu/os/Math.h>
#include <capu/os/CondVar.h>
#include <capu/os/NumericLimits.h>
#include <capu/util/IInputStream.h>
#include <capu/util/SocketInputStream.h>

namespace capu
{
    struct TestTcpSocketReceiver: public IInputStream
    {
        TestTcpSocketReceiver(TcpSocket& socket)
            : mSocket(socket)
        {
        }

        TcpSocket& mSocket;

        void receiveFromSocket(char_t* data, const uint32_t size)
        {
            int32_t length = 0;
            uint32_t receivedBytes = 0;
            char_t* buffer = static_cast<char_t*>(data);
            while (receivedBytes < size)
            {
                status_t result = mSocket.receive(&buffer[receivedBytes], size - receivedBytes, length);
                if (result != CAPU_OK)
                {
                    break;
                }

                if (0 == length)
                {
                    break;
                }

                receivedBytes += length;
            }
        }

        IInputStream& operator>>(float_t& value)
        {
            return operator>>(reinterpret_cast<int32_t&>(value));
        }

        IInputStream& operator>>(int32_t& value)
        {
            int32_t tmp;
            receiveFromSocket(reinterpret_cast<char_t*>(&tmp), sizeof(int32_t));
            value = ntohl(tmp);
            return *this;
        }

        IInputStream& operator>>(uint32_t& value)
        {
            int32_t tmp;
            operator>>(tmp);
            value = static_cast<uint32_t>(tmp);
            return *this;
        }


        IInputStream& operator>>(uint64_t& value)
        {
            uint64_t tmp;
            receiveFromSocket(reinterpret_cast<char_t*>(&tmp), sizeof(uint64_t));
            value = _ntohll(tmp);
            return *this;
        }

        IInputStream& operator>>(int64_t& value)
        {
            uint64_t tmp;
            operator>>(tmp);
            value = static_cast<int64_t>(tmp);
            return *this;

        }

        IInputStream& operator>>(String& value)
        {
            int32_t strLen = 0;
            operator>>(strLen);
            char_t* buf = new char_t[strLen + 1];
            receiveFromSocket(buf, strLen);
            buf[strLen] = 0;
            value = buf;
            delete[] buf;
            return *this;
        }

        IInputStream& operator>>(bool_t& value)
        {
            receiveFromSocket(reinterpret_cast<char_t*>(&value), sizeof(bool_t));
            return *this;
        }

        IInputStream& operator>>(uint16_t& value)
        {
            receiveFromSocket(reinterpret_cast<char_t*>(&value), sizeof(uint16_t));
            value = ntohs(value);
            return *this;
        }

        IInputStream& operator>>(Guid& value)
        {
            generic_uuid_t tmpValue;
            read(reinterpret_cast<char_t*>(&tmpValue), sizeof(generic_uuid_t));
            value = tmpValue;
            return *this;
        }

        IInputStream& read(char_t* data, const uint32_t size)
        {
            receiveFromSocket(data, size);
            return *this;
        }

        status_t getState() const
        {
            return CAPU_OK;
        }
    };

    template<typename T>
    class SocketOutputStreamTestRunnable: public Runnable
    {
    public:
        Mutex mStartupMutex;

        CondVar mStartupCv;

        int16_t mPort;

        SocketOutputStreamTestRunnable()
        {
        }

        T result;
        void run()
        {
            mStartupMutex.lock();
            TcpServerSocket serverSocket;
            serverSocket.bind(0);
            mPort = serverSocket.port();
            serverSocket.listen(10);

            mStartupCv.broadcast();
            mStartupMutex.unlock();

            TcpSocket* socket = serverSocket.accept();

            TestTcpSocketReceiver receiver(*socket);
            receiver >> result;

            socket->close();
            delete socket;
        }
    };

    TcpSocketOutputStreamTest::TcpSocketOutputStreamTest()
    {
    }

    TcpSocketOutputStreamTest::~TcpSocketOutputStreamTest()
    {
    }

    void TcpSocketOutputStreamTest::SetUp()
    {
    }

    void TcpSocketOutputStreamTest::TearDown()
    {
    }

    template<typename T>
    struct TcpSocketOutputStreamTestExecutor
    {
        static T Execute(const T& value)
        {
            SocketOutputStreamTestRunnable<T> runnable;
            runnable.mStartupMutex.lock();

            Thread thread;
            thread.start(runnable);

            runnable.mStartupCv.wait(&runnable.mStartupMutex);
            runnable.mStartupMutex.unlock();

            TcpSocket socket;
            capu::status_t result = capu::CAPU_ERROR;
            capu::int32_t attemps = 0;
            while (result != capu::CAPU_OK && attemps < 100)
            {
                result = socket.connect("127.0.0.1", runnable.mPort);
                attemps++;
                capu::Thread::Sleep(50);
            }
            TcpSocketOutputStream<1450> outputStream(socket);

            outputStream << value;
            outputStream.flush();

            thread.join();

            return runnable.result;
        }
    };

    TEST_F(TcpSocketOutputStreamTest, SendInt32Data)
    {
        EXPECT_EQ(5, TcpSocketOutputStreamTestExecutor<int32_t>::Execute(5));
    }

    TEST_F(TcpSocketOutputStreamTest, SendUInt32Data)
    {
        EXPECT_EQ(5u, TcpSocketOutputStreamTestExecutor<uint32_t>::Execute(5u));
        EXPECT_EQ(0u, TcpSocketOutputStreamTestExecutor<uint32_t>::Execute(0u));
        EXPECT_EQ(NumericLimits::Max<uint32_t>(), TcpSocketOutputStreamTestExecutor<uint32_t>::Execute(NumericLimits::Max<uint32_t>()));
    }

    TEST_F(TcpSocketOutputStreamTest, SendInt64Data)
    {
        const uint64_t value = 9223372036854775807LL;
        EXPECT_EQ(value, TcpSocketOutputStreamTestExecutor<uint64_t>::Execute(value));
    }

    TEST_F(TcpSocketOutputStreamTest, SendUInt64Data)
    {
        const uint64_t value = 18446744073709551615uLL;
        EXPECT_EQ(value, TcpSocketOutputStreamTestExecutor<uint64_t>::Execute(value));
        EXPECT_EQ(0u, TcpSocketOutputStreamTestExecutor<uint32_t>::Execute(0u));
        EXPECT_EQ(NumericLimits::Max<uint64_t>(), TcpSocketOutputStreamTestExecutor<uint64_t>::Execute(NumericLimits::Max<uint64_t>()));
    }

    TEST_F(TcpSocketOutputStreamTest, SendFloatData)
    {
        EXPECT_EQ(Math::PI_f, TcpSocketOutputStreamTestExecutor<float_t>::Execute(Math::PI_f));
    }

    TEST_F(TcpSocketOutputStreamTest, SendStringData)
    {
        EXPECT_STREQ("Hello World", TcpSocketOutputStreamTestExecutor<String>::Execute("Hello World").c_str());
    }

    TEST_F(TcpSocketOutputStreamTest, SendUInt16Data)
    {
        EXPECT_EQ((uint16_t)(4), TcpSocketOutputStreamTestExecutor<uint16_t>::Execute(4));
    }

    TEST_F(TcpSocketOutputStreamTest, SendGuidData)
    {
        Guid guid;
        EXPECT_EQ(guid, TcpSocketOutputStreamTestExecutor<Guid>::Execute(guid));
    }

    TEST_F(TcpSocketOutputStreamTest, SendBoolData)
    {
        EXPECT_EQ(true, TcpSocketOutputStreamTestExecutor<bool_t>::Execute(true));
    }

    TEST_F(TcpSocketOutputStreamTest, TestStreamInitialStateIsOk)
    {
        TcpSocket socket;
        TcpSocketOutputStream<1450> stream(socket);
        EXPECT_EQ(CAPU_OK, stream.getState());
    }

    TEST_F(TcpSocketOutputStreamTest, TestFlushFailsOnUnconnectedSocket)
    {
        TcpSocket socket;
        TcpSocketOutputStream<1450> stream(socket);
        String str("hello world");
        stream << str;
        status_t state = stream.flush(); // flush will fail on an unconnected socket
        EXPECT_EQ(CAPU_SOCKET_ESOCKET, state);

        state = stream.flush(); // flush will not succeed because the buffer is in error state
        EXPECT_EQ(CAPU_SOCKET_ESOCKET, state);

        stream.resetState();

        state = stream.flush(); // flush will succeed because the buffer is not in error state and buffer is empty
        EXPECT_EQ(CAPU_OK, state);
    }
}
