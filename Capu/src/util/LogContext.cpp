#include "capu/util/LogContext.h"

namespace capu
{
    LogContext::LogContext(const String& name, const String& id)
        : m_contextName(name)
        , m_contextId(id)
        , m_enabled(true)
        , m_logLevel(LL_ERROR)
        , m_data(0)
    {

    }
}
