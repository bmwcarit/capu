/*
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

#ifndef CAPU_IPHONEOS_ARM64_UDP_SOCKET_H
#define CAPU_IPHONEOS_ARM64_UDP_SOCKET_H

#include <capu/os/iPhoneOS/UdpSocket.h>

namespace capu
{
    namespace os
    {
        namespace arch
        {
            class UdpSocket: private capu::os::UdpSocket
            {
            public:
                using capu::os::UdpSocket::bind;
                using capu::os::UdpSocket::send;
                using capu::os::UdpSocket::receive;
                using capu::os::UdpSocket::close;
                using capu::os::UdpSocket::setBufferSize;
                using capu::os::UdpSocket::setTimeout;
                using capu::os::UdpSocket::getBufferSize;
                using capu::os::UdpSocket::getTimeout;
                using capu::os::UdpSocket::getSocketAddrInfo;
            };
        }
    }
}

#endif /* CAPU_IPHONEOS_ARM64_UDP_SOCKET_H */
