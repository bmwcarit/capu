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

#ifndef CAPU_GENERIC_MATH_H
#define CAPU_GENERIC_MATH_H


#include <cmath>
#include <cstdlib>

namespace capu
{
    namespace generic
    {
        class Math
        {
        public:
            static const float PI_f;
            static const double PI_d;

            static const float LN2_f;
            static const double LN2_d;

            static float  Ceil(float val);
            static double Ceil(double val);
            static float  Floor(float val);
            static double Floor(double val);
            static float  Abs(float val);
            static double Abs(double val);
            static int_t    Abs(int_t val);
            static float  Sqrt(float val);
            static double Sqrt(double val);
            static float  Pow2(float val);
            static double Pow2(double val);
            static float  Pow(float val, float exponent);
            static double Pow(double val, double exponent);
            static float  Cos(float val);
            static double Cos(double val);
            static float  Sin(float val);
            static double Sin(double val);
            static float  Tan(float val);
            static double Tan(double val);
            static float  ArcCos(float val);
            static double ArcCos(double val);
            static float  ArcSin(float val);
            static double ArcSin(double val);
            static float  ArcTan(float val);
            static double ArcTan(double val);
            static float  Rad2Deg(float val);
            static double Rad2Deg(double val);
            static float  Deg2Rad(float val);
            static double Deg2Rad(double val);
            static float  Log2(float val);
            static double Log2(double val);
            static float  Exp(float val);
            static double Exp(double val);
        };

        inline
        float
        Math::Ceil(float val)
        {
            return ::std::ceil(val);
        }

        inline
        double
        Math::Ceil(double val)
        {
            return ::std::ceil(val);
        }

        inline
        float
        Math::Floor(float val)
        {
            return ::std::floor(val);
        }

        inline
        double
        Math::Floor(double val)
        {
            return ::std::floor(val);
        }

        inline
        float
        Math::Abs(float val)
        {
            return ::std::fabs(val);
        }

        inline
        double
        Math::Abs(double val)
        {
            return ::std::fabs(val);
        }

        inline
        int_t
        Math::Abs(int_t val)
        {
            return ::std::abs(val);
        }

        inline
        float
        Math::Sqrt(float val)
        {
            return ::std::sqrt(val);
        }

        inline
        double
        Math::Sqrt(double val)
        {
            return ::std::sqrt(val);
        }

        inline
        float
        Math::Pow2(float val)
        {
            return val * val;
        }

        inline
        double
        Math::Pow2(double val)
        {
            return val * val;
        }

        inline
        float
        Math::Cos(float val)
        {
            return ::std::cos(val);
        }

        inline
        float
        Math::Pow(float val, float exponent)
        {
            return ::std::pow(val, exponent);
        }

        inline
        double
        Math::Pow(double val, double exponent)
        {
            return ::std::pow(val, exponent);
        }

        inline
        double
        Math::Cos(double val)
        {
            return ::std::cos(val);
        }

        inline
        float
        Math::Sin(float val)
        {
            return ::std::sin(val);
        }

        inline
        double
        Math::Sin(double val)
        {
            return ::std::sin(val);
        }

        inline
        float
        Math::Tan(float val)
        {
            return ::std::tan(val);
        }

        inline
        double
        Math::Tan(double val)
        {
            return ::std::tan(val);
        }

        inline
        float
        Math::ArcCos(float val)
        {
            return ::std::acos(val);
        }

        inline
        double
        Math::ArcCos(double val)
        {
            return ::std::acos(val);
        }

        inline
        float
        Math::ArcSin(float val)
        {
            return ::std::asin(val);
        }

        inline
        double
        Math::ArcSin(double val)
        {
            return ::std::asin(val);
        }

        inline
        float
        Math::ArcTan(float val)
        {
            return ::std::atan(val);
        }

        inline
        double
        Math::ArcTan(double val)
        {
            return ::std::atan(val);
        }

        inline
        float
		Math::Rad2Deg(float val)
        {
            return val * (180.f / PI_f);
        }

        inline
        double
        Math::Rad2Deg(double val)
        {
            return val * (180.0 / PI_d);
        }

        inline
        float
        Math::Deg2Rad(float val)
        {
            return val * (PI_f / 180.f);
        }

        inline
        double
        Math::Deg2Rad(double val)
        {
            return val * (PI_d / 180.0);
        }

        inline
        float
        Math::Log2(float val)
        {
            return ::std::log(val) / LN2_f;

        }

        inline
        double
        Math::Log2(double val)
        {
            return ::std::log(val) / LN2_d;
        }

        inline
        float
        Math::Exp(float val)
        {
            return ::std::exp(val);
        }

        inline
        double
        Math::Exp(double val)
        {
            return ::std::exp(val);
        }
    }
}
#endif //CAPU_GENERIC_MATH_H
