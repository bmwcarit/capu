/*
 * Copyright (C) 2014 jambit GmbH
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

#ifndef CAPU_IPHONEOS_ARM_MATH_H
#define CAPU_IPHONEOS_ARM_MATH_H

#include <capu/os/iPhoneOS/Math.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Math: private capu::iphoneos::Math
            {
            public:
                using capu::iphoneos::Math::PI_f;
                using capu::iphoneos::Math::PI_d;
                using capu::iphoneos::Math::LN2_f;
                using capu::iphoneos::Math::LN2_d;
                using capu::iphoneos::Math::Ceil;
                using capu::iphoneos::Math::Floor;
                using capu::iphoneos::Math::Abs;
                using capu::iphoneos::Math::Sqrt;
                using capu::iphoneos::Math::Pow2;
                using capu::iphoneos::Math::Pow;
                using capu::iphoneos::Math::Cos;
                using capu::iphoneos::Math::Sin;
                using capu::iphoneos::Math::Tan;
                using capu::iphoneos::Math::ArcCos;
                using capu::iphoneos::Math::ArcSin;
                using capu::iphoneos::Math::ArcTan;
                using capu::iphoneos::Math::Rad2Deg;
                using capu::iphoneos::Math::Deg2Rad;
                using capu::iphoneos::Math::Log2;
                using capu::iphoneos::Math::Exp;
            };
        }
    }
}

#endif
