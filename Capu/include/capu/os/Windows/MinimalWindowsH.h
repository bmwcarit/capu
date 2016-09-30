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

#ifndef CAPU_MINIMAL_WINDOWSH_H
#define CAPU_MINIMAL_WINDOWSH_H

#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES 
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS 
#define NOMENUS
#define NOICONS 
#define NOKEYSTATES 
#define NOSYSCOMMANDS 
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM 
#define NOCLIPBOARD 
#define NOCOLOR 
#define NOCTLMGR 
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER 
#define NONLS
#define NOMB 
#define NOMEMMGR 
#define NOMETAFILE 
#define NOMINMAX 
#define NOMSG 
#define NOOPENFILE 
#define NOSCROLL 
#define NOSERVICE 
#define NOSOUND 
#define NOTEXTMETRIC 
#define NOWH 
#define NOWINOFFSETS 
#define NOCOMM 
#define NOKANJI 
#define NOHELP 
#define NOPROFILER
#define NODEFERWINDOWPOS 
#define NOMCX 

#include <windows.h>

#undef min
#undef max

#endif
