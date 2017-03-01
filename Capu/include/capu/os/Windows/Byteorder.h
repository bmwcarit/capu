/*
 * Copyright (C) 2017 BMW Car IT GmbH
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

#ifndef CAPU_WINDOWS_BYTEORDER_H
#define CAPU_WINDOWS_BYTEORDER_H

#include <Winsock2.h>

#include "capu/capustdint.h"

namespace capu
{
    namespace os
    {
        class Byteorder
        {
        public:
            static uint32_t NtoHl(uint32_t netlong);
            static uint16_t NtoHs(uint16_t netshort);
            static uint32_t HtoNl(uint32_t hostlong);
            static uint16_t HtoNs(uint16_t hostshort);
        };

        inline uint32_t Byteorder::NtoHl(uint32_t netlong)
        {
            return ntohl(netlong);
        }

        inline uint16_t Byteorder::NtoHs(uint16_t netshort)
        {
            return ntohs(netshort);
        }

        inline uint32_t Byteorder::HtoNl(uint32_t hostlong)
        {
            return htonl(hostlong);
        }

        inline uint16_t Byteorder::HtoNs(uint16_t hostshort)
        {
            return htons(hostshort);
        }
    }
}

#endif // CAPU_WINDOWS_BYTEORDER_H
