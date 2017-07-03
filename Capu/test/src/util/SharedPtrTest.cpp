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

#include "capu/util/shared_ptr.h"
#include <gtest/gtest.h>


namespace {
    class BaseClass
    {
    public:
        BaseClass(capu::uint_t value)
            : mValue(value)
        {
            ++mReferences;
        }

        ~BaseClass()
        {
            --mReferences;
        }

        capu::uint_t mValue;
        static capu::uint_t mReferences;

    private:
        BaseClass(const BaseClass&);
    };

    capu::uint_t BaseClass::mReferences = 0;

    class DerivedClass : public BaseClass
    {
    public:
        DerivedClass(capu::uint_t value)
            : BaseClass(value)
        {
        }
    };

    template <typename T>
    class TestDeleter
    {
    public:
        TestDeleter(bool& wasCalled)
            : mWasCalled(wasCalled)
        {
        }

        void operator()(T* p)
        {
            assert(!mWasCalled);
            mWasCalled = true;
            delete p;
        }

    private:
        bool& mWasCalled;
    };
}

class SharedPtrTest : public ::testing::Test
{
public:
    SharedPtrTest()
    {
        BaseClass::mReferences = 0;
        wasCalled = false;
    }

    bool wasCalled;
};


TEST_F(SharedPtrTest, DefaultConstructor)
{
    capu::shared_ptr<capu::uint_t> sp;
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_TRUE(0 == sp.get());
}

TEST_F(SharedPtrTest, ConstructorWithNullPointer)
{
    capu::uint_t *nullPtr = 0;
    capu::shared_ptr<capu::uint_t> sp(nullPtr);
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_TRUE(0 == sp.get());
}

TEST_F(SharedPtrTest, PointerConstructor)
{
    capu::shared_ptr<capu::uint_t> sp(new capu::uint_t(123));
    EXPECT_EQ(1u, sp.use_count());
    EXPECT_EQ(123u, *sp);
}

TEST_F(SharedPtrTest, PointerConstructorConvertible)
{
    capu::shared_ptr<BaseClass> sp(new DerivedClass(123));
    EXPECT_EQ(1u, sp.use_count());
    EXPECT_EQ(123u, sp->mValue);
    EXPECT_EQ(1u, sp->mReferences);
}

