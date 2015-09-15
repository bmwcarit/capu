/*
* Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_ITERATOR_H
#define CAPU_ITERATOR_H

#include "capu/Config.h"

namespace capu
{
    /// Tag type for input iterators
    struct input_iterator_tag {};
    /// Tag type for forward iterators
    struct forward_iterator_tag : public input_iterator_tag {};
    /// Tag type for bidirectional iterators
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    /// Tag type for random accessible iterators
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    /// Tag type for output iterators
    struct output_iterator_tag {};

    /**
     * Base class for iterators
     * @param Category iterator_tag type
     * @param T the type iterator encapsules
     * @param Distance type that can represent the difference between iterators of this type
     * @param Pointer pointer to element in iterator
     * @param Reference reference to element in iterator
     */

    template <
        class Category,
        class T,
        class Distance = int_t,
        class Pointer = T*,
        class Reference = T&
    > struct iterator
    {
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Category iterator_category;
    };

    /// iterator trait struct to specialize for iterators not deriving from struct iterator
    template <class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::iterator_category iterator_category;
    };

    // iterator_traits specialization for pointers
    template <class T>
    struct iterator_traits<T*> : public iterator<random_access_iterator_tag, T> {};

    // iterator_traits specialization for constant pointers
    template <class T>
    struct iterator_traits<const T*> : public iterator<random_access_iterator_tag, T> {};
}

#endif // CAPU_ITERATOR_H
