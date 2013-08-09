#
# Copyright (C) 2012 BMW Car IT GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ACME_ADD_MODULE(capu static)


MACRO(ADD_PLATFORM_FILE filename)
	ACME_ADD_FILE(os/${filename} SOURCE_GROUP "os")
	ACME_ADD_FILE(os/${TARGET_OS}/${filename} SOURCE_GROUP "os/${TARGET_OS}")
	ACME_ADD_FILE(os/${TARGET_OS}/${TARGET_ARCH}/${filename} SOURCE_GROUP "os/${TARGET_OS}_${TARGET_ARCH}")

IF(NOT ${TARGET_OS} 	STREQUAL "Windows")
	ACME_ADD_FILE(os/Posix/${filename} SOURCE_GROUP "os/Posix")
ENDIF()

	ACME_ADD_FILE(os/Generic/${filename} SOURCE_GROUP "os/Generic")
ENDMACRO()

MACRO(ADD_CONTAINER_FILE filename)
    ACME_ADD_FILE(container/${filename} SOURCE_GROUP "container")
ENDMACRO()

MACRO(ADD_UTIL_FILE filename)
    ACME_ADD_FILE(util/${filename} SOURCE_GROUP "util")
ENDMACRO()

ACME_ADD_FILE(Config)
ACME_ADD_FILE(Error)

ADD_PLATFORM_FILE(PlatformInclude)
ADD_PLATFORM_FILE(Semaphore)
ADD_PLATFORM_FILE(Mutex)
ADD_PLATFORM_FILE(AtomicOperation)
ADD_PLATFORM_FILE(Thread)
ADD_PLATFORM_FILE(CondVar)
ADD_PLATFORM_FILE(StringUtils)
ADD_PLATFORM_FILE(NumericLimits)
ADD_PLATFORM_FILE(Time)
ADD_PLATFORM_FILE(File)
ADD_PLATFORM_FILE(Debug)
ADD_PLATFORM_FILE(EnvironmentVariables)
ADD_PLATFORM_FILE(TcpSocket)
ADD_PLATFORM_FILE(TcpServerSocket)
ADD_PLATFORM_FILE(UdpSocket)
ADD_PLATFORM_FILE(Memory)
ADD_PLATFORM_FILE(Math)
ADD_PLATFORM_FILE(DynamicLibrary)
ADD_PLATFORM_FILE(Random)
ADD_PLATFORM_FILE(FileSystemIterator)
ADD_PLATFORM_FILE(Console)

ADD_CONTAINER_FILE(List)
ADD_CONTAINER_FILE(String)
ADD_CONTAINER_FILE(Comparator)
ADD_CONTAINER_FILE(BlockingQueue)
ADD_CONTAINER_FILE(Stack)
ADD_CONTAINER_FILE(Hash)
ADD_CONTAINER_FILE(Pair)
ADD_CONTAINER_FILE(Array)
ADD_CONTAINER_FILE(Queue)
ADD_CONTAINER_FILE(RingBuffer)
ADD_CONTAINER_FILE(HashTable)
ADD_CONTAINER_FILE(HashSet)
ADD_CONTAINER_FILE(ConstString)
ADD_CONTAINER_FILE(Vector)

ADD_UTIL_FILE(Guid)
ADD_UTIL_FILE(Swap)
ADD_UTIL_FILE(SmartPointer)
ADD_UTIL_FILE(Runnable)
ADD_UTIL_FILE(ScopedLock)
ADD_UTIL_FILE(CountDownLatch)
ADD_UTIL_FILE(Traits)
ADD_UTIL_FILE(ReadWriteLock)
ADD_UTIL_FILE(ThreadPool)
ADD_UTIL_FILE(IOutputStream)
ADD_UTIL_FILE(IInputStream)
ADD_UTIL_FILE(BinaryOutputStream)
ADD_UTIL_FILE(BinaryInputStream)
ADD_UTIL_FILE(SocketOutputStream)
ADD_UTIL_FILE(SocketInputStream)
ADD_UTIL_FILE(TcpSocketInputStream)
ADD_UTIL_FILE(UdpSocketInputStream)
ADD_UTIL_FILE(TcpSocketOutputStream)
ADD_UTIL_FILE(UdpSocketOutputStream)
ADD_UTIL_FILE(ScopedPointer)
ADD_UTIL_FILE(FileTraverser)
ADD_UTIL_FILE(IFileVisitor)
ADD_UTIL_FILE(FileUtils)
ADD_UTIL_FILE(TestUtils)
ADD_UTIL_FILE(Allocator)
ADD_UTIL_FILE(StaticAllocator)
ADD_UTIL_FILE(HybridAllocator)
ADD_UTIL_FILE(BinaryFileOutputStream)
ADD_UTIL_FILE(BinaryFileInputStream)
ADD_UTIL_FILE(StringOutputStream)
ADD_UTIL_FILE(StringTokenizer)
ADD_UTIL_FILE(Synchronized)

ADD_DEFINITIONS(-DCAPU_LOGGING_ENABLED=1)

IF("${TARGET_ARCH}" STREQUAL "X86_32")
    SET(FIND_LIBRARY_USE_LIB64_PATHS FALSE)
ENDIF()
IF("${TARGET_ARCH}" STREQUAL "X86_64")
    SET(FIND_LIBRARY_USE_LIB64_PATHS TRUE)
ENDIF()

ACME_ADD_DEPENDENCY(Thread)
ACME_ADD_DEPENDENCY(Rt)
ACME_ADD_DEPENDENCY(Socket)
ACME_ADD_DEPENDENCY(Net)
ACME_ADD_DEPENDENCY(IP4)
ACME_ADD_DEPENDENCY(Res)
ACME_ADD_DEPENDENCY(DynamicLibrary)

IF("${TARGET_OS}" STREQUAL "Windows")
    ACME_ADD_DEPENDENCY(Shlwapi) # file handling
    ACME_ADD_DEPENDENCY(psapi) # process handling (and Windows versions < Vista)
ENDIF()

IF("${TARGET_OS}" STREQUAL "Integrity")
    ACME_ADD_LINKER_FLAG("${INTEGRITY_LD_FILES_FOLDER}/kernel_default.ld")
    ACME_ADD_LINKER_FLAG("-kernel -lutil")
ENDIF()

ACME_ADD_DEPENDENCY(DynamicTestLibraryProject)

ACME_CPPCHECK_TARGET()
