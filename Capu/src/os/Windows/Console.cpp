#include "capu/os/Windows/Console.h"
#include "capu/os/Mutex.h"


namespace capu
{
    const uint8_t capu::os::Console::Colors[] = {
        0x0C, // RED
        0x01, // BLUE
        0x0A, // GREEN
        0x0E, // YELLOW
        0x0F, // WHITE
        0x0B, // AQUA
    };

    HANDLE os::Console::m_event = INVALID_HANDLE_VALUE;
    os::Mutex os::Console::interruptMutex;

    capu::status_t os::Console::ReadOneCharacter(HANDLE fileHandle, char& buffer)
    {
        char readBuffer;
        DWORD bytesRead = 0;
        const DWORD numberOfBytesToRead = 1;
        const BOOL ret = ReadFile(fileHandle, &readBuffer, numberOfBytesToRead, &bytesRead, NULL);
        if (ret)
        {
            if (0 == bytesRead)
            {
                return CAPU_EOF;
            }
            buffer = readBuffer;
            return CAPU_OK;
        }
        else
        {
            const DWORD error = GetLastError();
            if (ERROR_INVALID_HANDLE == error)
            {
                return CAPU_EOF;
            }
            else
            {
                return CAPU_ERROR;
            }
        }
    }

    HANDLE os::Console::GetInterruptEventHandle()
    {
        return m_event;
    }

    void os::Console::SetEventHandle(HANDLE eventHandle)
    {
        m_event = eventHandle;
    }

    void os::Console::InitializeInterruptEvent()
    {
        HANDLE previouslyCreatedEventHandle = GetInterruptEventHandle();
        if (previouslyCreatedEventHandle == INVALID_HANDLE_VALUE)
        {
            HANDLE createdEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
            SetEventHandle(createdEventHandle);
        }
    }

}

