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

#ifndef CAPU_HASH_H
#define CAPU_HASH_H

#include "capu/Config.h"
#include "capu/util/Traits.h"
#include "capu/os/Debug.h"

namespace capu
{
    /*************************************************************************************/
    /****************************** Resizers Traits **************************************/
    /*************************************************************************************/

    /**
     * No default trait for Resizer
     */
    template<typename T>
    struct Resizer;

    /**
     * Resizer for Car IT Hash with value uint32_t
     */
    template<>
    struct Resizer<uint32_t>
    {
        static uint32_t Resize(const uint32_t hashValue, const uint8_t bitcount)
        {
            return bitcount >= 32 ? hashValue : hashValue & ((static_cast<uint32_t>(1) << bitcount) - 1);
        }
    };

    /**
     * Resizer for FNV Hash with value uint64_t
     */
    template<>
    struct Resizer<uint64_t>
    {
        static uint64_t Resize(const uint64_t hashValue, const uint8_t bitcount)
        {
            return bitcount >= 64 ? hashValue : hashValue & ((static_cast<uint64_t>(1) << (bitcount)) - 1);
        }
    };

    /*************************************************************************************/
    /******************************Trait for Hash functions ******************************/
    /*************************************************************************************/

    /**
     * No default implementation for Hash function trait
     */
    template<typename T>
    struct HashFunction;

    /**
     * Trait for Hash function which returns a uint32_t value for different types of keys
     */
    template<>
    struct HashFunction<uint32_t>
    {
        //FNV Has prime numbers for 32 bit
        /**
         * A prime number for use in hash functions
         */
        static const uint32_t prime = 16777619UL;

        /**
         * Offset used in hash functions
         */
        static const uint32_t offset_base = 2166136261UL;

        //Car IT Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const uint32_t key)
        {
            return key ^ (((key >> 16) ^ key) >> 8);
        }

