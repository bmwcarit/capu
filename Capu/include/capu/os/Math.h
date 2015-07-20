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

#ifndef CAPU_MATH_H
#define CAPU_MATH_H

#include "capu/Config.h"
#include <capu/os/PlatformInclude.h>

#include CAPU_PLATFORM_INCLUDE(Math)


namespace capu
{
    /**
     * Mathematical operations and constants.
     */
    class Math: private os::arch::Math
    {

    public:
        using os::arch::Math::PI_f;
        using os::arch::Math::PI_d;
        using os::arch::Math::LN2_f;
        using os::arch::Math::LN2_d;

        /**
         * Ceiling function, round given number up to full integer
         */
        static float Ceil(float val);

        /**
         * Ceiling function, round given number up to full integer
         */
        static double_t Ceil(double_t val);

        /**
         * Floor function, round given number down to full integer
         */
        static float Floor(float val);

        /**
         * Floor function, round given number down to full integer
         */
        static double_t Floor(double_t val);

        /**
         * Absolute value of given number
         */
        static float Abs(float val);

        /**
         * Absolute value of given number
         */
        static double_t Abs(double_t val);

        /**
         * Absolute value of given number
         */
        static int_t Abs(int_t val);

        /**
         * Square root of given number
         */
        static float Sqrt(float val);

        /**
         * Square root of given number
         */
        static double_t Sqrt(double_t val);

        /**
         * Given number raised to the power 2 (val²)
         */
        static float Pow2(float val);

        /**
         * Given number raised to the power 2 (val²)
         */
        static double_t Pow2(double_t val);

        /**
         * Given value raised to the power exponent
         */
        static float Pow(float val, float exponent);

        /**
         * Given value raised to the power exponent
         */
        static double_t Pow(double_t val, double_t exponent);

        /**
         * Cosine value of given value
         */
        static float Cos(float val);

        /**
         * Cosine value of given value
         */
        static double_t Cos(double_t val);

        /**
         * Sine value of given value
         */
        static float Sin(float val);

        /**
         * Sine value of given value
         */
        static double_t Sin(double_t val);

        /**
         * Tangent value of given radians
         */
        static float Tan(float val);

        /**
         * Tangent value of given radians
         */
        static double_t Tan(double_t val);

        /**
         * ArcCosine value
         */
        static float ArcCos(float val);

        /**
         * ArcCosine value
         */
        static double_t ArcCos(double_t val);

        /**
         * ArcSine value
         */
        static float ArcSin(float val);

        /**
         * ArcSine value
         */
        static double_t ArcSin(double_t val);

        /**
         * ArcTangent value
         */
        static float ArcTan(float val);

        /**
         * ArcTangent value
         */
        static double_t ArcTan(double_t val);

        /**
         * Convert given value in radians to degrees
         */
        static float Rad2Deg(float val);

        /**
         * Convert given value in radians to degrees
         */
        static double_t Rad2Deg(double_t val);

        /**
         * Convert given value in radians to degrees
         */
        static float Deg2Rad(float val);

        /**
         * Convert given value in radians to degrees
         */
        static double_t Deg2Rad(double_t val);

        /**
         * Logrithm of val to the base 2
         */
        static float Log2(float val);

        /**
         * Logrithm of val to the base 2
         */
        static double_t Log2(double_t val);

        /**
         * Exponential function (e)
         */
        static float Exp(float val);

        /**
         * Exponential function (e)
         */
        static double_t Exp(double_t val);
    };

    inline
    float
    Math::Ceil(float val)
    {
        return os::arch::Math::Ceil(val);
    }

    inline
    double_t
    Math::Ceil(double_t val)
    {
        return os::arch::Math::Ceil(val);
    }

    inline
    float
    Math::Floor(float val)
    {
        return os::arch::Math::Floor(val);
    }

    inline
    double_t
    Math::Floor(double_t val)
    {
        return os::arch::Math::Floor(val);
    }

    inline
    float
    Math::Abs(float val)
    {
        return os::arch::Math::Abs(val);
    }

    inline
    double_t
    Math::Abs(double_t val)
    {
        return os::arch::Math::Abs(val);
    }

    inline
    int_t
    Math::Abs(int_t val)
    {
        return os::arch::Math::Abs(val);
    }

    inline
    float
    Math::Sqrt(float val)
    {
        return os::arch::Math::Sqrt(val);
    }

    inline
    double_t
    Math::Sqrt(double_t val)
    {
        return os::arch::Math::Sqrt(val);
    }

    inline
    float
    Math::Pow2(float val)
    {
        return os::arch::Math::Pow2(val);
    }

    inline
    double_t
    Math::Pow2(double_t val)
    {
        return os::arch::Math::Pow2(val);
    }

    inline
    float
    Math::Pow(float val, float exponent)
    {
        return os::arch::Math::Pow(val, exponent);
    }

    inline
    double_t
    Math::Pow(double_t val, double_t exponent)
    {
        return os::arch::Math::Pow(val, exponent);
    }

    inline
    float
    Math::Cos(float val)
    {
        return os::arch::Math::Cos(val);
    }

    inline
    double_t
    Math::Cos(double_t val)
    {
        return os::arch::Math::Cos(val);
    }

    inline
    float
    Math::Sin(float val)
    {
        return os::arch::Math::Sin(val);
    }

    inline
    double_t
    Math::Sin(double_t val)
    {
        return os::arch::Math::Sin(val);
    }

    inline
    float
    Math::Tan(float val)
    {
        return os::arch::Math::Tan(val);
    }

    inline
    double_t
    Math::Tan(double_t val)
    {
        return os::arch::Math::Tan(val);
    }

    inline
    float
    Math::ArcCos(float val)
    {
        return os::arch::Math::ArcCos(val);
    }

    inline
    double_t
    Math::ArcCos(double_t val)
    {
        return os::arch::Math::ArcCos(val);
    }

    inline
    float
    Math::ArcSin(float val)
    {
        return os::arch::Math::ArcSin(val);
    }

    inline
    double_t
    Math::ArcSin(double_t val)
    {
        return os::arch::Math::ArcSin(val);
    }

    inline
    float
    Math::ArcTan(float val)
    {
        return os::arch::Math::ArcTan(val);
    }

    inline
    double_t
    Math::ArcTan(double_t val)
    {
        return os::arch::Math::ArcTan(val);
    }

    inline
    float
    Math::Rad2Deg(float val)
    {
        return os::arch::Math::Rad2Deg(val);
    }

    inline
    double_t
    Math::Rad2Deg(double_t val)
    {
        return os::arch::Math::Rad2Deg(val);
    }

    inline
    float
    Math::Deg2Rad(float val)
    {
        return os::arch::Math::Deg2Rad(val);
    }

    inline double_t Math::Deg2Rad(double_t val)
    {
        return os::arch::Math::Deg2Rad(val);
    }

    inline
    float Math::Log2(float val)
    {
        return os::arch::Math::Log2(val);
    }

    inline
    double_t Math::Log2(double_t val)
    {
        return os::arch::Math::Log2(val);
    }

    inline
        float Math::Exp(float val)
    {
        return os::arch::Math::Exp(val);
    }

    inline
        double_t Math::Exp(double_t val)
    {
        return os::arch::Math::Exp(val);
    }
}

#endif //CAPU_MATH_H