TEST_F(SharedPtrTest, DestructorOnLastOneOwnerOnly)
{
    {
        capu::shared_ptr<BaseClass> sp(new BaseClass(123));
        EXPECT_EQ(1u, BaseClass::mReferences);
    }
    EXPECT_EQ(0u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, ConstructorWithNullPointerWithDeleter)
{
    capu::uint_t *nullPtr = 0;
    capu::shared_ptr<capu::uint_t> sp(nullPtr, TestDeleter<capu::uint_t>(wasCalled));
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_TRUE(0 == sp.get());
    EXPECT_FALSE(wasCalled);
}

TEST_F(SharedPtrTest, PointerConstructorWithDeleter)
{
    {
        capu::shared_ptr<capu::uint_t> sp(new capu::uint_t(123), TestDeleter<capu::uint_t>(wasCalled));
        EXPECT_EQ(1u, sp.use_count());
        EXPECT_EQ(123u, *sp);
    }
    EXPECT_TRUE(wasCalled);
}

TEST_F(SharedPtrTest, PointerConstructorConvertibleWithDeleter)
{
    {
        capu::shared_ptr<BaseClass> sp(new DerivedClass(123), TestDeleter<BaseClass>(wasCalled));
        EXPECT_EQ(1u, sp.use_count());
        EXPECT_EQ(123u, sp->mValue);
        EXPECT_EQ(1u, sp->mReferences);
    }
    EXPECT_TRUE(wasCalled);
}

TEST_F(SharedPtrTest, CopyConstructor)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other(sp);
    EXPECT_EQ(2u, sp.use_count());
    EXPECT_EQ(2u, other.use_count());
    EXPECT_EQ(123u, other->mValue);
    EXPECT_EQ(1u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, CopyConstructorConvertibleClass)
{
    capu::shared_ptr<DerivedClass> sp(new DerivedClass(123));
    capu::shared_ptr<BaseClass> other(sp);
    EXPECT_EQ(2u, sp.use_count());
    EXPECT_EQ(2u, other.use_count());
    EXPECT_EQ(123u, other->mValue);
    EXPECT_EQ(1u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, DestructorOnLastOwner)
{
    capu::shared_ptr<BaseClass> sp1;
    capu::shared_ptr<BaseClass> sp2;
    capu::shared_ptr<BaseClass> sp3(new BaseClass(123));
    sp1 = sp3;
    sp2 = sp3;
    EXPECT_EQ(3u, sp1.use_count());
    EXPECT_EQ(1u, BaseClass::mReferences);
    sp3.reset();
    sp2.reset();
    EXPECT_EQ(1u, sp1.use_count());
    EXPECT_EQ(1u, BaseClass::mReferences);
    sp1.reset();
    EXPECT_EQ(0u, sp1.use_count());
    EXPECT_EQ(0u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, DeleterOnLastOwner)
{
    capu::shared_ptr<BaseClass> sp1;
    capu::shared_ptr<BaseClass> sp2;
    capu::shared_ptr<BaseClass> sp3(new BaseClass(123), TestDeleter<BaseClass>(wasCalled));
    sp1 = sp3;
    sp2 = sp3;
    sp3.reset();
    sp2.reset();
    EXPECT_FALSE(wasCalled);
    sp1.reset();
    EXPECT_TRUE(wasCalled);
}

TEST_F(SharedPtrTest, CopyConstructorWithDeleter)
{
    {
        capu::shared_ptr<BaseClass> sp(new BaseClass(123), TestDeleter<BaseClass>(wasCalled));
        capu::shared_ptr<BaseClass> other(sp);
        EXPECT_EQ(2u, sp.use_count());
        EXPECT_EQ(2u, other.use_count());
        EXPECT_EQ(123u, other->mValue);
        EXPECT_EQ(1u, BaseClass::mReferences);
    }
    EXPECT_TRUE(wasCalled);
}

TEST_F(SharedPtrTest, Accessors)
{
    BaseClass *c = new BaseClass(123);
    capu::shared_ptr<BaseClass> sp(c);
    EXPECT_TRUE(c == sp.get());
    EXPECT_TRUE(c == &(*sp));
    EXPECT_EQ(123u, sp->mValue);
}

TEST_F(SharedPtrTest, AssignSameType)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other;
    other = sp;
    EXPECT_EQ(2u, sp.use_count());
    EXPECT_TRUE(sp.get() == other.get());
}

TEST_F(SharedPtrTest, AssignConvertibleType)
{
    capu::shared_ptr<DerivedClass> sp(new DerivedClass(123));
    capu::shared_ptr<BaseClass> other;
    other = sp;
    EXPECT_EQ(2u, sp.use_count());
    EXPECT_TRUE(sp.get() == other.get());
}

TEST_F(SharedPtrTest, AssignSelf)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    sp = sp;
    EXPECT_EQ(1u, sp.use_count());
}

TEST_F(SharedPtrTest, AssignSamePointer)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other(sp);
    EXPECT_EQ(2u, sp.use_count());
    sp = other;
    EXPECT_EQ(2u, sp.use_count());
}

TEST_F(SharedPtrTest, AssignEmptySharedPtr)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other;
    EXPECT_EQ(1u, sp.use_count());
    sp = other;
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_EQ(0u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, ResetToNull)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    sp.reset();
    EXPECT_TRUE(sp.get() == NULL);
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_EQ(0u, BaseClass::mReferences);
    sp.reset();
    EXPECT_EQ(0u, sp.use_count());
}

TEST_F(SharedPtrTest, ResetToNullShared)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other(sp);
    EXPECT_EQ(2u, sp.use_count());
    sp.reset();
    EXPECT_TRUE(sp.get() == NULL);
    EXPECT_EQ(0u, sp.use_count());
    EXPECT_EQ(1u, other.use_count());
    EXPECT_EQ(1u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, ResetEmptyToPointer)
{
    capu::shared_ptr<BaseClass> sp;
    sp.reset(new BaseClass(123));
    EXPECT_TRUE(sp.get() != NULL);
    EXPECT_EQ(1u, sp.use_count());
    EXPECT_EQ(123u, sp->mValue);
}

