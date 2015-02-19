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

    bool_t os::Console::IsKeyboardEventAvailable(HANDLE input)
    {
        const uint32_t numberOfInputEventsToRead = 1u;
        DWORD numberOfReadEvents = 0;
        do
        {
            INPUT_RECORD inputRecord;
            ZeroMemory(&inputRecord, sizeof(INPUT_RECORD));
            BOOL status = PeekConsoleInput(input, &inputRecord, numberOfInputEventsToRead, &numberOfReadEvents);
            if (status && numberOfReadEvents> 0)
            {
                if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
                {
                    // next available event is keyboard event
                    return true;
                }
                else
                {
                    // prune non keyboard event
                    ReadConsoleInput(input, &inputRecord, numberOfInputEventsToRead, &numberOfReadEvents);
                }
            }
        } while (numberOfReadEvents > 0);
        // no more events left, did not find any keyboard events
        return false;
    }

    capu::char_t os::Console::ReadOneCharacter(HANDLE fileHandle)
    {
        char_t buffer;
        DWORD bytesRead = 0;
        const DWORD numberOfBytesToRead = 1;
        int ret = ReadFile(fileHandle, &buffer, numberOfBytesToRead, &bytesRead, NULL);
        if (ret > 0)
        {
            return buffer;
        }
        return -1;
    }

    HANDLE os::Console::GetEventHandle()
    {
        return m_event;
    }

    void os::Console::SetEventHandle(HANDLE eventHandle)
    {
        m_event = eventHandle;
    }

}

