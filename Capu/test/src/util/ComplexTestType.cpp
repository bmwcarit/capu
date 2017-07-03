/*
* Copyright (C) 2015 BMW Car IT GmbH
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

#include "ComplexTestType.h"

capu::uint_t ComplexTestType::ctor_count = 0u;
capu::uint_t ComplexTestType::copyctor_count = 0u;
capu::uint_t ComplexTestType::dtor_count = 0u;

capu::uint_t MoveableComplexTestType::ctor_count = 0u;
capu::uint_t MoveableComplexTestType::copyctor_count = 0u;
capu::uint_t MoveableComplexTestType::movector_count = 0u;
capu::uint_t MoveableComplexTestType::dtor_count = 0u;
