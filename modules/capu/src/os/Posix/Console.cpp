#include "capu/os/Console.h"

namespace capu
{
    const char_t* capu::posix::Console::Colors[] = {
        "\e[1;31m", //RED
        "\e[1;34m", // BLUE
        "\e[1;32m", // GREEN
        "\e[1;33m", // YELLOW
        "\e[1;37m", // WHITE
    };
}
