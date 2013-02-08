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

#ifndef CAPU_LINUX_X86_64_MATH_H
#define CAPU_LINUX_X86_64_MATH_H

#include <capu/os/Linux/Math.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class Math: private os::Math
            {
            public:
                using capu::os::Math::PI_f;
                using capu::os::Math::PI_d;
                using capu::os::Math::LN2_f;
                using capu::os::Math::LN2_d;
                using capu::os::Math::Ceil;
                using capu::os::Math::Floor;
                using capu::os::Math::Abs;
                using capu::os::Math::Sqrt;
                using capu::os::Math::Pow2;
                using capu::os::Math::Pow;
                using capu::os::Math::Cos;
                using capu::os::Math::Sin;
                using capu::os::Math::Tan;
                using capu::os::Math::ArcCos;
                using capu::os::Math::ArcSin;
                using capu::os::Math::ArcTan;
                using capu::os::Math::Rad2Deg;
                using capu::os::Math::Deg2Rad;
                using capu::os::Math::Log2;
            };
        }
    }
}

#endif // CAPU_LINUX_X86_64_MATH_H
