/*
 * Copyright (C) 2016 BMW Car IT GmbH
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
#include "capu/os/Atomic.h"
#include "capu/os/NumericLimits.h"
#include "capu/os/Mutex.h"
#include "capu/os/Thread.h"
#include "capu/util/Runnable.h"
#include <type_traits>

#if defined(ARCH_X86_64) || defined(ARCH_ARM64)
#define CAPU_HAS_64BIT_ATOMICS
#endif

namespace {
    template <typename T, typename Enable = void>
    struct ValueProvider;

    template <>
    struct ValueProvider<bool>
    {
        static bool FirstValue()  { return false; }
        static bool SecondValue() { return false; }
    };

    template <typename T>
    struct ValueProvider<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
        static T FirstValue()  { return 1; }
        static T SecondValue() { return 2; }
    };

    template <typename T>
    struct ValueProvider<T*>
    {
        static T value[2];
        static T* FirstValue()  { return &value[0]; }
        static T* SecondValue() { return &value[1]; }
    };

    template <typename T>
    T ValueProvider<T*>::value[2];
}

template <typename T>
class AtomicTestBase : public ::testing::Test
{
public:
    AtomicTestBase()
        : first(ValueProvider<T>::FirstValue())
        , second(ValueProvider<T>::SecondValue())
    {
    }

    T first;
    T second;
};


template <typename T>
class AtomicTest : public AtomicTestBase<T>
{
};

typedef ::testing::Types<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
#ifdef CAPU_HAS_64BIT_ATOMICS
                         , uint64_t, int64_t
#endif
                         , int*> AllAtomicTypes;
TYPED_TEST_CASE(AtomicTest, AllAtomicTypes);

TYPED_TEST(AtomicTest, DefaultConstruction)
{
    capu::Atomic<TypeParam> uninitialized;
    UNUSED(uninitialized);  // cannot expect any value but it must compile
}

TYPED_TEST(AtomicTest, ValueConstruction)
{
    capu::Atomic<TypeParam> a(this->first);
    capu::Atomic<TypeParam> b(this->second);

    EXPECT_EQ(this->first, a);
    EXPECT_EQ(this->second, b);
}

TYPED_TEST(AtomicTest, CanLoadValue)
{
    capu::Atomic<TypeParam> a(this->first);
    capu::Atomic<TypeParam> b(this->second);
    EXPECT_EQ(this->first, a.load());
    EXPECT_EQ(this->second, b.load());
}

TYPED_TEST(AtomicTest, CanRetrievViaConversionOperator)
{
    capu::Atomic<TypeParam> a(this->first);
    capu::Atomic<TypeParam> b(this->second);
    TypeParam v = a;
    TypeParam w = b;
    EXPECT_EQ(this->first, v);
    EXPECT_EQ(this->second, w);
}

TYPED_TEST(AtomicTest, CanStoreValue)
{
    capu::Atomic<TypeParam> a(this->first);
    a.store(this->second);
    EXPECT_EQ(this->second, a);
}

TYPED_TEST(AtomicTest, CanAssignValue)
{
    capu::Atomic<TypeParam> a(this->first);
    a = this->second;
    EXPECT_EQ(this->second, a);
}


template <typename T>
class AtomicTestIntegral : public AtomicTestBase<T>
{
};

typedef ::testing::Types<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
#ifdef CAPU_HAS_64BIT_ATOMICS
                         , uint64_t, int64_t
#endif
                         > IntegralAtomicTypes;
TYPED_TEST_CASE(AtomicTestIntegral, IntegralAtomicTypes);

TYPED_TEST(AtomicTestIntegral, CanPreIncrement)
{
    capu::Atomic<TypeParam> a(this->first);
    ++a;
    EXPECT_EQ(this->first + 1, a);
}

TYPED_TEST(AtomicTestIntegral, CanPreDecrement)
{
    capu::Atomic<TypeParam> a(this->first);
    --a;
    EXPECT_EQ(this->first - 1, a);
}

TYPED_TEST(AtomicTestIntegral, CanPostIncrement)
{
    capu::Atomic<TypeParam> a(this->first);
    EXPECT_EQ(this->first, a++);
    EXPECT_EQ(this->first + 1, a);
}


TYPED_TEST(AtomicTestIntegral, CanPostDecrement)
{
    capu::Atomic<TypeParam> a(this->first);
    EXPECT_EQ(this->first, a--);
    EXPECT_EQ(this->first - 1, a);
}

TYPED_TEST(AtomicTestIntegral, CanAssignAdd)
{
    capu::Atomic<TypeParam> a(this->first);
    EXPECT_EQ(this->first + 1, a += 1);
    EXPECT_EQ(this->first + 1, a);
}

TYPED_TEST(AtomicTestIntegral, CanAssignSub)
{
    capu::Atomic<TypeParam> a(this->first);
    EXPECT_EQ(this->first - 1, a -= 1);
    EXPECT_EQ(this->first - 1, a);
}

template <typename T>
class AtomicTestUnsignedIntegral : public AtomicTestBase<T>
{
};

typedef ::testing::Types<uint8_t, uint16_t, uint32_t
#ifdef CAPU_HAS_64BIT_ATOMICS
                         , uint64_t
#endif
                         > UnsignedIntegralAtomicTypes;
TYPED_TEST_CASE(AtomicTestUnsignedIntegral, UnsignedIntegralAtomicTypes);

TYPED_TEST(AtomicTestUnsignedIntegral, DoesNotAffectSurroundingValues)
{
    TypeParam a(0);
    capu::Atomic<TypeParam> b(0);
    TypeParam c(0);

    b -= 1;

    EXPECT_EQ(0u, a);
    EXPECT_EQ(capu::NumericLimits<TypeParam>::Max(), b);
    EXPECT_EQ(0u, c);
}

TYPED_TEST(AtomicTestUnsignedIntegral, DoesNotAffectSurroundingAtomics)
{
    capu::Atomic<TypeParam> a(0);
    capu::Atomic<TypeParam> b(0);
    capu::Atomic<TypeParam> c(0);

    b -= 1;

    EXPECT_EQ(0u, a);
    EXPECT_EQ(capu::NumericLimits<TypeParam>::Max(), b);
    EXPECT_EQ(0u, c);
}

TEST(AtomicTestWrapAround, Add_Overflow_32bit)
{
    capu::Atomic<uint32_t> val(4294967295u);
    val += 3;
    EXPECT_EQ(2u, val);
}

TEST(AtomicTestWrapAround, Sub_Overflow_32bit)
{
    capu::Atomic<uint32_t> val(0);
    val -= 5;
    EXPECT_EQ(4294967291u, val);
}

#ifdef CAPU_HAS_64BIT_ATOMICS
TEST(AtomicTestWrapAround, Add_Overflow_64bit) {
    capu::Atomic<uint64_t> val(18446744073709551615u);
    val += 3;
    EXPECT_EQ(2u, val);
}

TEST(AtomicTestWrapAround, Sub_Overflow_64bit) {
    capu::Atomic<uint64_t> val(0);
    val -= 5;
    EXPECT_EQ(18446744073709551611u, val);
}
#endif


namespace
{
    struct Globals {
        static capu::Atomic<capu::uint_t> var;
        static capu::uint_t sum;
        static capu::uint_t n;
        static capu::Mutex mutex;
    };

    capu::Atomic<capu::uint_t> Globals::var(1);
    capu::uint_t Globals::sum = 0;
    capu::uint_t Globals::n = 100000;
    capu::Mutex Globals::mutex;
};


class AtomicOperationIncrementThread : public capu::Runnable
{
public:
    void run()
    {
        capu::uint_t oldValue = 0;
        for (capu::uint_t i = 0; i < Globals::n; i++)
        {
            oldValue += Globals::var++;
        }

        Globals::mutex.lock();
        Globals::sum += oldValue;
        Globals::mutex.unlock();
    }
};

class AtomicOperationDecrementThread : public capu::Runnable
{
public:
    void run()
    {
        capu::uint_t oldValue = 0;
        for (capu::uint_t i = 0; i < Globals::n; i++)
        {
            oldValue += Globals::var--;
        }

        Globals::mutex.lock();
        Globals::sum += oldValue;
        Globals::mutex.unlock();
    }
};

TEST(AtomicOperation, Atomicity)
{
    capu::Thread thread1;
    capu::Thread thread2;
    capu::Thread thread3;
    capu::Thread thread4;
    capu::Thread thread5;
    AtomicOperationIncrementThread runnable1;
    AtomicOperationIncrementThread runnable2;
    AtomicOperationIncrementThread runnable3;
    AtomicOperationIncrementThread runnable4;
    AtomicOperationIncrementThread runnable5;

    thread1.start(runnable1);
    thread2.start(runnable2);
    thread3.start(runnable3);
    thread4.start(runnable4);
    thread5.start(runnable5);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    EXPECT_EQ(Globals::n * 5 + 1, Globals::var);
    EXPECT_EQ(((Globals::n * 5 * Globals::n * 5) + Globals::n * 5) / 2, Globals::sum);

    AtomicOperationDecrementThread runnable6;
    AtomicOperationDecrementThread runnable7;
    AtomicOperationDecrementThread runnable8;
    AtomicOperationDecrementThread runnable9;
    AtomicOperationDecrementThread runnable10;

    Globals::var = Globals::n * 5;
    Globals::sum = 0;

    thread1.start(runnable6);
    thread2.start(runnable7);
    thread3.start(runnable8);
    thread4.start(runnable9);
    thread5.start(runnable10);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    EXPECT_EQ(0u, Globals::var);
    EXPECT_EQ(((Globals::n * 5 * Globals::n * 5) + Globals::n * 5) / 2, Globals::sum);
}
