#include "capu/util/LogContext.h"

namespace capu
{
    LogContext::LogContext(const String& name)
        : m_contextName(name)
        , m_enabled(true)
        , m_logLevel(LL_ERROR)
    {

    }
}
