/*
 * Copyright 2013 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAPU_BINARYFILEINPUTSTREAM_TEST_H
#define CAPU_BINARYFILEINPUTSTREAM_TEST_H

#include "gmock/gmock.h"
#include "capu/os/File.h"

namespace capu
{
    class BinaryFileInputStreamTest : public testing::Test
    {
    public:
        BinaryFileInputStreamTest();
        ~BinaryFileInputStreamTest();
        void SetUp();
        void TearDown();
    protected:
        capu::File mFile;

    };
}

#endif // __capu_FileOutputStream_Test_h__
