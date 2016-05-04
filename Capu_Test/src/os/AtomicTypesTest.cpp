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
#include "capu/os/AtomicTypes.h"
#include "capu/os/Thread.h"
#include "capu/os/Mutex.h"
#include "capu/util/Runnable.h"

TEST(AtomicTypeBool, DefaultConstruction)
{
    capu::AtomicBool defaultValue;
    EXPECT_FALSE(defaultValue);
}

TEST(AtomicTypeBool, ValueConstruction)
{
    capu::AtomicBool createdFalse(false);
    capu::AtomicBool createdTrue(true);

    EXPECT_FALSE(createdFalse);
    EXPECT_TRUE(createdTrue);
}

TEST(AtomicTypeBool, CanLoadValue)
{
    capu::AtomicBool createdFalse(false);
    capu::AtomicBool createdTrue(true);

    bool valueFalse = createdFalse.load();
    bool valueTrue  = createdTrue.load();

    EXPECT_FALSE(valueFalse);
    EXPECT_TRUE(valueTrue);
}

TEST(AtomicTypeBool, CanRetrieveValue)
{
    capu::AtomicBool createdFalse(false);
    capu::AtomicBool createdTrue(true);

    bool valueFalse = createdFalse;
    bool valueTrue  = createdTrue;

    EXPECT_FALSE(valueFalse);
    EXPECT_TRUE(valueTrue);
}

TEST(AtomicTypeBool, CanStoreValue)
{
    capu::AtomicBool createdFalse(false);
    capu::AtomicBool createdTrue(true);

    createdFalse.store(true);
    createdTrue.store(false);

    EXPECT_FALSE(createdTrue);
    EXPECT_TRUE(createdFalse);
}

TEST(AtomicTypeBool, CanAssignValue)
{
    capu::AtomicBool createdFalse(false);
    capu::AtomicBool createdTrue(true);

    createdFalse = true;
    createdTrue  = false;

    EXPECT_FALSE(createdTrue);
    EXPECT_TRUE(createdFalse);
}

