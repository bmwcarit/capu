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

#ifndef CAPU_ALGORITHM_H
#define CAPU_ALGORITHM_H

#include "capu/util/Iterator.h"

namespace capu
{
    /**
     * Copy range to destination iterator
     * @param first begin of source range
     * @param last non-inclusive end of source range
     * @param dest start of destination range
     */
    template <class InputIt, class OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt dest)
    {
        while (first != last)
        {
            *dest = *first;
            ++first;
            ++dest;
        }
        return dest;
    }

    /**
    * Copy range to destination iterator
    * @param first begin of source range
    * @param last non-inclusive end of source range
    * @param lastResult non-inclusive end of destination range
    */
    template <class InputIt, class OutputIt>
    OutputIt copy_backward(InputIt first, InputIt last, OutputIt lastResult)
    {
        while (first != last)
        {
            --last;
            --lastResult;
            *lastResult = *last;
        }
        return lastResult;
    }

    /**
     * Fill a number of elements with given value
     * @param first start of range to fill
     * @param count number of consecutive elements to fill
     * @param value reference value used set elements to
     */
    template <class OutputIt, class Size, class T>
    OutputIt fill_n(OutputIt first, Size count, const T& value)
    {
        for (Size i = 0; i < count; ++i)
        {
            *first = value;
            ++first;
        }
        return first;
    }

    /**
     * Compare two equal-length ranges for default equality
     * @param first1 begin of first range
     * @param last1 non-inclusive end of first range
     * @param first2 begin of second range to compare to
     */
    template <class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
    {
        while (first1 != last1)
        {
            if (*first1 != *first2)
            {
                return false;
            }
            ++first1;
            ++first2;
        }
        return true;
    }
}

#endif // CAPU_ALGORITHM_H
