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

#ifndef CAPU_UDPSOCKETOUTPUTSTREAMTEST_H
#define CAPU_UDPSOCKETOUTPUTSTREAMTEST_H

#include "gmock/gmock.h"
#include "capu/util/SocketOutputStream.h"
#include <capu/util/UdpSocketOutputStream.h>

namespace capu
{
    class UdpSocketOutputStreamTest : public testing::Test
    {
    public:
        UdpSocketOutputStreamTest();
        ~UdpSocketOutputStreamTest();
        void SetUp();
        void TearDown();
    protected:
    };
}

#endif // __Utils_SocketOutputStream_Test_H