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
#include <assert.h>

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


    template <class InputIt>
    typename capu::iterator_traits<InputIt>::difference_type DistanceHelper(InputIt first, InputIt last, random_access_iterator_tag)
    {
        return last - first;
    }

    template <class InputIt>
    typename capu::iterator_traits<InputIt>::difference_type DistanceHelper(InputIt first, InputIt last, input_iterator_tag)
    {
        typename capu::iterator_traits<InputIt>::difference_type result = 0;
        while (first != last)
        {
            ++first;
            ++result;
        }
        return result;
    }

    template <class InputIt>
    typename capu::iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last)
    {
        typedef typename capu::iterator_traits<InputIt>::iterator_category iterator_category;
        return DistanceHelper(first, last, iterator_category());
    }


    template <class InputIt, class Distance>
    void AdvanceHelper(InputIt& it, Distance n, input_iterator_tag)
    {
        assert(n >= 0);
        while (n > 0)
        {
            ++it;
            --n;
        }
    }

    template <class InputIt, class Distance>
    void AdvanceHelper(InputIt& it, Distance n, bidirectional_iterator_tag)
    {
        while (n > 0)
        {
            --n;
            ++it;
        }
        while (n < 0)
        {
            ++n;
            --it;
        }
    }

    template <class InputIt, class Distance>
    void AdvanceHelper(InputIt& it, Distance n, random_access_iterator_tag)
    {
        it = it + n;
    }

    template <class InputIt, class Distance>
    void advance(InputIt& it, Distance n)
    {
        typedef typename iterator_traits<InputIt>::iterator_category iterator_category;
        AdvanceHelper<InputIt, Distance>(it, n, iterator_category());
    }
}

#endif // CAPU_ITERATOR_H
