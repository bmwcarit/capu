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

#ifndef CAPU_SOCKETOUTPUTSTREAM_H
#define CAPU_SOCKETOUTPUTSTREAM_H

#include <capu/util/IOutputStream.h>
#include <capu/os/Socket.h>
#include <capu/os/StringUtils.h>
#include <capu/os/Memory.h>
#include <capu/util/Guid.h>

namespace capu
{
    uint64_t _htonll(const uint64_t value);

    /* The SocketOutputStream writes data to a given socket*/
    template<uint16_t SNDBUFSIZE = 1450>
    class SocketOutputStream: public IOutputStream
    {
    public:

        /**
         * Constructor with socket to write to
         * @param socket to write to
         */
        SocketOutputStream();
        virtual ~SocketOutputStream();

        /**
         * Write an integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const int8_t value);

        /**
         * Write an unsigned integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const uint8_t value);

        /**
         * Write an integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const int16_t value);

        /**
         * Write an unsigned integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const uint16_t value);

        /**
         * Write an integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const int32_t value);

        /**
         * Write an unsigned integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const uint32_t value);


        /**
         * Write an integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const int64_t value);

        /**
         * Write an unsigned integer to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const uint64_t value);

        /**
         * Write a string to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const String& value);

        /**
         * Write a boolean to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const bool_t value);

        /**
         * Write a float to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const float_t value);

        /**
         * Write a character to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const char_t* value);

        /**
         * Write a byte to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const void* value);

        /**
         * Write a Guid to the stream
         * @param value Value to write to the stream
         */
        IOutputStream& operator<<(const Guid& value);

        /**
         * Write bytes to the stream
         * @param data Pointer to the byte data
         * @param size Number of bytes to write tot he stream
         */
        IOutputStream& write(const void* data, const uint32_t size);

        /**
         * Flushes the internal data to the socket. This also happens if the internal buffer is full.
         * Don't forget to flush.
         * @return The status of the socket output stream.
         */
        status_t flush();

        /**
         * Returns the state of the stream.
         * @return The state of the stream.
         */
        status_t getState() const;

        /**
         * Resets the state of the stream to CAPU_OK.
         */
        void resetState();

    protected:
        virtual status_t writeToSocket(const char_t* buffer, const uint32_t size, int32_t&  numBytes) = 0;

    private:
        /**
         * TcpSocket to write the data to.
         */
        char_t   mBuffer[SNDBUFSIZE];
        uint16_t mBufferSize;
        status_t m_state;

        void writeToInternalBuffer(const void* data, const uint32_t size);
        void internalSend(const char_t* data, const uint32_t size);
    };

    template<uint16_t SNDBUFSIZE>
    inline
    SocketOutputStream<SNDBUFSIZE>::SocketOutputStream()
        : mBufferSize(0)
        , m_state(CAPU_OK)
    {
    }
    template<uint16_t SNDBUFSIZE>
    inline
    void SocketOutputStream<SNDBUFSIZE>::resetState()
    {
        m_state = CAPU_OK;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    status_t SocketOutputStream<SNDBUFSIZE>::getState() const
    {
        return m_state;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    SocketOutputStream<SNDBUFSIZE>::~SocketOutputStream()
    {
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream& SocketOutputStream<SNDBUFSIZE>::operator<<(const int32_t value)
    {
        const int32_t networkOrder = htonl(value);
        return write(&networkOrder, sizeof(int32_t));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const uint32_t value)
    {
        return operator<<(static_cast<int32_t>(value));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<( const int64_t value)
    {
        const int64_t networkOrder = _htonll(value);
        return write(&networkOrder, sizeof(int64_t));
    }
    
    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream& 
    SocketOutputStream<SNDBUFSIZE>::operator<<( const uint64_t value)
    {
        return operator<<(static_cast<int64_t>(value));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const String& value)
    {
        const uint32_t length = static_cast<uint32_t>(value.getLength());
        operator<<(length);
        return write(value.c_str(), length);
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const char_t* value)
    {
        const int32_t length = static_cast<uint32_t>(StringUtils::Strlen(value));
        operator<<(length);
        return write(value, length);
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream& SocketOutputStream<SNDBUFSIZE>::operator<<(const int16_t value)
    {
        const int16_t networkOrder = htons(value);
        return write(&networkOrder, sizeof(int16_t));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const uint16_t value)
    {
        return operator<<(static_cast<int16_t>(value));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream& SocketOutputStream<SNDBUFSIZE>::operator<<(const int8_t value)
    {
        return write(&value, sizeof(int8_t));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const uint8_t value)
    {
        return operator<<(static_cast<int8_t>(value));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const bool_t value)
    {
        return write(reinterpret_cast<const char_t*>(&value), sizeof(bool_t));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const float_t value)
    {
        // we assume that a float is 4 bytes long (=uint32_t)
        union
        {
            float_t floatVal;
            uint32_t uint32Val;
        } uint32Convert;

        uint32Convert.floatVal = value;

        return operator<<(uint32Convert.uint32Val);
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const void* value)
    {
        return write(reinterpret_cast<const char_t*>(value), sizeof(void*));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::operator<<(const Guid& value)
    {
        return write(reinterpret_cast<const char_t*>(&value.getGuidData()), sizeof(generic_uuid_t));
    }

    template<uint16_t SNDBUFSIZE>
    inline
    void
    SocketOutputStream<SNDBUFSIZE>::writeToInternalBuffer(const void* data, const uint32_t size)
    {
        Memory::Copy(&mBuffer[mBufferSize], data, size);
        mBufferSize += static_cast<uint16_t>(size);
    }

    template<uint16_t SNDBUFSIZE>
    inline
    IOutputStream&
    SocketOutputStream<SNDBUFSIZE>::write(const void* data, const uint32_t size)
    {
        // Check for free space in internal buffer
        if (SNDBUFSIZE > mBufferSize + size)
        {
            writeToInternalBuffer(data, size);
        }
        else
        {
            flush();
            internalSend(static_cast<const char_t*>(data), size);
        }
        return *this;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    status_t
    SocketOutputStream<SNDBUFSIZE>::flush()
    {
        internalSend(mBuffer, mBufferSize);
        mBufferSize = 0;
        return m_state;
    }

    template<uint16_t SNDBUFSIZE>
    inline
    void
    SocketOutputStream<SNDBUFSIZE>::internalSend(const char_t* data, const uint32_t size)
    {
        if (size == 0 || m_state != CAPU_OK)
        {
            return;
        }
        uint32_t sentBytes = 0;
        do
        {
            int32_t numBytes = 0;
            m_state = writeToSocket(data + sentBytes, size - sentBytes, numBytes);
            if (m_state != CAPU_OK)
            {
                // sending failed, don't continue
                return;
            }
            sentBytes += numBytes;
        }
        while (sentBytes != size);
    }


    inline
    uint64_t 
    _htonll(const uint64_t value)
    {
        int64_t checkNumber = 42;
        if(*(char_t*)&checkNumber == 42)
        {
            // Little endian
            const uint64_t lowbits  = (uint64_t)(htonl(value & 0xFFFFFFFF)) << 32LL;
            const uint64_t highbits = (uint64_t)htonl(value >> 32);
            return lowbits | highbits;
        }
        else
        {
            // Big endian
            return value;
        }
    }
}

#endif // CAPU_SOCKETOUTPUTSTREAM_H
