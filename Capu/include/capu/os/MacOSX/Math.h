/*
 * Copyright (C) 2014 BMW Car IT GmbH
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

#ifndef CAPU_MACOSX_MATH_H
#define CAPU_MACOSX_MATH_H

#include <capu/os/Posix/Math.h>

namespace capu
{
    namespace os
    {
        class Math: private capu::posix::Math
        {
        public:
            using capu::posix::Math::PI_f;
            using capu::posix::Math::PI_d;
            using capu::posix::Math::LN2_f;
            using capu::posix::Math::LN2_d;
            using capu::posix::Math::Ceil;
            using capu::posix::Math::Floor;
            using capu::posix::Math::Abs;
            using capu::posix::Math::Sqrt;
            using capu::posix::Math::Pow2;
            using capu::posix::Math::Pow;
            using capu::posix::Math::Cos;
            using capu::posix::Math::Sin;
            using capu::posix::Math::Tan;
            using capu::posix::Math::ArcCos;
            using capu::posix::Math::ArcSin;
            using capu::posix::Math::ArcTan;
            using capu::posix::Math::Rad2Deg;
            using capu::posix::Math::Deg2Rad;
            using capu::posix::Math::Log2;
            using capu::posix::Math::Exp;
        };
    }
}

#endif
