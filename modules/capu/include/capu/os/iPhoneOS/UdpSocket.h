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

#ifndef CAPU_IPHONEOS_UDP_SOCKET_H
#define CAPU_IPHONEOS_UDP_SOCKET_H

#include <capu/os/Posix/UdpSocket.h>

namespace capu
{
    namespace iphoneos
    {
        class UdpSocket: private capu::posix::UdpSocket
        {
        public:
            using capu::posix::UdpSocket::bind;
            using capu::posix::UdpSocket::send;
            using capu::posix::UdpSocket::receive;
            using capu::posix::UdpSocket::close;
            using capu::posix::UdpSocket::setBufferSize;
            using capu::posix::UdpSocket::setTimeout;
            using capu::posix::UdpSocket::getBufferSize;
            using capu::posix::UdpSocket::getTimeout;
            using capu::posix::UdpSocket::getSocketAddrInfo;
        };
    }
}

#endif // CAPU_IPHONEOS_UDP_SOCKET_H
