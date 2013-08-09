

#include "capu/util/LogLevel.h"

namespace capu
{
    /**
     * Initialize the static members.
     */
    const char* LogLevel::m_logLevelNames[LOG_LEVEL_COUNT] =
    {
        "ALL"  ,
        "TRACE",
        "DEBUG",
        "INFO" ,
        "WARN" ,
        "ERROR",
        "FATAL",
        "OFF"
    };
}