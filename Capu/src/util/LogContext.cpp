#include "capu/util/LogContext.h"

namespace capu
{
    LogContext::LogContext(const String& name, const String& id)
        : m_contextName(name)
        , m_contextId(id)
        , m_enabled(true)
        , m_logLevel(static_cast<int32_t>(LL_ERROR))
        , m_data(0)
    {
    }
}
