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

#include "UdpSocketInputStreamTest.h"
#include <capu/util/UdpSocketInputStream.h>
#include <capu/util/Runnable.h>
#include <capu/os/Thread.h>
#include <capu/util/BinaryOutputStream.h>
#include <capu/util/SocketOutputStream.h>

#include <capu/os/Math.h>

namespace capu
{
    struct TestUdpSocketSender
    {
        static void SendToSocket(const capu::uint16_t port, UdpSocket& socket, const char_t* data, const uint32_t size)
        {
            socket.send(data, size, "127.0.0.1", port);
        }
    };

    struct UdpInt32TestSender: public TestUdpSocketSender
    {
        typedef int32_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const int32_t& data)
        {
            int32_t tmp = htonl(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(int32_t));
        }
    };

    struct UdpUInt32TestSender: public TestUdpSocketSender
    {
        typedef uint32_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const uint32_t& data)
        {
            int32_t tmp = htonl(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint32_t));
        }
    };

    struct UdpInt64TestSender: public TestUdpSocketSender
    {
        typedef int64_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const int64_t& data)
        {
            int64_t tmp = _htonll(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(int64_t));
        }
    };

    struct UdpUInt64TestSender: public TestUdpSocketSender
    {
        typedef uint64_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const uint64_t& data)
        {
            int64_t tmp = _htonll(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint64_t));
        }
    };

    struct UdpUInt16TestSender: public TestUdpSocketSender
    {
        typedef uint16_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const uint16_t& data)
        {
            uint16_t tmp = htons(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(uint16_t));
        }
    };

    struct UdpInt16TestSender: public TestUdpSocketSender
    {
        typedef int16_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const int16_t& data)
        {
            int16_t tmp = htons(data);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&tmp), sizeof(int16_t));
        }
    };

    struct UdpInt8TestSender: public TestUdpSocketSender
    {
        typedef int8_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const int8_t& data)
        {
            SendToSocket(port, socket, reinterpret_cast<const char_t*>(&data), sizeof(int8_t));
        }
    };

    struct UdpUInt8TestSender: public TestUdpSocketSender
    {
        typedef uint8_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const uint8_t& data)
        {
            SendToSocket(port, socket, reinterpret_cast<const char_t*>(&data), sizeof(uint8_t));
        }
    };

    struct UdpBoolTestSender: public TestUdpSocketSender
    {
        typedef bool_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const bool_t& data)
        {
            SendToSocket(port, socket, reinterpret_cast<const char_t*>(&data), sizeof(bool_t));
        }
    };

    struct UdpFloatTestSender: public TestUdpSocketSender
    {
        typedef float_t VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const float_t& data)
        {
            const int32_t tmp = htonl(*reinterpret_cast<const int32_t*>(&data));
            SendToSocket(port, socket, reinterpret_cast<const char_t*>(&tmp), sizeof(int32_t));
        }
    };

    struct UdpStringTestSender: public TestUdpSocketSender
    {
        typedef String VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const String& data)
        {
            int32_t strLen = static_cast<uint32_t>(data.getLength() + 1);
            int32_t strNetLent  = htonl(strLen);
            SendToSocket(port, socket, reinterpret_cast<char_t*>(&strNetLent), sizeof(int32_t));
            SendToSocket(port, socket, data.c_str(), strLen);
        }
    };

    struct UdpGuidTestSender: public TestUdpSocketSender
    {
        typedef Guid VALUE_TYPE;
        static void Send(const capu::uint16_t port, UdpSocket& socket, const Guid& data)
        {
            SendToSocket(port, socket, reinterpret_cast<const char_t*>(&(data.getGuidData())), sizeof(generic_uuid_t));
        }
    };

    template<class T>
    class TestUdpSenderRunnable: public Runnable
    {
    public:
        TestUdpSenderRunnable(const capu::uint16_t port, const typename T::VALUE_TYPE& value)
            : mValue(value)
            , mPort(port)
        {
        }

        void run()
        {
            UdpSocket socket;

            T::Send(mPort, socket, mValue);
        }
    private:
        const typename T::VALUE_TYPE& mValue;
        const capu::uint16_t mPort;
    };

    class TestUdpMultipleSender: public Runnable
    {
    public:
        TestUdpMultipleSender(const uint16_t port, const int32_t intValue, const String& stringValue, const float_t floatValue, const bool_t boolValue)
            : mIntValue(intValue)
            , mStringValue(stringValue)
            , mFloatValue(floatValue)
            , mBoolValue(boolValue)
            , mPort(port)
        {
        }

        void run()
        {
            UdpSocket socket;

            char_t buffer[1450];

            char_t* pos = buffer;
            int32_t networkInt   =  htonl(mIntValue);
            
            union
            {
                float_t floatVal;
                int32_t int32Val;
            } int32Convert;

            int32Convert.floatVal = mFloatValue;            

            int32_t networkFloat =  htonl(int32Convert.int32Val);
            int32_t strLen       =  static_cast<int32_t>(mStringValue.getLength());
            int32_t netowrkStrlen = htonl(strLen);

            Memory::Copy(pos, reinterpret_cast<char_t*>(&networkInt), sizeof(int32_t));
            pos += sizeof(int32_t);
            Memory::Copy(pos, reinterpret_cast<char_t*>(&netowrkStrlen), sizeof(int32_t));
            pos += sizeof(int32_t);
            Memory::Copy(pos, mStringValue.c_str(), strLen);
            pos += strLen;
            Memory::Copy(pos, reinterpret_cast<char_t*>(&networkFloat), sizeof(int32_t));
            pos += sizeof(int32_t);
            Memory::Copy(pos, reinterpret_cast<char_t*>(&mBoolValue), sizeof(bool_t));
            pos += sizeof(bool_t);

            socket.send(buffer, static_cast<int32_t>(pos - buffer), "127.0.0.1", mPort);
        }
    private:
        int32_t mIntValue;
        String  mStringValue;
        float_t mFloatValue;
        bool_t  mBoolValue;
        uint16_t mPort;
    };

    UdpSocketInputStreamTest::UdpSocketInputStreamTest()
    {
    }

    UdpSocketInputStreamTest::~UdpSocketInputStreamTest()
    {
    }

    void UdpSocketInputStreamTest::SetUp()
    {
    }

    void UdpSocketInputStreamTest::TearDown()
    {
    }

    struct UdpSocketInputStreamTestExecutor
    {
        template<typename T>
        static typename T::VALUE_TYPE Execute(const typename T::VALUE_TYPE& value)
        {
            UdpSocket socket;
            socket.bind(0, 0);

            TestUdpSenderRunnable<T> testSender(socket.getSocketAddrInfo().port, value);
            Thread thread;
            thread.start(testSender);

            UdpSocketInputStream<1450> inStream(socket);

            typename T::VALUE_TYPE resultValue;
            inStream >> resultValue;

            thread.join();

            return resultValue;
        }
    };

    TEST_F(UdpSocketInputStreamTest, ReceiveInt32)
    {
        EXPECT_EQ(5, UdpSocketInputStreamTestExecutor::Execute<UdpInt32TestSender>(5));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveUInt32)
    {
        EXPECT_EQ(5u, UdpSocketInputStreamTestExecutor::Execute<UdpUInt32TestSender>(5u));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveInt64)
    {
        const int64_t value = 9223372036854775807LL;
        EXPECT_EQ(value, UdpSocketInputStreamTestExecutor::Execute<UdpInt64TestSender>(value));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveUInt64)
    {
        const uint64_t value = 18446744073709551615uLL;
        EXPECT_EQ(value, UdpSocketInputStreamTestExecutor::Execute<UdpUInt64TestSender>(value));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveString)
    {
        EXPECT_STREQ("Hello World", UdpSocketInputStreamTestExecutor::Execute<UdpStringTestSender>("Hello World").c_str());
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveFloat)
    {
        EXPECT_FLOAT_EQ(Math::PI_f, UdpSocketInputStreamTestExecutor::Execute<UdpFloatTestSender>(Math::PI_f));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveBool)
    {
        EXPECT_EQ(true, UdpSocketInputStreamTestExecutor::Execute<UdpBoolTestSender>(true));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveUInt16)
    {
        EXPECT_EQ(4, UdpSocketInputStreamTestExecutor::Execute<UdpUInt16TestSender>(4));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveInt16)
    {
        EXPECT_EQ(4, UdpSocketInputStreamTestExecutor::Execute<UdpInt16TestSender>(4));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveInt8)
    {
        EXPECT_EQ(5, UdpSocketInputStreamTestExecutor::Execute<UdpInt8TestSender>(5));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveUInt8)
    {
        EXPECT_EQ(5u, UdpSocketInputStreamTestExecutor::Execute<UdpUInt8TestSender>(5u));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveGuid)
    {
        Guid guid;
        EXPECT_EQ(guid, UdpSocketInputStreamTestExecutor::Execute<UdpGuidTestSender>(guid));
    }

    TEST_F(UdpSocketInputStreamTest, ReceiveSomeData)
    {
        UdpSocket serverSocket;
        serverSocket.bind(0, 0);

        TestUdpMultipleSender sender(serverSocket.getSocketAddrInfo().port, 5, "Hello World", Math::LN2_f, true);
        Thread thread;
        thread.start(sender);

        UdpSocketInputStream<1450> inStream(serverSocket);

        int32_t intResult;
        String  stringResult;
        float_t floatResult;
        bool_t  boolResult;

        inStream >> intResult >> stringResult >> floatResult >> boolResult;

        thread.join();

        serverSocket.close();

        EXPECT_EQ(5, intResult);
        EXPECT_STREQ("Hello World", stringResult.c_str());
        EXPECT_FLOAT_EQ(Math::LN2_f, floatResult);
        EXPECT_EQ(true, boolResult);
    }
}
