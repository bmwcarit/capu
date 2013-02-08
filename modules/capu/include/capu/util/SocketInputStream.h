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

        IInputStream& operator>>(uint16_t& value);

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
        status_t getState() const;

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
    IInputStream&
    SocketInputStream::operator>>(bool_t& value)
    {
        return read(reinterpret_cast<char_t*>(&value), sizeof(bool_t));
    }

    inline
    IInputStream&
    SocketInputStream::operator>>(float_t& value)
    {
        uint32_t tmpValue = 0;

        operator>>(tmpValue);
        value = *reinterpret_cast<float_t*>(&tmpValue);

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
