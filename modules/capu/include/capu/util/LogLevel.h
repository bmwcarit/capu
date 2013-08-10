/*
 * Copyright (C) 2013 BMW Car IT GmbH
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


#ifndef CAPU_LOGLEVEL_H
#define CAPU_LOGLEVEL_H


namespace capu
{
   /**
     * Definition of the different logging levels for a capu logger.
     */
    enum ELogLevel
    {
        // Everything will be logged.
        LL_ALL          = 0x0000,
        // All trace message and all higher log levels will be logged.
        LL_TRACE        = 0x1000,
        // All debug message and all higher log levels will be logged.
        LL_DEBUG        = 0x2000,
        // All info message and all higher log levels will be logged.
        LL_INFO         = 0x3000,
        // All warning message and all higher log levels will be logged.
        LL_WARN         = 0x4000,
        // All error message and all higher log levels will be logged.
        LL_ERROR        = 0x5000,
        // Only the fatal message will be logged.
        LL_FATAL        = 0x6000,
        // Logging is turned off, nothing will be logged.
        LL_OFF          = 0x7000
    };

    /**
     * Helper class for logging level related functions.
     */
    class LogLevel
    {
    public:
        /**
         * Get the text representation of the supplied logging level.
         * @param   logLevel    The logging level.
         * @return  The text representation.
         */
        static const char* GetLogLevelText(ELogLevel logLevel);

    private:
        /**
         * Anonymous enumeration to define constants.
         */
        enum
        {
            // The number of bits a log level value should be shifted to the right in order to get a valid index.
            SHIFT_TO_RIGHT_COUNT    = 12,
            // The number of existing log levels. It assumes that LL_OFF is the highest (last) logging level.
            LOG_LEVEL_COUNT         = (LL_OFF >> SHIFT_TO_RIGHT_COUNT) + 1
        };

        /**
         * Table of the different logging level text representations.
         */
        static const char* m_logLevelNames[LOG_LEVEL_COUNT];
    };

    inline const char* LogLevel::GetLogLevelText(ELogLevel logLevel)
    {
        return (m_logLevelNames[logLevel >> 12]);
    }
}

#endif // CAPU_LOGLEVEL_H