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

#ifndef CAPU_IINPUTSTREAM_H
#define CAPU_IINPUTSTREAM_H

#include <capu/Config.h>
#include <capu/util/Guid.h>
#include <capu/container/String.h>

namespace capu
{
    /**
     * An abstract inputstream.
     */
    class IInputStream
    {
    public:

        /**
         * Destructor of input stream
         */
        virtual ~IInputStream();

        /**
         * Reads an int32_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(int32_t& value) = 0;

        /**
         * Reads an uint32_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(uint32_t& value) = 0;

        /**
         * Reads an uint64_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(uint64_t& value) = 0;

        /**
         * Reads an int64_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(int64_t& value) = 0;

        /**
         * Reads a String value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(String&  value) = 0;

        /**
         * Reads an bool value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(bool&  value) = 0;

        /**
         * Reads a float_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(float_t& value) = 0;

        /**
         * Reads a uint16_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(uint16_t& value) = 0;

        /**
         * Reads a guid value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& operator>>(Guid& value) = 0;

        /**
         * Reads raw data from the stream
         * @param data the destination of the read data
         * @param size the size of the data to read
         * @return a reference to the input stream for further processing
         */
        virtual IInputStream& read(char_t* data, const uint32_t size) = 0;

        /**
         * Returns the current state of the stream. This can be used to
         * determine if something went wrong during the last read operation
         * @return the current state of the stream
         */
        virtual status_t getState() const = 0;

    protected:
    private:
    };  

    inline
    IInputStream::~IInputStream()
    {
    }
}

#endif // CAPU_IINPUTSTREAM_H
