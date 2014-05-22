#include "DelegateTest.h"
#include "capu/Config.h"
#include <capu/container/Vector.h>

namespace capu
{

    bool_t g_simpleVoidFunctionCalled = false;
    uint32_t g_uint32Parameter = 0;

    class SimpleClass
    {
    public:
        static uint32_t DoStaticStuff(const uint32_t value)
        {
            return value;
        }

        void doStuff()
        {

        }

        virtual void doVirtualStuff(uint32_t)
        {
        }

        virtual void doPureVirtualStuff(uint32_t value) = 0;

        virtual void doPureVirtualStuff2(uint32_t value1, uint32_t value2) = 0;

        uint32_t doNonStaticStuffWOParam()
        {
            return 1337u;
        }

        uint32_t doNonStaticStuff(uint32_t value)
        {
            return value;
        }

        uint32_t doMoreNonStaticStuff(uint32_t value1, uint32_t value2)
        {
            return value1 + value2;
        }
    };

    class MockSimpleClass: public SimpleClass
    {
    public:
        MOCK_METHOD1(doVirtualStuff, void(uint32_t value));
        MOCK_METHOD1(doPureVirtualStuff, void(uint32_t value));
        MOCK_METHOD2(doPureVirtualStuff2, void(uint32_t value1, uint32_t value2));
    };

    DelegateTest::DelegateTest()
    {
        g_simpleVoidFunctionCalled = false;
        g_uint32Parameter = 0;
    }

    void SimpleVoidFunction()
    {
        g_simpleVoidFunctionCalled = true;
    }

    void OneParameterVoidFunction(const uint32_t value)
    {
        g_uint32Parameter = value;
    }

    uint32_t OneParameterFunctionWithReturnValue(const uint32_t value)
    {
        return value;
    }

    void TwoParameterVoidFunction(const uint32_t value1, const uint32_t value2)
    {
        g_uint32Parameter = value1 + value2;
    }

    uint32_t TwoParameterFunctionWithReturnValue(const uint32_t value1, const uint32_t value2)
    {
        return value1 + value2;
    }

    TEST_F(DelegateTest, DelegateSimpleVoidFunction)
    {
        Delegate<void> delegate = Delegate<void>::Create<&SimpleVoidFunction>();

        delegate();

        EXPECT_TRUE(g_simpleVoidFunctionCalled);
    }


    TEST_F(DelegateTest, DelegateSimpleOneParameterFunction)
    {
        Delegate<void, uint32_t> delegate = Delegate<void, uint32_t>::Create<&OneParameterVoidFunction>();
        delegate(5);
 
        EXPECT_EQ(5u, g_uint32Parameter);
    }

    TEST_F(DelegateTest, DelegateSimpleOneParameterFunctionWithReturnValue)
    {
        Delegate<uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t>::Create<&OneParameterFunctionWithReturnValue>();

        EXPECT_EQ(5u, delegate(5u));
    }

    TEST_F(DelegateTest, DelegateSimpleTwoParameterFunction)
    {
        Delegate<void, uint32_t, uint32_t> delegate = Delegate<void, uint32_t, uint32_t>::Create<&TwoParameterVoidFunction>();
        delegate(5, 2);
 
        EXPECT_EQ(7u, g_uint32Parameter);
    }

    TEST_F(DelegateTest, DelegateSimpleTwoParameterFunctionWithReturnValue)
    {
        Delegate<uint32_t, uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t, uint32_t>::Create<&TwoParameterFunctionWithReturnValue>();

        EXPECT_EQ(7u, delegate(5u, 2u));
    }

    TEST_F(DelegateTest, DelegateStaticMemberFunctionWithReturnValue)
    {
        Delegate<uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t>::Create<&MockSimpleClass::DoStaticStuff>();

        EXPECT_EQ(5u, delegate(5u));
    }

    TEST_F(DelegateTest, DelegateNonStaticMemberFunctionWithReturnValue)
    {
        MockSimpleClass simpleClass;
        Delegate<uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t>::Create<SimpleClass, &MockSimpleClass::doNonStaticStuff>(simpleClass);

        EXPECT_EQ(5u, delegate(5u));
    }

    TEST_F(DelegateTest, DelegateNonStaticMemberFuncion)
    {
        MockSimpleClass simpleClass;
        Delegate<uint32_t> delegate = Delegate<uint32_t>::Create<SimpleClass, &MockSimpleClass::doNonStaticStuffWOParam>(simpleClass);

        EXPECT_EQ(1337u, delegate());
    }

    TEST_F(DelegateTest, DelegateNonStaticMemberFuncionWithTwoParametersAndReturnValue)
    {
        MockSimpleClass simpleClass;
        Delegate<uint32_t, uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t, uint32_t>::Create<SimpleClass, &MockSimpleClass::doMoreNonStaticStuff>(simpleClass);

        EXPECT_EQ(9u, delegate(5u, 4u));
    }

    TEST_F(DelegateTest, DelegateVirtualMethod)
    {
        MockSimpleClass simpleClass;
        Delegate<void, uint32_t> delegate = Delegate<void, uint32_t>::Create<SimpleClass, &SimpleClass::doVirtualStuff>(simpleClass);

        EXPECT_CALL(simpleClass, doVirtualStuff(5));
        delegate(5);
    }

    TEST_F(DelegateTest, DelegatePureVirtualMethod)
    {
        MockSimpleClass simpleClass;
        Delegate<void, uint32_t> delegate = Delegate<void, uint32_t>::Create<SimpleClass, &SimpleClass::doPureVirtualStuff>(simpleClass);

        EXPECT_CALL(simpleClass, doPureVirtualStuff(5));
        delegate(5);
    }

    TEST_F(DelegateTest, Compare)
    {
        MockSimpleClass simpleClass;
        Delegate<void, uint32_t> delegate1 = Delegate<void, uint32_t>::Create<SimpleClass, &SimpleClass::doPureVirtualStuff>(simpleClass);
        Delegate<void, uint32_t> delegate2 = Delegate<void, uint32_t>::Create<SimpleClass, &SimpleClass::doPureVirtualStuff>(simpleClass);

        EXPECT_EQ(delegate1, delegate2);

        Delegate<void, uint32_t, uint32_t> delegate3 = Delegate<void, uint32_t, uint32_t>::Create<SimpleClass, &SimpleClass::doPureVirtualStuff2>(simpleClass);
        Delegate<void, uint32_t, uint32_t> delegate4 = Delegate<void, uint32_t, uint32_t>::Create<SimpleClass, &SimpleClass::doPureVirtualStuff2>(simpleClass);

        EXPECT_EQ(delegate3, delegate4);

        Delegate<void> delegate5 = Delegate<void>::Create<SimpleClass, &SimpleClass::doStuff>(simpleClass);
        Delegate<void> delegate6 = Delegate<void>::Create<SimpleClass, &SimpleClass::doStuff>(simpleClass);

        EXPECT_EQ(delegate5, delegate6);
    }

}


