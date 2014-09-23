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

#ifndef CAPU_ALLOCATOR_H
#define CAPU_ALLOCATOR_H

namespace capu
{
    template<typename T>
    class Allocator
    {
    public:

        T* allocate();
        void deallocate(T*& element);

    protected:
    private:
    };

    template<typename T>
    T* Allocator<T>::allocate()
    {
        return new T;
    }

    template<typename T>
    void Allocator<T>::deallocate(T*& element)
    {
        delete element;
        element = 0;
    }

}

#endif // CAPU_Allocator_H
