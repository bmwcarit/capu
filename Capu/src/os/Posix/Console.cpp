#include "capu/os/Console.h"

namespace capu
{
    namespace posix
    {
        const char_t* Console::Colors[] = {
            "\e[1;31m", //RED
            "\e[1;34m", // BLUE
            "\e[1;32m", // GREEN
            "\e[1;33m", // YELLOW
            "\e[1;37m", // WHITE
            "\e[1;36m", // AQUA
        };

        int32_t Console::pipeDescriptorsForInterruption[2] = {-1,-1};
        Mutex Console::interruptMutex;
    }
}
