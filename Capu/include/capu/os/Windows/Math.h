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

#ifndef CAPU_WINDOWS_MATH_H
#define CAPU_WINDOWS_MATH_H

#include <capu/os/Generic/Math.h>

namespace capu
{
    namespace os
    {
        class Math: private capu::generic::Math
        {
        public:
            using capu::generic::Math::PI_f;
            using capu::generic::Math::PI_d;
            using capu::generic::Math::LN2_f;
            using capu::generic::Math::LN2_d;
            using capu::generic::Math::Ceil;
            using capu::generic::Math::Floor;
            using capu::generic::Math::Abs;
            using capu::generic::Math::Sqrt;
            using capu::generic::Math::Pow2;
            using capu::generic::Math::Pow;
            using capu::generic::Math::Cos;
            using capu::generic::Math::Sin;
            using capu::generic::Math::Tan;
            using capu::generic::Math::ArcCos;
            using capu::generic::Math::ArcSin;
            using capu::generic::Math::ArcTan;
            using capu::generic::Math::Rad2Deg;
            using capu::generic::Math::Deg2Rad;
            using capu::generic::Math::Log2;
            using capu::generic::Math::Exp;
        };
    }
}

#endif //CAPU_WINDOWS_MATH_H
