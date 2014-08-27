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

#ifndef CAPU_UNIXBASED_SOCKET_H
#define CAPU_UNIXBASED_SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <capu/container/Pair.h>
#include <capu/util/Delegate.h>

namespace capu {
    namespace os {
        typedef int32_t SocketDescription;
        typedef socklen_t socketOptionLen;

        typedef Delegate<void, const capu::os::SocketDescription&> SocketDelegate;
        typedef Pair<capu::os::SocketDescription, SocketDelegate> SocketInfoPair;
    }
}

#define CAPU_INVALID_SOCKET -1
#define CAPU_SOCKET_ERROR -1

#endif // CAPU_UNIXBASED_SOCKET_H
