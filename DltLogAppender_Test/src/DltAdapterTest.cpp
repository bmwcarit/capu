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

#include <gtest/gtest.h>
#include "DltLogAppender/DltAdapter.h"
#include "capu/os/Memory.h"
#include "capu/container/String.h"

TEST(DltAdapter, SingletonNotNull)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();
    EXPECT_NE(dltAdapter,(capu::DltAdapter*)0);
}

TEST(DltAdapter, DltDaemonPresent)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();
    bool dltInitialized = dltAdapter->isDltInitialized();
    EXPECT_TRUE(dltInitialized);
}

TEST(DltAdapter, RegisterAndUnregisterApplication)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();
    bool dltInitialized = dltAdapter->isDltInitialized();
    EXPECT_TRUE(dltInitialized);

    dltAdapter->registerApplication("TEST","Test application");

    EXPECT_STREQ(dltAdapter->getApplicationName().c_str(),"TEST");
    EXPECT_STREQ(dltAdapter->getApplicationDescription().c_str(),"Test application");
    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);

    dltAdapter->unregisterApplication();

    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);
    EXPECT_STREQ(dltAdapter->getApplicationName().c_str(),"");
    EXPECT_STREQ(dltAdapter->getApplicationDescription().c_str(),"");
}

TEST(DltAdapter, RegisterApplicationWithAppNameTooLarge)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();
    bool dltInitialized = dltAdapter->isDltInitialized();
    EXPECT_TRUE(dltInitialized);

    dltAdapter->registerApplication("TESTABC","Test application");

    EXPECT_STREQ(dltAdapter->getApplicationName().c_str(),"TEST");
    EXPECT_STREQ(dltAdapter->getApplicationDescription().c_str(),"Test application");
    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);

    dltAdapter->unregisterApplication();
}

TEST(DltAdapter, RegisterandUnregisterContext)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();
    bool dltInitialized = dltAdapter->isDltInitialized();
    EXPECT_TRUE(dltInitialized);

    dltAdapter->registerApplication("TEST","Test application");
    capu::LogContext con("Test context");

    dltAdapter->registerContext(&con,"TCTX");
    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);

    dltAdapter->unregisterContext(&con);

    dltAdapter->unregisterApplication();

    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);
}

TEST(DltAdapter, LogMessageInDltAdapter)
{
    capu::DltAdapter* dltAdapter = capu::DltAdapter::getDltAdapter();

    bool dltInitialized = dltAdapter->isDltInitialized();
    EXPECT_TRUE(dltInitialized);

    dltAdapter->registerApplication("TEST","Test application");

    capu::LogContext con("Test context");

    dltAdapter->registerContext(&con,"TCTX");

    capu::LogMessage msg(con,capu::LL_INFO);

    (msg.getStream() << "sample message").flush();

    dltAdapter->logMessage(msg);
    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);

    dltAdapter->unregisterContext(&con);
    dltAdapter->unregisterApplication();
    EXPECT_EQ(dltAdapter->getDltStatus(),capu::DLT_ERROR_NO_ERROR);
}