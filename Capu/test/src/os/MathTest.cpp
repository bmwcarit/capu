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
#include <gtest/gtest.h>
#include "capu/os/Math.h"

TEST(Math, abs1)
{
    capu::int_t val = 5;
    EXPECT_EQ(5, capu::Math::Abs(val));
    val = -5;
    EXPECT_EQ(5, capu::Math::Abs(val));
}

TEST(Math, abs2)
{
    double val = 6;
    EXPECT_EQ(6, capu::Math::Abs(val));
    val = -6;
    EXPECT_EQ(6, capu::Math::Abs(val));
}

TEST(Math, abs3)
{
    float val = 7;
    EXPECT_EQ(7, capu::Math::Abs(val));
    val = -7;
    EXPECT_EQ(7, capu::Math::Abs(val));
}

TEST(Math, PI)
{
    float  val = capu::Math::PI_f;
    double val2 = capu::Math::PI_d;
    EXPECT_TRUE(val > 3);
    EXPECT_TRUE(val2 > 3);
}

TEST(Math, Log2)
{
    EXPECT_FLOAT_EQ(3.321928f, capu::Math::Log2(10.f));
    EXPECT_NEAR(3.3219280948873644, capu::Math::Log2(10.0), 0.0000000000001);
}

TEST(Math, Exp)
{
    EXPECT_FLOAT_EQ(200.33685f, capu::Math::Exp(5.3f));
    EXPECT_NEAR(200.33680997479166, capu::Math::Exp(5.3), 0.0000000000001);
}

TEST(Math, Ceil)
{
    EXPECT_FLOAT_EQ(3.0f, capu::Math::Ceil(2.3f));
    EXPECT_FLOAT_EQ(3.0f, capu::Math::Ceil(2.8f));
    EXPECT_FLOAT_EQ(-2.0f, capu::Math::Ceil(-2.8f));
    EXPECT_DOUBLE_EQ(3.0, capu::Math::Ceil(2.3));
    EXPECT_DOUBLE_EQ(3.0, capu::Math::Ceil(2.8));
    EXPECT_DOUBLE_EQ(-2.0, capu::Math::Ceil(-2.8));
}

TEST(Math, Floor)
{
    EXPECT_FLOAT_EQ(2.0f, capu::Math::Floor(2.3f));
    EXPECT_FLOAT_EQ(2.0f, capu::Math::Floor(2.8f));
    EXPECT_FLOAT_EQ(-3.0f, capu::Math::Floor(-2.3f));
    EXPECT_DOUBLE_EQ(2.0, capu::Math::Floor(2.3));
    EXPECT_DOUBLE_EQ(2.0, capu::Math::Floor(2.8));
    EXPECT_DOUBLE_EQ(-3.0, capu::Math::Floor(-2.3));
}

TEST(Math, Sqrt)
{
    EXPECT_FLOAT_EQ(3.0f, capu::Math::Sqrt(9.0f));
    EXPECT_DOUBLE_EQ(3.0, capu::Math::Sqrt(9.0));
}

TEST(Math, Pow)
{
    EXPECT_FLOAT_EQ(4.0f, capu::Math::Pow2(2.0f));
    EXPECT_DOUBLE_EQ(4.0, capu::Math::Pow2(2.0));

    EXPECT_FLOAT_EQ(8.0f, capu::Math::Pow(2.0f, 3.0f));
    EXPECT_DOUBLE_EQ(8.0, capu::Math::Pow(2.0, 3.0));

    EXPECT_FLOAT_EQ(capu::Math::Pow(4.0f, 2.0f), capu::Math::Pow2(4.0f));
    EXPECT_DOUBLE_EQ(capu::Math::Pow(4.0, 2.0), capu::Math::Pow2(4.0));
}

TEST(Math, Cos)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(-1.0f, capu::Math::Cos(pif));
    EXPECT_DOUBLE_EQ(-1.0, capu::Math::Cos(pid));
}

TEST(Math, Sin)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(1.0f, capu::Math::Sin(pif/2.0f));
    EXPECT_DOUBLE_EQ(1.0, capu::Math::Sin(pid/2.0));
}

TEST(Math, Tan)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(1.0f, capu::Math::Tan(pif/4.0f));
    EXPECT_DOUBLE_EQ(1.0, capu::Math::Tan(pid/4.0));
}

TEST(Math, ArcCos)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(pif, capu::Math::ArcCos(-1.0f));
    EXPECT_DOUBLE_EQ(pid, capu::Math::ArcCos(-1.0));
}

TEST(Math, ArcSin)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(pif/2.0f, capu::Math::ArcSin(1.0f));
    EXPECT_DOUBLE_EQ(pid/2.0, capu::Math::ArcSin(1.0));
}

TEST(Math, ArcTan)
{
    double pid = capu::Math::PI_d;
    float pif = capu::Math::PI_f;

    EXPECT_FLOAT_EQ(pif/4.0f, capu::Math::ArcTan(1.0f));
    EXPECT_DOUBLE_EQ(pid/4.0, capu::Math::ArcTan(1.0));
}

TEST(Math, Rad2Deg)
{
    EXPECT_FLOAT_EQ(180.0f, capu::Math::Rad2Deg(capu::Math::PI_f));
    EXPECT_DOUBLE_EQ(180.0, capu::Math::Rad2Deg(capu::Math::PI_d));
}

TEST(Math, Deg2Rad)
{
    EXPECT_FLOAT_EQ(capu::Math::PI_f, capu::Math::Deg2Rad(180.0f));
    EXPECT_DOUBLE_EQ(capu::Math::PI_d, capu::Math::Deg2Rad(180.0));
}