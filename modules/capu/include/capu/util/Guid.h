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

#ifndef CAPU_GUID_H
#define CAPU_GUID_H

#include "capu/os/Memory.h"
#include "capu/os/Random.h"
#include "capu/container/String.h"

namespace capu
{
    /**
     * This is the uuid struct which defines the binary format of the guid.
     */
    struct generic_uuid_t
    {
        uint32_t  Data1;
        uint16_t Data2;
        uint16_t Data3;
        uint8_t Data4[8];
    };

    /**
     * Represents a global unique identifier with 128-bit length ('Guid'). It has the representation XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX.
     */
    class Guid
    {
    public:
        /**
         * Creates a new random guid.
         */
        Guid();

        /**
         * Parses a guid from the given string. The guid will be invalid if the string does not match the
         * format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
         * @param guid The guid string.
         */
        Guid(const String& guid);

        /**
         * Takes the given generic uuid and sets it internally.
         * @param other The guid data.
         */
        Guid(const generic_uuid_t& other);

        /**
         * Copy constructor.
         * @param other The other guid which should get copied.
         */
        Guid(const Guid& other);

        /**
         * Returns a string representation of the guid.
         * @return A string representation of the guid.
         */
        const String& toString() const;

        /**
         * Checks if this guid equals the given guid.
         * @return true if the guids are equal.
         */
        bool_t equals(const Guid& other) const;

        /**
         * Checks if this guid equals the given guid.
         * @return true if the guids are equal.
         */
        bool_t operator==(const Guid& other) const;

        /**
         * Checks if the current guid is invalid which may be the case if it was parsed and the given string was no guid representation.
         * @return true if the guid is invalid.
         */
        bool_t isInvalid() const;

        /**
         * Assigns the given guid to this guid.
         * @param other The other guid.
         * @return This guid.
         */
        Guid& operator=(const Guid& other);

        /**
         * Assigns the given guid to this guid.
         * @param other The other guid.
         * @return This guid.
         */
        Guid& operator=(const generic_uuid_t& other);

        /**
         * Parses the given guid and sets this guid to the specified value. If the value is invalid, the guid will be invalid.
         * @param guid The guid string.
         * @return This guid.
         */
        Guid& parse(const String& guid);

        /**
         * Gets the raw binary guid data.
         * @return The binary guid data.
         */
        const generic_uuid_t& getGuidData() const;

    private:
        void createString() const;
        void createNew();
        generic_uuid_t m_id;
        mutable String m_stringRepresentation;
        mutable bool_t m_stringRepresentationIsInvalid;
        static Random& getRandom();
    };

    inline Guid::Guid()
        : m_stringRepresentationIsInvalid(true)
    {
        Memory::Set(&m_id, 0, sizeof(generic_uuid_t));
        createNew();
    }

    inline Guid::Guid(const String& guid)
        : m_stringRepresentationIsInvalid(true)
    {
        parse(guid);
    }

    inline Guid::Guid(const Guid& other)
        : m_id(other.m_id)
        , m_stringRepresentation()
        , m_stringRepresentationIsInvalid(true)
    {
    }

    inline Guid::Guid(const generic_uuid_t& other)
        : m_id(other)
        , m_stringRepresentationIsInvalid(true)
    {
    }

    inline const generic_uuid_t& Guid::getGuidData() const
    {
        return m_id;
    }

    inline Random& Guid::getRandom()
    {
        static Random singletonInstance;
        return singletonInstance;
    }

    inline bool_t Guid::operator==(const Guid& other) const
    {
        return equals(other);
    }

    inline Guid& Guid::operator=(const Guid& other)
    {
        m_id = other.m_id;
        m_stringRepresentationIsInvalid = true;
        m_stringRepresentation = "";
        return *this;
    }

    inline Guid& Guid::operator=(const generic_uuid_t& other)
    {
        m_id = other;
        m_stringRepresentationIsInvalid = true;
        m_stringRepresentation = "";
        return *this;
    }

    inline bool_t Guid::isInvalid() const
    {
        generic_uuid_t emptyId;
        Memory::Set(&emptyId, 0, sizeof(generic_uuid_t));
        return Memory::Compare(&emptyId, &m_id, sizeof(generic_uuid_t)) == 0;
    }

