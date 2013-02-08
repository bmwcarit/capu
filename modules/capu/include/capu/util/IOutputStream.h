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

#ifndef CAPU_IOUTPUTSTREAM_H
#define CAPU_IOUTPUTSTREAM_H

#include <capu/Config.h>
#include <capu/container/String.h>
#include <capu/util/Guid.h>

namespace capu
{
    /**
     * Interface for writing data to a stream
     */
    class IOutputStream
    {
    public:

        /**
         * Destructor for IOutputStream
         */
        virtual ~IOutputStream();

        /**
         * Operator for writing float_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const float_t value) = 0;

        /**
         * Operator for writing int32_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const int32_t value) = 0;

        /**
         * Operator for writing uint32_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const uint32_t value) = 0;

        /**
         * Operator for writing String values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const String& value) = 0;

        /**
         * Operator for writing bool_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const bool_t  value) = 0;

        /**
         * Operator for writing char_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const char_t* value) = 0;

        /**
         * Operator for writing uint16_t values to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const uint16_t value) = 0;

        /**
         * Operator for writing a guid value to a stream
         * @param value the value which will be written to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& operator<<(const Guid& value) = 0;

        /**
         * Operator for writing raw data to the stream
         * @param data void pointer to the data which will be written to the stream
         * @param size the size of the data for writing to the stream
         * @return a reference to the IOutputStream for further processing of data
         */
        virtual IOutputStream& write(const void* data, const uint32_t size) = 0;

        /**
         * Method to flush the data in the stream to the underlying layer
         * @return status_t of the OutputStream
         */
        virtual status_t flush() = 0;
    };

    inline
    IOutputStream::~IOutputStream()
    {
    }
}

#endif // CAPU_IOUTPUTSTREAM_H