TEST_F(SharedPtrTest, ResetToPointer)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(456));
    sp.reset(new BaseClass(123));
    EXPECT_TRUE(sp.get() != NULL);
    EXPECT_EQ(1u, sp.use_count());
    EXPECT_EQ(123u, sp->mValue);
    EXPECT_EQ(1u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, ResetToPointerDeleterBefore)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(456), TestDeleter<BaseClass>(wasCalled));
    sp.reset(new BaseClass(123));
    EXPECT_TRUE(wasCalled);
}

TEST_F(SharedPtrTest, ResetToPointerDeleterOnNew)
{
    {
        capu::shared_ptr<BaseClass> sp(new BaseClass(456));
        sp.reset(new BaseClass(123), TestDeleter<BaseClass>(wasCalled));
        EXPECT_FALSE(wasCalled);
    }
    EXPECT_TRUE(wasCalled);
    EXPECT_EQ(0u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, ResetToConvertiblePointer)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(456));
    sp.reset(new DerivedClass(123));
    EXPECT_EQ(123u, sp->mValue);
}

TEST_F(SharedPtrTest, ResetToConvertiblePointerDeleterOnNew)
{
    {
        capu::shared_ptr<BaseClass> sp(new BaseClass(456));
        sp.reset(new DerivedClass(123), TestDeleter<DerivedClass>(wasCalled));
        EXPECT_FALSE(wasCalled);
    }
    EXPECT_TRUE(wasCalled);
    EXPECT_EQ(0u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, Swap)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other(new BaseClass(456));
    sp.swap(other);
    EXPECT_EQ(123u, other->mValue);
    EXPECT_EQ(456u, sp->mValue);
    EXPECT_EQ(1u, sp.use_count());
    EXPECT_EQ(1u, other.use_count());
    EXPECT_EQ(2u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, SwapWithEmpty)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other;
    sp.swap(other);
    EXPECT_TRUE(sp.get() == NULL);
    EXPECT_EQ(123u, other->mValue);
    EXPECT_EQ(1u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, SwapFreeFunction)
{
    capu::shared_ptr<BaseClass> sp(new BaseClass(123));
    capu::shared_ptr<BaseClass> other(new BaseClass(456));
    using std::swap;
    swap(sp, other);
    EXPECT_EQ(123u, other->mValue);
    EXPECT_EQ(456u, sp->mValue);
    EXPECT_EQ(2u, BaseClass::mReferences);
}

TEST_F(SharedPtrTest, OperatorBool)
{
    capu::shared_ptr<BaseClass> sp;
    EXPECT_FALSE(sp);
    sp.reset(new BaseClass(123));
    EXPECT_TRUE(sp);
    sp.reset();
    EXPECT_FALSE(sp);
}

TEST_F(SharedPtrTest, EqualUnequalOperators)
{
    capu::shared_ptr<DerivedClass> sp(new DerivedClass(123));
    capu::shared_ptr<DerivedClass> sameAsSp(sp);
    capu::shared_ptr<BaseClass> sameAsSpButOtherType(sp);
    capu::shared_ptr<DerivedClass> different(new DerivedClass(123));
    capu::shared_ptr<BaseClass> differentOtherType(new BaseClass(123));

    EXPECT_TRUE(sp == sp);
    EXPECT_TRUE(sp == sameAsSp);
    EXPECT_TRUE(sp == sameAsSpButOtherType);
    EXPECT_FALSE(sp == different);
    EXPECT_FALSE(sp == differentOtherType);

    EXPECT_FALSE(sp != sp);
    EXPECT_FALSE(sp != sameAsSp);
    EXPECT_FALSE(sp != sameAsSpButOtherType);
    EXPECT_TRUE(sp != different);
    EXPECT_TRUE(sp != differentOtherType);
}
