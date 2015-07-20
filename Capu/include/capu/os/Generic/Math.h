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
            static const double_t PI_d;

            static const float LN2_f;
            static const double_t LN2_d;

            static float  Ceil(float val);
            static double_t Ceil(double_t val);
            static float  Floor(float val);
            static double_t Floor(double_t val);
            static float  Abs(float val);
            static double_t Abs(double_t val);
            static int_t    Abs(int_t val);
            static float  Sqrt(float val);
            static double_t Sqrt(double_t val);
            static float  Pow2(float val);
            static double_t Pow2(double_t val);
            static float  Pow(float val, float exponent);
            static double_t Pow(double_t val, double_t exponent);
            static float  Cos(float val);
            static double_t Cos(double_t val);
            static float  Sin(float val);
            static double_t Sin(double_t val);
            static float  Tan(float val);
            static double_t Tan(double_t val);
            static float  ArcCos(float val);
            static double_t ArcCos(double_t val);
            static float  ArcSin(float val);
            static double_t ArcSin(double_t val);
            static float  ArcTan(float val);
            static double_t ArcTan(double_t val);
            static float  Rad2Deg(float val);
            static double_t Rad2Deg(double_t val);
            static float  Deg2Rad(float val);
            static double_t Deg2Rad(double_t val);
            static float  Log2(float val);
            static double_t Log2(double_t val);
            static float  Exp(float val);
            static double_t Exp(double_t val);
        };

        inline
        float
        Math::Ceil(float val)
        {
            return ceilf(val);
        }

        inline
        double_t
        Math::Ceil(double_t val)
        {
            return std::ceil(val);
        }

        inline
        float
        Math::Floor(float val)
        {
            return floorf(val);
        }

        inline
        double_t
        Math::Floor(double_t val)
        {
            return std::floor(val);
        }

        inline
        float
        Math::Abs(float val)
        {
            return std::fabs(val);
        }

        inline
        double_t
        Math::Abs(double_t val)
        {
            return std::abs(val);
        }

        inline
        int_t
        Math::Abs(int_t val)
        {
            return std::abs(val);
        }

        inline
        float
        Math::Sqrt(float val)
        {
            return sqrtf(val);
        }

        inline
        double_t
        Math::Sqrt(double_t val)
        {
            return std::sqrt(val);
        }

        inline
        float
        Math::Pow2(float val)
        {
            return val * val;
        }

        inline
        double_t
        Math::Pow2(double_t val)
        {
            return val * val;
        }

        inline
        float
        Math::Cos(float val)
        {
            return cosf(val);
        }

        inline
        float
        Math::Pow(float val, float exponent)
        {
            return powf(val, exponent);
        }

        inline
        double_t
        Math::Pow(double_t val, double_t exponent)
        {
            return std::pow(val, exponent);
        }

        inline
        double_t
        Math::Cos(double_t val)
        {
            return std::cos(val);
        }

        inline
        float
        Math::Sin(float val)
        {
            return sinf(val);
        }

        inline
        double_t
        Math::Sin(double_t val)
        {
            return std::sin(val);
        }

        inline
        float
        Math::Tan(float val)
        {
            return tanf(val);
        }

        inline
        double_t
        Math::Tan(double_t val)
        {
            return std::tan(val);
        }

        inline
        float
        Math::ArcCos(float val)
        {
            return acosf(val);
        }

        inline
        double_t
        Math::ArcCos(double_t val)
        {
            return std::acos(val);
        }

        inline
        float
        Math::ArcSin(float val)
        {
            return asinf(val);
        }

        inline
        double_t
        Math::ArcSin(double_t val)
        {
            return std::asin(val);
        }

        inline
        float
        Math::ArcTan(float val)
        {
            return atanf(val);
        }

        inline
        double_t
        Math::ArcTan(double_t val)
        {
            return std::atan(val);
        }

        inline
        float
		Math::Rad2Deg(float val)
        {
            return val * (180.f / PI_f);
        }

        inline
        double_t
        Math::Rad2Deg(double_t val)
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
        double_t
        Math::Deg2Rad(double_t val)
        {
            return val * (PI_d / 180.0);
        }

        inline
        float
        Math::Log2(float val)
        {
            return logf(val) / Math::LN2_f;

        }

        inline
        double_t
        Math::Log2(double_t val)
        {
            return log(val) / Math::LN2_d;
        }

        inline
            float
            Math::Exp(float val)
        {
            return expf(val);

        }

        inline
            double_t
            Math::Exp(double_t val)
        {
            return std::exp(val);
        }
    }
}
#endif //CAPU_GENERIC_MATH_H
