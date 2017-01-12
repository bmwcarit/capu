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

#ifndef CAPU_ATOMIC_H
#define CAPU_ATOMIC_H

#include "capu/Config.h"
#include <atomic>
#include <type_traits>

namespace capu
{
    /**
     * A generic class that offers atomic operations
     */
    template <typename T>
    class Atomic {
    public:
        Atomic() = default;

        Atomic(T value)
            : mValue(value)
        {
        }

        Atomic& operator=(const Atomic&) = delete;
        Atomic(const Atomic&) = delete;

        T load() const
        {
            return mValue.load();
        }

        operator T() const
        {
            return mValue.load();
        }

        void store(T value)
        {
            mValue.store(value);
        }

        T operator=(T value)
        {
            mValue.store(value);
            return value;
        }

        T operator++()
        {
            return ++mValue;
        }

        T operator--()
        {
            return --mValue;
        }

        T operator++(int)
        {
            return mValue++;
        }

        T operator--(int)
        {
            return mValue--;
        }

        T operator+=(T value)
        {
            return mValue += value;
        }

        T operator-=(T value)
        {
            return mValue -= value;
        }

        static_assert(std::is_integral<T>::value || std::is_pointer<T>::value,
                      "Atomic<T> supports only integral and pointer types");
        static_assert(sizeof(T) <= sizeof(capu::uint_t),
                      "Atomic<T> type T must fit into architecture dependant word size");
    private:
        std::atomic<T> mValue;
    };
}

#endif // CAPU_ATOMIC_H
