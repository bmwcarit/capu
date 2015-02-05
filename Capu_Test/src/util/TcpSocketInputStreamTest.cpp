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

#include "TcpSocketInputStreamTest.h"
#include <capu/os/TcpSocket.h>
#include <capu/os/TcpServerSocket.h>
#include <capu/os/Thread.h>
#include <capu/os/Math.h>
#include <capu/os/NumericLimits.h>
#include <capu/os/Console.h>
#include <capu/util/TestUtils.h>
#include <capu/util/SocketOutputStream.h>
#include <capu/util/TcpSocketOutputStream.h>


namespace capu
{
    struct TestTcpSocketSender
    {
        static void SendToSocket(TcpSocket& socket, const char_t* data, const uint32_t size)
        {
            int32_t numBytes;
            socket.send(data, size, numBytes);
        }
    };

    struct TcpInt32TestSender: public TestTcpSocketSender
    {
        typedef int32_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const int32_t& data)
        {
            int32_t tmp = htonl(data);
            SendToSocket(socket, reinterpret_cast<char_t*>(&tmp), sizeof(int32_t));
        }
    };

    struct TcpUInt32TestSender: public TestTcpSocketSender
    {
        typedef uint32_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const uint32_t& data)
        {
            int32_t tmp = htonl(data);
            SendToSocket(socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint32_t));
        }
    };


    struct TcpUInt64TestSender: public TestTcpSocketSender
    {
        typedef uint64_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const uint64_t& data)
        {
            uint64_t tmp = _htonll(data);
            SendToSocket(socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint64_t));
        }
    };

    struct TcpInt64TestSender: public TestTcpSocketSender
    {
        typedef int64_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const int64_t& data)
        {
            uint64_t tmp = _htonll(static_cast<int64_t>(data));
            SendToSocket(socket, reinterpret_cast<char_t*>(&tmp), sizeof(int64_t));
        }
    };

    struct TcpUInt16TestSender: public TestTcpSocketSender
    {
        typedef uint16_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const uint16_t& data)
        {
            uint16_t tmp = htons(data);
            SendToSocket(socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint16_t));
        }
    };

    struct TcpBoolTestSender: public TestTcpSocketSender
    {
        typedef bool_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const bool_t& data)
        {
            SendToSocket(socket, reinterpret_cast<const char_t*>(&data), sizeof(bool_t));
        }
    };

    struct TcpFloatTestSender: public TestTcpSocketSender
    {
        typedef float_t VALUE_TYPE;
        static void Send(TcpSocket& socket, const float_t& data)
        {
            const int32_t tmp = htonl(*reinterpret_cast<const int32_t*>(&data));
            SendToSocket(socket, reinterpret_cast<const char_t*>(&tmp), sizeof(int32_t));
        }
    };

    struct TcpStringTestSender: public TestTcpSocketSender
    {
        typedef String VALUE_TYPE;
        static void Send(TcpSocket& socket, const String& data)
        {
            int32_t strLen = static_cast<uint32_t>(data.getLength() + 1);
            int32_t strNetLent  = htonl(strLen);
            SendToSocket(socket, reinterpret_cast<char_t*>(&strNetLent), sizeof(int32_t));
            SendToSocket(socket, data.c_str(), strLen);
        }
    };

    struct TcpGuidTestSender: public TestTcpSocketSender
    {
        typedef Guid VALUE_TYPE;
        static void Send(TcpSocket& socket, const Guid& data)
        {
            SendToSocket(socket, reinterpret_cast<const char_t*>(&(data.getGuidData())), sizeof(generic_uuid_t));
        }
    };

    template<class T>
    class TestTcpSenderRunnable: public Runnable
    {
    public:
        TestTcpSenderRunnable(const typename T::VALUE_TYPE& value, const uint16_t port)
            : mValue(value)
            , mPort(port)
        {
        }

        void run()
        {
            TcpSocket socket;
            socket.connect("127.0.0.1", mPort);

            T::Send(socket, mValue);
        }
    private:
        const typename T::VALUE_TYPE& mValue;
        uint16_t mPort;
    };

    class TestTcpMultipleSender: public Runnable
    {
    public:
        TestTcpMultipleSender(const int32_t intValue, const String& stringValue, const float_t floatValue, const bool_t boolValue, const uint16_t port)
            : mIntValue(intValue)
            , mStringValue(stringValue)
            , mFloatValue(floatValue)
            , mBoolValue(boolValue)
            , mPort(port)
        {
        }

        void run()
        {
            TcpSocket socket;
            socket.connect("127.0.0.1", mPort);

            TcpInt32TestSender::Send(socket, mIntValue);
            TcpStringTestSender::Send(socket, mStringValue);
            TcpFloatTestSender::Send(socket, mFloatValue);
            TcpBoolTestSender::Send(socket, mBoolValue);
        }
    private:
        int32_t mIntValue;
        String  mStringValue;
        float_t mFloatValue;
        bool_t  mBoolValue;
        uint16_t mPort;
    };

    TcpSocketInputStreamTest::TcpSocketInputStreamTest()
    {
    }

    TcpSocketInputStreamTest::~TcpSocketInputStreamTest()
    {
    }

    void TcpSocketInputStreamTest::SetUp()
    {
    }

    void TcpSocketInputStreamTest::TearDown()
    {
    }

    struct TcpSocketInputStreamTestExecutor
    {
        template<typename T>
        static typename T::VALUE_TYPE Execute(const typename T::VALUE_TYPE& value)
        {
            TcpServerSocket serverSocket;
            serverSocket.bind(0);
            serverSocket.listen(10);

            TestTcpSenderRunnable<T> testSender(value, serverSocket.port());
            Thread thread;
            thread.start(testSender);

            TcpSocket* socket = serverSocket.accept();

            TcpSocketInputStream inStream(*socket);

            typename T::VALUE_TYPE resultValue;
            inStream >> resultValue;

            thread.join();

            delete socket;
            serverSocket.close();

            return resultValue;
        }
    };

    TEST_F(TcpSocketInputStreamTest, ReceiveInt32)
    {
        EXPECT_EQ(5, TcpSocketInputStreamTestExecutor::Execute<TcpInt32TestSender>(5));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveUInt32)
    {
        EXPECT_EQ(5u, TcpSocketInputStreamTestExecutor::Execute<TcpUInt32TestSender>(5u));
        EXPECT_EQ(0u, TcpSocketInputStreamTestExecutor::Execute<TcpUInt32TestSender>(0u));
        EXPECT_EQ(NumericLimits::Max<uint32_t>(), TcpSocketInputStreamTestExecutor::Execute<TcpUInt32TestSender>(NumericLimits::Max<uint32_t>()));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveInt64)
    {
        const int64_t value = 9223372036854775807LL;
        EXPECT_EQ(value, TcpSocketInputStreamTestExecutor::Execute<TcpInt64TestSender>(value));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveUInt64)
    {
        const uint64_t value = 18446744073709551615uLL;
        EXPECT_EQ(value, TcpSocketInputStreamTestExecutor::Execute<TcpUInt64TestSender>(value));
        EXPECT_EQ(0u, TcpSocketInputStreamTestExecutor::Execute<TcpUInt64TestSender>(0u));
        EXPECT_EQ(NumericLimits::Max<uint64_t>(), TcpSocketInputStreamTestExecutor::Execute<TcpUInt64TestSender>(NumericLimits::Max<uint64_t>()));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveString)
    {
        EXPECT_STREQ("Hello World", TcpSocketInputStreamTestExecutor::Execute<TcpStringTestSender>("Hello World").c_str());
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveBool)
    {
        EXPECT_EQ(true, TcpSocketInputStreamTestExecutor::Execute<TcpBoolTestSender>(true));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveFloat)
    {
        EXPECT_FLOAT_EQ(Math::PI_f, TcpSocketInputStreamTestExecutor::Execute<TcpFloatTestSender>(Math::PI_f));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveGuid)
    {
        Guid guid;
        EXPECT_EQ(guid, TcpSocketInputStreamTestExecutor::Execute<TcpGuidTestSender>(guid));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveUInt16)
    {
        EXPECT_EQ(4, TcpSocketInputStreamTestExecutor::Execute<TcpUInt16TestSender>(4));
    }

    TEST_F(TcpSocketInputStreamTest, ReceiveSomeData)
    {
        TcpServerSocket serverSocket;
        serverSocket.bind(0);
        serverSocket.listen(10);

        TestTcpMultipleSender sender(5, "Hello World", Math::LN2_f, true, serverSocket.port());
        Thread thread;
        thread.start(sender);

        TcpSocket* socket = serverSocket.accept();

        TcpSocketInputStream inStream(*socket);

        int32_t intResult;
        String  stringResult;
        float_t floatResult;
        bool_t  boolResult;

        inStream >> intResult >> stringResult >> floatResult >> boolResult;

        thread.join();

        delete socket;
        serverSocket.close();

        EXPECT_EQ(5, intResult);
        EXPECT_STREQ("Hello World", stringResult.c_str());
        EXPECT_FLOAT_EQ(Math::LN2_f, floatResult);
        EXPECT_EQ(true, boolResult);
    }


    enum SendType
    {
        SENDTYPE_UNKNOWN = -1,
        SENDTYPE_INT32 = 1,
        SENDTYPE_UINT8ARRAY,
        SENDTYPE_UINT32,
        SENDTYPE_FLOAT,
        SENDTYPE_END,
    };
}
