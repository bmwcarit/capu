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

#ifndef CAPU_BINARYINPUTSTREAM_H
#define CAPU_BINARYINPUTSTREAM_H

#include <capu/util/IInputStream.h>
#include <capu/Error.h>

namespace capu
{
    /**
     * Reads data from a binary stream
     */
    class BinaryInputStream: public IInputStream
    {
    public:

        /**
         * Constructor with a buffer to read from
         * @param input A pointer to the data to read from
         */
        BinaryInputStream(const char_t* input);

        ~BinaryInputStream();

        /**
         * Read an integer from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(int32_t& value);

        /**
         * Read an integer from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(uint32_t& value);

         /**
         * Reads an uint64_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        IInputStream& operator>>(uint64_t& value);

        /**
         * Reads an int64_t value from the stream
         * @param value the destination of the read value
         * @return a reference to the input stream for further processing
         */
        IInputStream& operator>>(int64_t& value);

        /**
         * Read a String from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(String&  value);

        /**
         * Read a bool from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(bool&  value);

        /**
         * Read a float from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(float& value);

        /**
         * Read a uint16_t from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(uint16_t& value);

        /**
         * Read a guid from the stream
         * @param value The variable to write the value to
         */
        IInputStream& operator>>(Guid& value);

        /**
         * Read a number of bytes from the stream
         * @param data Pointer to which the bytes will be written
         * @param size Number of bytes to read
         */
        IInputStream& read(char_t* data, const uint32_t size);

        /**
         * @see IInputstream
         */
        virtual status_t getState() const;

        /**
         * Resets the current reading position to the start of the buffer
         */
        void reset();

    protected:

        void setState(status_t state);

    private:

        /**
         * Buffer for reading the data
         */
        const char_t* mBuffer;

        /**
         * Pointer to the current position of the buffer
         */
        const char_t* mCurrent;

        /**
         * The current state of the stream
         */
        status_t mState;
    };

    
    inline
    status_t
    BinaryInputStream::getState() const
    {
        return mState;
    }

    inline
    void 
    BinaryInputStream::setState(status_t state)
    {
        mState = state;
    }

}

#endif // CAPU_BINARYINPUTSTREAM_H
