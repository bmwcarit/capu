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

namespace capu
{
    namespace generic
    {
        class Math
        {
        public:
            static const float_t PI_f;
            static const double_t PI_d;

            static const float_t LN2_f;
            static const double_t LN2_d;

            static float_t  Ceil(float_t val);
            static double_t Ceil(double_t val);
            static float_t  Floor(float_t val);
            static double_t Floor(double_t val);
            static float_t  Abs(float_t val);
            static double_t Abs(double_t val);
            static int_t    Abs(int_t val);
            static float_t  Sqrt(float_t val);
            static double_t Sqrt(double_t val);
            static float_t  Pow2(float_t val);
            static double_t Pow2(double_t val);
            static float_t  Pow(float_t val, float_t exponent);
            static double_t Pow(double_t val, double_t exponent);
            static float_t  Cos(float_t val);
            static double_t Cos(double_t val);
            static float_t  Sin(float_t val);
            static double_t Sin(double_t val);
            static float_t  Tan(float_t val);
            static double_t Tan(double_t val);
            static float_t  ArcCos(float_t val);
            static double_t ArcCos(double_t val);
            static float_t  ArcSin(float_t val);
            static double_t ArcSin(double_t val);
            static float_t  ArcTan(float_t val);
            static double_t ArcTan(double_t val);
            static float_t  Rad2Deg(float_t val);
            static double_t Rad2Deg(double_t val);
            static float_t  Deg2Rad(float_t val);
            static double_t Deg2Rad(double_t val);
            static float_t  Log2(float_t val);
            static double_t Log2(double_t val);
        };

        inline
        float_t
        Math::Ceil(float_t val)
        {
            return ceilf(val);
        }

        inline
        double_t
        Math::Ceil(double_t val)
        {
            return ceil(val);
        }

        inline
        float_t
        Floor(float_t val)
        {
            return floorf(val);
        }

        inline
        double_t
        Floor(double_t val)
        {
            return floor(val);
        }

        inline
        float_t
        Math::Abs(float_t val)
        {
            return fabs(val);
        }

        inline
        double_t
        Math::Abs(double_t val)
        {
            return abs(val);
        }

        inline
        int_t
        Math::Abs(int_t val)
        {
            return abs(val);
        }

        inline
        float_t
        Math::Sqrt(float_t val)
        {
            return sqrtf(val);
        }

        inline
        double_t
        Math::Sqrt(double_t val)
        {
            return sqrt(val);
        }

        inline
        float_t
        Math::Pow2(float_t val)
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
        float_t
        Math::Cos(float_t val)
        {
            return cosf(val);
        }

        inline
        float_t
        Pow(float_t val, float_t exponent)
        {
            return powf(val, exponent);
        }

        inline
        double_t
        Pow(double_t val, double_t exponent)
        {
            return pow(val, exponent);
        }

        inline
        double_t
        Math::Cos(double_t val)
        {
            return cos(val);
        }

        inline
        float_t
        Math::Sin(float_t val)
        {
            return sinf(val);
        }

        inline
        double_t
        Math::Sin(double_t val)
        {
            return sin(val);
        }

        inline
        float_t
        Math::Tan(float_t val)
        {
            return tanf(val);
        }

        inline
        double_t
        Math::Tan(double_t val)
        {
            return tan(val);
        }

        inline
        float_t
        Math::ArcCos(float_t val)
        {
            return acosf(val);
        }

        inline
        double_t
        Math::ArcCos(double_t val)
        {
            return acos(val);
        }

        inline
        float_t
        Math::ArcSin(float_t val)
        {
            return asinf(val);
        }

        inline
        double_t
        Math::ArcSin(double_t val)
        {
            return asin(val);
        }

        inline
        float_t
        Math::ArcTan(float_t val)
        {
            return atanf(val);
        }

        inline
        double_t
        Math::ArcTan(double_t val)
        {
            return atan(val);
        }

        inline
        float_t Math::Rad2Deg(float_t val)
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
        float_t
        Math::Deg2Rad(float_t val)
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
        float_t
        Math::Log2(float_t val)
        {
            return logf(val) / Math::LN2_f;

        }

        inline
        double_t
        Math::Log2(double_t val)
        {
            return log(val) / Math::LN2_d;
        }
    }
}
#endif //CAPU_GENERIC_MATH_H
