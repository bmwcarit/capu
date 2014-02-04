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

#include "gmock/gmock.h"
#include "capu/util/HybridAllocator.h"
#include "capu/container/List.h"

using namespace capu;

TEST(HybridAllocator, TestHybridAllocation)
{
    HybridAllocator<capu::uint32_t, 3> hybridAlloc; // allocator with three static spaces
    capu::uint32_t* int1 = hybridAlloc.allocate(); // on stack
    capu::uint32_t* int2 = hybridAlloc.allocate(); // on stack
    capu::uint32_t* int3 = hybridAlloc.allocate(); // on stack
    capu::uint32_t* int4 = hybridAlloc.allocate(); // on heap
    capu::uint32_t* int5 = hybridAlloc.allocate(); // on heap
    EXPECT_TRUE(int1 != 0);
    EXPECT_TRUE(int2 != 0);
    EXPECT_TRUE(int3 != 0);
    EXPECT_TRUE(int4 != 0);
    EXPECT_TRUE(int5 != 0);
    hybridAlloc.deallocate(int1); // 'stack delete'
    hybridAlloc.deallocate(int2); // 'stack delete'
    hybridAlloc.deallocate(int3); // 'stack delete'
    hybridAlloc.deallocate(int4); // heap delete
    hybridAlloc.deallocate(int5); // heap delete
    EXPECT_TRUE(int1 == 0);
    EXPECT_TRUE(int2 == 0);
    EXPECT_TRUE(int3 == 0);
    EXPECT_TRUE(int4 == 0);
    EXPECT_TRUE(int5 == 0);
    int4 = hybridAlloc.allocate(); // on stack, reusing static memory
    int5 = hybridAlloc.allocate(); // on stack, reusing static memory
    EXPECT_TRUE(int4 != 0);
    EXPECT_TRUE(int5 != 0);
    hybridAlloc.deallocate(int4); // 'stack delete'
    hybridAlloc.deallocate(int5); // 'stack delete'
    EXPECT_TRUE(int4 == 0);
    EXPECT_TRUE(int5 == 0);
}

TEST(HybridAllocator, TestHybridAllocationInList)
{
    List<capu::uint32_t, HybridAllocator<GenericListNode<capu::uint32_t>, 3> > list;
    list.push_front(1); // stack
    list.push_front(2); // stack
    list.push_front(3); // stack
    list.push_front(4); // heap
    list.push_front(5); // heap
    list.clear();
}
