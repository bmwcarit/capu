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

#include <gmock/gmock.h>
#include "capu/Config.h"
#include "capu/util/ScopedPointer.h"

class Resource
{
public:

    Resource()
        : mData(0)
    {
        ++refCount;
    }

    void setData(capu::int32_t data)
    {
        mData = data;
    }

    ~Resource()
    {
        --refCount;
    }

    static capu::int32_t refCount;

    capu::int32_t mData;
};

struct CustomDeleter
{
    static void performDelete(capu::int32_t* ptr)
    {
        delete ptr;
        ++count;
    }

    static capu::uint32_t count;
};

capu::uint32_t CustomDeleter::count = 0;

capu::int32_t Resource::refCount = 0;

TEST(ScopedArrayTest, NoResource)
{
    Resource::refCount = 0;

    {
        capu::ScopedArray<Resource> handler(0);
        EXPECT_EQ(0, Resource::refCount);
        EXPECT_EQ(0, handler.getRawData());
    }

    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedArrayTest, SingleResource)
{
    Resource::refCount = 0;

    {
        capu::ScopedArray<Resource> handler(1);
        EXPECT_EQ(1, Resource::refCount);
    }

    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedArrayTest, MultipleResources)
{
    Resource::refCount = 0;
    for (capu::int32_t i = 0; i < 100; ++i)
    {
        {
            capu::ScopedArray<Resource> handler(i);
            EXPECT_EQ(i, Resource::refCount);
        }
        EXPECT_EQ(0, Resource::refCount);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedArrayTest, AccessOperator)
{
    Resource::refCount = 0;
    for (capu::int32_t i = 0; i < 100; ++i)
    {
        {
            capu::ScopedArray<Resource> handler(i);
            EXPECT_EQ(i, Resource::refCount);

            for (capu::int32_t j = 0; j < i; ++j)
            {
                handler[j].setData(j);
            }
            for (capu::int32_t j = 0; j < i; ++j)
            {
                EXPECT_EQ(j, handler[j].mData);
            }
        }
        EXPECT_EQ(0, Resource::refCount);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedArrayTest, Swap)
{
    Resource::refCount = 0;
    {
        capu::ScopedArray<Resource> handler1(50);
        EXPECT_EQ(50, Resource::refCount);

        Resource* firstOfHandler1 = handler1.getRawData();
        {
            capu::ScopedArray<Resource> handler2(100);
            EXPECT_EQ(150, Resource::refCount);

            Resource* firstOfHandler2 = handler2.getRawData();

            capu::swap(handler1, handler2);
            EXPECT_EQ(firstOfHandler1, handler2.getRawData());
            EXPECT_EQ(firstOfHandler2, handler1.getRawData());

            handler1.swap(handler2);
            EXPECT_EQ(firstOfHandler1, handler1.getRawData());
            EXPECT_EQ(firstOfHandler2, handler2.getRawData());

            // another swap for checking resource count
            capu::swap(handler1, handler2);
        }
        EXPECT_EQ(100, Resource::refCount);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, NoResource)
{
    Resource::refCount = 0;

    capu::ScopedPointer<Resource> handler(NULL);
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, WithResource)
{
    Resource::refCount = 0;

    {
        capu::ScopedPointer<Resource> handler(new Resource());
        EXPECT_EQ(1, Resource::refCount);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, DereferencingOperator)
{
    Resource::refCount = 0;

    {
        Resource* resource = new Resource();
        resource->mData = 42;
        capu::ScopedPointer<Resource> handler(resource);

        Resource* dereferencedResource = &(*handler);
        Resource* rawData = handler.getRawData();

        EXPECT_EQ(resource, dereferencedResource);
        EXPECT_EQ(resource, rawData);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, DereferencingAccessOperator)
{
    Resource::refCount = 0;

    {
        Resource* resource = new Resource();
        capu::ScopedPointer<Resource> handler(resource);

        handler->mData = 42;
        EXPECT_EQ(resource->mData, handler->mData);
        EXPECT_EQ(42, handler->mData);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, Swap)
{
    Resource::refCount = 0;
    {
        Resource* resource1 = new Resource();
        resource1->mData = 42;
        capu::ScopedPointer<Resource> handler1(resource1);

        {
            Resource* resource2 = new Resource();
            resource2->mData = 5;
            capu::ScopedPointer<Resource> handler2(resource2);

            Resource* resourceOfHandler1 = handler1.getRawData();
            Resource* resourceOfHandler2 = handler2.getRawData();
            EXPECT_EQ(resource1, resourceOfHandler1);
            EXPECT_EQ(resource2, resourceOfHandler2);

            capu::swap(handler1, handler2);
            EXPECT_EQ(resource1, handler2.getRawData());
            EXPECT_EQ(resource2, handler1.getRawData());
            EXPECT_EQ(resource1->mData, handler2->mData);
            EXPECT_EQ(resource2->mData, handler1->mData);
            EXPECT_EQ(5, handler1->mData);
            EXPECT_EQ(42, handler2->mData);

            handler1.swap(handler2);
            EXPECT_EQ(resource1, handler1.getRawData());
            EXPECT_EQ(resource2, handler2.getRawData());
            EXPECT_EQ(resource1->mData, handler1->mData);
            EXPECT_EQ(resource2->mData, handler2->mData);
            EXPECT_EQ(42, handler1->mData);
            EXPECT_EQ(5, handler2->mData);
        }
        EXPECT_EQ(1, Resource::refCount);
    }
    EXPECT_EQ(0, Resource::refCount);
}

TEST(ScopedPointerTest, CustomDeleter)
{
    CustomDeleter::count = 0;
    {
        capu::ScopedPointer<capu::int32_t, CustomDeleter> scopedPointer(new capu::int32_t(42));
        EXPECT_EQ(42, *scopedPointer);
    }
    EXPECT_EQ(1u, CustomDeleter::count);
}

TEST(ScopedPointerTest, DefaultConstructorValue)
{
    capu::ScopedPointer<Resource> ptr; // will automatically create a resource object
    capu::int32_t val = ptr->mData;
    EXPECT_EQ(0, val);
}