        //Car IT Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const int32_t key)
        {
            return key ^ (((key >> 16) ^ key) >> 8);
        }

        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const float key)
        {
            return Hash(static_cast<int32_t>(key));
        }

        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const double key)
        {
            return Hash(static_cast<int64_t>(key));
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const uint64_t key)
        {
            uint32_t result = offset_base;
            const void* keyPtr = &key;
            const uint32_t* ptr = static_cast<const uint32_t*>(keyPtr);
            result = (result ^ *ptr) * prime;
            ++ptr;
            return (result ^ *ptr) * prime;
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const int64_t key)
        {
            uint32_t result = offset_base;
            const void* keyPtr = &key;
            const int32_t* ptr = static_cast<const int32_t*>(keyPtr);
            result = (result ^ *ptr) * prime;
            ++ptr;
            return (result ^ *ptr) * prime;
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint32_t Hash(const char* key)
        {
            uint32_t result = offset_base;
            while (*key)
            {
                result = (result ^ *key) * prime;
                ++key;
            }
            return result;
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @param len Number of bytes to hash from the given key pointer
         * @return the computed hash value
         */
        static uint32_t Hash(const void* key, const uint_t len)
        {
            uint_t length = len;
            uint32_t result = offset_base;
            const uint8_t* ptr = static_cast<const uint8_t*>(key);
            do
            {
                result = (result ^ *ptr) * prime;
                ++ptr;
            }
            while (--length);

            return result;
        }
    };

    /**
     * Trait for Hash function which returns a uint64_t value for different types of keys
     */
    template<>
    struct HashFunction<uint64_t>
    {
        //FNV Has prime numbers for 64 bit
        /**
         * A prime number for use in hash functions
         */
        static const uint64_t prime = 1099511628211ULL;

        /**
         * Offset used in hash functions
         */
        static const uint64_t offset_base = 14695981039346656037ULL;

        //Car IT Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const uint64_t key)
        {
            return key ^ (((((key >> 32) ^ key) >> 16) ^ key) >> 8);
        }

        //Car IT Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const int64_t key)
        {
            return key ^ (((((key >> 32) ^ key) >> 16) ^ key) >> 8);
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const float key)
        {
            return Hash(static_cast<int64_t>(key));
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const double key)
        {
            return Hash(static_cast<int64_t>(key));
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key The key to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const char* key)
        {
            uint64_t result = offset_base;
            while (*key)
            {
                result = (result ^ *key) * prime;
                ++key;
            }
            return result;
        }

        //FNV Hash
        /**
         * Compute a hashvalue for the given key.
         * @param key Pointer to the key data to hash
         * @param len The length of the key data to hash
         * @return the computed hash value
         */
        static uint64_t Hash(const void* key, const uint_t len)
        {
            uint64_t length = len;
            uint64_t result = offset_base;
            const uint8_t* ptr = static_cast<const uint8_t*>(key);
            do
            {
                result = (result ^ *ptr) * prime;
                ++ptr;
            }
            while (--length);

            return result;
        }
    };

    /**************************************************************************************/
    /******************************Trait for Hash calculator ******************************/
    /***Choosing the correct hash function and resizer for the desired hash result value***/
    /**************************************************************************************/

    /**
     * No default mapping between hash function and resizer
     */
    template<typename T>
    struct HashCalculator;

    /**
     * Mapping of uint32_t hash result value to the respective hash and resizer functions
     */
    template<>
    struct HashCalculator<uint32_t>
    {
        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(uint32_t      key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(int32_t       key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(float       key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(double      key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(uint64_t      key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(int64_t       key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(const char* key, uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint32_t Hash(const void* key, const uint_t len, const uint8_t bitcount)
        {
            return Resizer<uint32_t>::Resize(HashFunction<uint32_t>::Hash(key, len), bitcount);
        }
    };

    /**
     * Mapping of uint64_t hash result value to the respective hash and resizer functions
     */
    template<>
    struct HashCalculator<uint64_t>
    {
        /**
        * Hash the given key using the given bitcount
        */
        static uint64_t Hash(const uint32_t key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(static_cast<uint64_t>(key)), bitcount);
        }

        /**
        * Hash the given key using the given bitcount
        */
        static uint64_t Hash(const int32_t  key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(static_cast<int64_t>(key)), bitcount);
        }
        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const uint64_t key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const int64_t  key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const float  key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const double key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key), bitcount);
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const char*  key, uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key), bitcount);    // Use 64 bit resizer
        }

        /**
         * Hash the given key using the given bitcount
         */
        static uint64_t Hash(const void* key, const uint_t len, const uint8_t bitcount)
        {
            return Resizer<uint64_t>::Resize(HashFunction<uint64_t>::Hash(key, len), bitcount);
        }
    };

    /**************************************************************************************/
    /*********************************** Hash class ***************************************/
    /**************************************************************************************/
    template<typename T, int TYPE, typename INTRESULTTYPE>
    struct Hasher
    {
        static INTRESULTTYPE Hash(const T& key, const uint8_t bitsize)
        {
            // default hasher
            return HashCalculator<INTRESULTTYPE>::Hash(&key, sizeof(T), bitsize);
        }
    };

    template<typename T, typename INTRESULTTYPE>
    struct Hasher<T, CAPU_TYPE_PRIMITIVE, INTRESULTTYPE>
    {
        static INTRESULTTYPE Hash(const T key, const uint8_t bitsize)
        {
            // hasher for primitives
            return HashCalculator<INTRESULTTYPE>::Hash(key, bitsize);
        }
    };

    template<typename T, typename INTRESULTTYPE>
    struct Hasher<T, CAPU_TYPE_ENUM, INTRESULTTYPE>
    {
        static INTRESULTTYPE Hash(const T key, const uint8_t bitsize)
        {
            // hasher for enums is the primitive hasher
            return Hasher<T, CAPU_TYPE_PRIMITIVE, INTRESULTTYPE>::Hash(key, bitsize);
        }
    };

    template<typename T, typename INTRESULTTYPE>
    struct Hasher<T, CAPU_TYPE_POINTER, INTRESULTTYPE>
    {
        static INTRESULTTYPE Hash(const T key, const uint8_t bitsize)
        {
            // shortcut: a pointer is already a hash, we need only the resizer
            const T *keyPtr = &key;
            return static_cast<INTRESULTTYPE>(Resizer<uint_t>::Resize(*reinterpret_cast<const uint_t*>(keyPtr), bitsize));
        }
    };


    template<int_t INTSIZE = (sizeof(uint_t)* 8)>
    struct CapuDefaultHashFunction
    {
    };

    template<>
    struct CapuDefaultHashFunction<32>
    {
        template<typename T>
        static uint32_t Digest(const T& key, const uint8_t bitsize = sizeof(uint_t)* 8)
        {
            Debug::Assert(bitsize <= 32);
            return Hasher<T, Type<T>::Identifier, uint32_t>::Hash(key, bitsize);
        }
    };

    template<>
    struct CapuDefaultHashFunction<64>
    {
        template<typename T>
        static uint64_t Digest(const T& key, const uint8_t bitsize = sizeof(uint_t)* 8)
        {
            Debug::Assert(bitsize <= 64);
            return Hasher<T, Type<T>::Identifier, uint64_t>::Hash(key, bitsize);
        }
    };
}
#endif /* CAPU_HASH_H */

