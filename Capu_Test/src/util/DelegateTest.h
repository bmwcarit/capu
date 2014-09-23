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

#ifndef CAPU_DELEGATE_TEST_H
#define CAPU_DELEGATE_TEST_H

#include "gmock/gmock.h"
#include "capu/util/Delegate.h"

namespace capu
{
    class DelegateTest : public testing::Test
    {
    public:
        DelegateTest();
    protected:
    };
}

#endif // CAPU_DELEGATE_TEST_H
