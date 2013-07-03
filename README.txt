---------------------------
Introduction
---------------------------

CAPU stands for Collections and Platform Utils. It is a library 
providing both an implementation of a modified C++ STL interface and a 
platform abstraction framework. This library has been designed for 
projects running on multiple hardware platforms and/or operation 
systems. 

---------------------------
C++ STL implementation
---------------------------

On multiplatform projects normally you don’t want to use the 
STL implementation shipped with the target’s compiler in order to 
avoid getting in trouble with different behaviors, error handling 
mechanisms or performance issues. 

Why we don’t use existing implementations? 
- In the embedded world you often have the requirement that C++ features 
as RTTI and exceptions are not supported by the target platform. 
Therefore we implemented our STL containers with an STL-like interface 
which works without those features but enables error management by error 
codes. 
- Furthermore we are trying to enable both static and dynamic containers 
in respect to memory management by using the same interface. So in the 
prototyping phase you are fine to use dynamically allocated memory blocks, 
when it goes to product you can become stricter without changing your code. 

At the moment we support only a subset of the known STL constructs: 
- Array (static + dynamic) 
- Queue, Blocking Queue 
- Hash table,  Hash set 
- List 
- Ring buffer 
- Stack 
- String, ConstString
- Vector

---------------------------
Platform abstraction
---------------------------

The platform abstraction itself has been designed in a way that it is easy 
to maintain and extend without any IFDEF blocks in the code itself. 
Furthermore if the platform abstraction is the same on multiple platforms 
(e.g. ARM and x86, Linux and QNX) no code duplication is needed. 

At the moment we have an abstraction for the following platforms/OS: 
- Linux (X86 32/64 bit, ARMv7) 
- QNX (X86 32 bit)
- GreenHills Integrity 
- Windows (32/64 bit) 
- Mac OSX (64 bit)
- Android (ARMv7)

The following features are available on those platforms: 
- Atomic Operations 
- Condition Variables
- Console abstraction
- Debug (Assertions)
- Dynamic Library
- Environment variables
- File, Filesystem iterator
- Math
- Memory
- Mutex 
- Numeric Limits
- Random numbers
- Semaphores
- StringUtils
- TcpSocket, UdpSocket 
- Thread 
- Time 

------------------------------------------------------------
Utils build on top of platform abstraction or STL containers
------------------------------------------------------------

- BinaryInputStream, BinaryOutputStream:
  Classes for serializing and reading/writing primitive data type from/to binary files or TCP sockets
- Logger, Console appender 
  Logger implementation with expandable appender interface
- ScopedLock, ReadWriteLock, CountDownLatch, easy synchronization
  Classes for easier synchronization in multi-thread environments
- File utils, File system traverser
  File / file system utils often needed when dealing with files
- SmartPointer, ScopedPointer
  Automatic memory management by using reference counting
- Memory allocators (static, dynamic, hybrid)
  Memory allocation abstraction, e.g. the any STL container data type could use static or dynamic memory pool inside. A hybrid allocator uses static memory and tries to get more memory dynamically, if needed.
- String tokenizer
  Utils working on strings
- Traits
  Usefuls traits when dealing with templates
- Threadpool



All mentioned components are heavily tested by using the GoogleMock/GoogleTest framework. 
Furthermore it is possible to execute the test executable on the all of the mentioned platforms.

