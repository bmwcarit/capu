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

#ifndef CAPU_WINDOWS_SOCKET_H
#define CAPU_WINDOWS_SOCKET_H

#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <capu/util/Delegate.h>
#include <capu/container/Pair.h>

namespace capu {
    namespace os {
        typedef SOCKET SocketDescription;
        typedef int32_t socketOptionLen;

        typedef Delegate<void, const capu::os::SocketDescription&> SocketDelegate;
        typedef Pair<capu::os::SocketDescription, SocketDelegate> SocketInfoPair;
    }
}

#define CAPU_INVALID_SOCKET INVALID_SOCKET
#define CAPU_SOCKET_ERROR SOCKET_ERROR

#endif // CAPU_WINDOWS_SOCKET_H

