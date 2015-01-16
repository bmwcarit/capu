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

#ifndef CAPU_SOCKETINPUTSTREAM_H
#define CAPU_SOCKETINPUTSTREAM_H

#include <capu/Config.h>
#include <capu/os/Socket.h>
#include <capu/container/String.h>
#include <capu/util/Guid.h>
#include <capu/util/IInputStream.h>

namespace capu
{

    inline
    uint64_t 
    _ntohll(uint64_t & value)
    {
        int_t checkNumber = 42;
        if(*(char_t*)&checkNumber == 42)
        {
            // Little endian
            const uint64_t lowbits  = (uint64_t)(ntohl(value & 0xFFFFFFFF)) << 32LL;
            const uint64_t highbits = (uint64_t)ntohl(value >> 32);
            return lowbits | highbits;
        }
        else
        {
            // Big endian
            return value;
        }
    }

    /**
     * The SocketInputStream reads date from a socket
     */
    class SocketInputStream: public IInputStream
    {
    public:

        /**
         * Constructor of SocketInputStream
         * @param socket to read data from
         */
        SocketInputStream();
        ~SocketInputStream();

        /**
         * Read an uint8_t value from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(uint8_t& value);

        /**
         * Read an uint8_t value from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(int8_t& value);

        /**
         * Read an uint16_t value from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(uint16_t& value);

        /**
         * Read an int16_t value from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(int16_t& value);

        /**
         * Read an unsigned int from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(uint32_t& value);

        /**
         * Read a signed int from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(int32_t& value);

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
         * Read a string from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(String&  value);

        /**
         * Read a bool from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(bool_t&  value);

        /**
         * Read a float from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(float_t& value);

        /**
         * Read a Guid from the stream
         * @param value Reference to variable to store the value read
         */
        IInputStream& operator>>(Guid& value);


        /**
         * Read bytes from the stream
         * @param data Pointer to store data read
         * @param size Number of bytes to read
         */
        virtual IInputStream& read(char_t* data, const uint32_t size) = 0;

        /**
         * Returns the current state of the SocketInputStream
         * CAPU_OK if everything is ok
         * CAPU_ETIMEOUT if a timeout has occured
         * CAPU_ERROR if there is a problem with the socket
         * @return the state of the socket
         */
        virtual status_t getState() const;

        /**
         * Resets the state to CAPU_OK
         */
        void resetState();


    protected:
        /**
         * The state of the input stream
         */
        status_t mState;

    private:


    };

    inline
    SocketInputStream::SocketInputStream()
        : mState(CAPU_OK)
    {
    }

    inline
    SocketInputStream::~SocketInputStream()
    {
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(int32_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(int32_t));
        value = ntohl(value);
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(uint32_t& value)
    {
        int32_t tmp;
        operator>>(tmp);
        value = static_cast<uint32_t>(tmp);
        return *this;
    }

    inline
    IInputStream& SocketInputStream::operator>>(uint64_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint64_t));
        value = _ntohll(value);
        return *this;
    }

    inline
    IInputStream& SocketInputStream::operator>>(int64_t& value)
    {
        uint64_t tmp = 0;
        operator>>(tmp);
        value = *reinterpret_cast<int64_t*>(&tmp);
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(bool_t& value)
    {
        return read(reinterpret_cast<char_t*>(&value), sizeof(bool_t));
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(float_t& value)
    {
        union
        {
            uint32_t intValue;
            float_t  floatValue;
        } floatConvert;

        operator>>(floatConvert.intValue);
        value = floatConvert.floatValue;

        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(uint16_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint16_t));
        value = ntohs(value);
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(int16_t& value)
    {
        uint16_t tmp = 0;
        operator>>(tmp);
        value = *reinterpret_cast<int16_t*>(&tmp);
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(uint8_t& value)
    {
        read(reinterpret_cast<char_t*>(&value), sizeof(uint8_t));
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(int8_t& value)
    {
        uint8_t tmp = 0;
        operator>>(tmp);
        value = *reinterpret_cast<int8_t*>(&tmp);
        return *this;
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(Guid& value)
    {
        generic_uuid_t fromStream;
        read(reinterpret_cast<char_t*>(&fromStream), sizeof(generic_uuid_t));
        value = fromStream;
        return *this;
    }

    inline
    void
    SocketInputStream::resetState()
    {
        mState = CAPU_OK;
    }

    inline
    status_t
    SocketInputStream::getState() const
    {
        return mState;
    }
}

#endif // CAPU_SOCKETINPUTSTREAM_H
