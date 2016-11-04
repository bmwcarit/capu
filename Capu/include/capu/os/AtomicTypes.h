/*
 * Copyright (C) 2016 BMW Car IT GmbH
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

#ifndef CAPU_ATOMIC_TYPES_H
#define CAPU_ATOMIC_TYPES_H

#include "capu/Config.h"
#include "capu/os/PlatformInclude.h"

#include CAPU_PLATFORM_INCLUDE(AtomicTypes)

namespace capu
{
    /**
     * A boolean type that offers atomic load and store functionality
     */
    class AtomicBool: private os::arch::AtomicBool
    {
    public:
        /**
         * @brief The default constructor
         * @details The default value is \c false
         */
        AtomicBool();

        /**
         * @brief Constructor with an initial value
         *
         * @param init The initial value for the boolean type
         */
        AtomicBool(bool init);

        /**
         * @brief The method returns the current value of the bool atomically
         * @return The current boolean value
         */
        bool load() const;

        /**
         * @brief Sets the value of the bool atomically
         *
         * @param desired The new value of the bool
         */
        void store(bool desired);

        /**
         * @brief Assignment of value
         *
         * @param rhs The value to be assigned
         * @return The assigned value
         */
        bool operator=(bool rhs);

        /**
         * @brief Type operator to implicitly cast to bool
         */
        operator bool() const;
    };


    inline
    AtomicBool::AtomicBool()
    {
        store(false);
    }

    inline
    AtomicBool::AtomicBool(bool init)
    {
        store(init);
    }

    inline
    bool AtomicBool::load() const
    {
        return os::arch::AtomicBool::load();
    }

    inline
    void AtomicBool::store(bool desired)
    {
        os::arch::AtomicBool::store(desired);
    }

    inline
    bool AtomicBool::operator=(bool rhs)
    {
        store(rhs);
        return rhs;
    }

    inline
    AtomicBool::operator bool() const
    {
        return load();
    }
}

#endif // CAPU_ATOMIC_TYPES_H