    inline const String& Guid::toString() const
    {
        if (m_stringRepresentationIsInvalid)
        {
            createString();
        }
        return m_stringRepresentation;
    }

    inline bool_t Guid::equals(const Guid& second) const
    {
        return Memory::Compare(&m_id, &second.m_id, sizeof(generic_uuid_t)) == 0;
    }

    inline void Guid::createString() const
    {
        char_t buffer[37];
        Memory::Set(buffer, 0, sizeof(buffer));
        StringUtils::Sprintf(buffer, 37, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                             m_id.Data1,
                             m_id.Data2,
                             m_id.Data3,
                             m_id.Data4[0],
                             m_id.Data4[1],
                             m_id.Data4[2],
                             m_id.Data4[3],
                             m_id.Data4[4],
                             m_id.Data4[5],
                             m_id.Data4[6],
                             m_id.Data4[7]);
        m_stringRepresentation = buffer;
        m_stringRepresentation.toUpperCase();
        m_stringRepresentationIsInvalid = false;
    }

    inline void Guid::createNew()
    {
        m_id.Data1 = getRandom().nextUInt32();
        m_id.Data2 = getRandom().nextUInt16();
        m_id.Data3 = 0x4000 | (getRandom().nextUInt16() & 0x00FF); // a guid V4 starts with 0x0100 in data3
        m_id.Data4[0] = getRandom().nextUInt8();
        m_id.Data4[1] = getRandom().nextUInt8();
        m_id.Data4[2] = getRandom().nextUInt8();
        m_id.Data4[3] = getRandom().nextUInt8();
        m_id.Data4[4] = getRandom().nextUInt8();
        m_id.Data4[5] = getRandom().nextUInt8();
        m_id.Data4[6] = getRandom().nextUInt8();
        m_id.Data4[7] = getRandom().nextUInt8();
    }

    inline Guid& Guid::parse(const String& guid)
    {
        if (guid.getLength() != 36)
        {
            // invalid length, reset to zero.
            Memory::Set(&m_id, 0, sizeof(generic_uuid_t));
            return *this;
        }

        // because scanf need 4 byte space for scanning, we must use these temp variables
        uint32_t data1, data2, data3, data40, data41, data42, data43, data44, data45, data46, data47;

        // scanf cannot be wrapped
        int_t retVal;
#ifdef OS_WINDOWS
        retVal = sscanf_s(guid.c_str(), "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                          &data1,
                          &data2,
                          &data3,
                          &data40,
                          &data41,
                          &data42,
                          &data43,
                          &data44,
                          &data45,
                          &data46,
                          &data47);
#else
        retVal = sscanf(guid.c_str(), "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        &data1,
                        &data2,
                        &data3,
                        &data40,
                        &data41,
                        &data42,
                        &data43,
                        &data44,
                        &data45,
                        &data46,
                        &data47);
#endif

        if (retVal == 11)
        {
            // successful scan
            m_id.Data1 = data1;
            m_id.Data2 = static_cast<uint16_t>(data2);
            m_id.Data3 = static_cast<uint16_t>(data3);
            m_id.Data4[0] = static_cast<uint8_t>(data40);
            m_id.Data4[1] = static_cast<uint8_t>(data41);
            m_id.Data4[2] = static_cast<uint8_t>(data42);
            m_id.Data4[3] = static_cast<uint8_t>(data43);
            m_id.Data4[4] = static_cast<uint8_t>(data44);
            m_id.Data4[5] = static_cast<uint8_t>(data45);
            m_id.Data4[6] = static_cast<uint8_t>(data46);
            m_id.Data4[7] = static_cast<uint8_t>(data47);
            m_stringRepresentation = guid;
            m_stringRepresentation.toUpperCase();
            m_stringRepresentationIsInvalid = false;
        }
        else
        {
            // error
            Memory::Set(&m_id, 0, sizeof(generic_uuid_t));
            m_stringRepresentationIsInvalid = true;
        }

        return *this;
    }

    /**
     * Specialization of Hash in order to calculate the Hash differently for strings
     */
    template<>
    struct Hasher<capu::Guid, CAPU_TYPE_CLASS>
    {
        static uint_t Hash(const Guid& key, const uint8_t bitsize)
        {
            return HashCalculator<uint_t>::Hash(&key.getGuidData(), sizeof(generic_uuid_t), bitsize);
        }
    };
}
#endif //CAPU_GUID_H
