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

        uint32_t doNonStaticStuff(uint32_t value)
        {
            return value;
        }

        uint32_t doMoreNonStaticStuff(uint32_t value1, uint32_t value2)
        {
            return value1 + value2;
        }
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

    TEST_F(DelegateTest, DelegateStaticMemberFunctionWithReturnValue)
    {
        Delegate<uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t>::Create<&SimpleClass::DoStaticStuff>();

        EXPECT_EQ(5u, delegate(5u));
    }

    TEST_F(DelegateTest, DelegateNonStaticMemberFunctionWithReturnValue)
    {
        SimpleClass simpleClass;
        Delegate<uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t>::Create<SimpleClass, &SimpleClass::doNonStaticStuff>(simpleClass);

        EXPECT_EQ(5u, delegate(5u));
    }

    TEST_F(DelegateTest, DelegateNonStaticMemberFuncionWithTwoParametersAndReturnValue)
    {
        SimpleClass simpleClass;
        Delegate<uint32_t, uint32_t, uint32_t> delegate = Delegate<uint32_t, uint32_t, uint32_t>::Create<SimpleClass, &SimpleClass::doMoreNonStaticStuff>(simpleClass);

        EXPECT_EQ(9u, delegate(5u, 4u));
    }
}


