
#include "Logger2Test.h"
#include "capu/util/ConsoleLogSink.h"
#include "capu/util/AsynchronousConsoleLogSink.h"
#include "capu/util/Logger2.h"
#include "capu/container/String.h"
#include "capu/util/LogMessage.h"

namespace capu
{
    Logger2Test::Logger2Test()
    {

    }

    Logger2Test::~Logger2Test()
    {

    }

    ConsoleLogSink consoleSink;
    Logger2& default = Logger2::InitGlobalLogger(consoleSink);
    Logger2 mySpecialLogger = Logger2(consoleSink);
    LogContext& CAPU_CONTEXT = Logger2::CreateContext("capu.Logger2");

    TEST_F(Logger2Test, CreateNewLogger)
    {
        LOG2(CAPU_CONTEXT, LL_DEBUG, "Debug message");
        LOG2(CAPU_CONTEXT, LL_WARN,  "Warn message");
        LOG2(CAPU_CONTEXT, LL_ERROR, "Error message");
        

        Thread::Sleep(200);
    }
}
