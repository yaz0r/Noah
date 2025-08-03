#include "noahLib.h"
#include "events.h"

enum event_class {
    EVENT_VBLANK = 0xf0000001,  // IRQ0
    EVENT_GPU = 0xf0000002,     // IRQ1
    EVENT_CDROM = 0xf0000003,   // IRQ2
    EVENT_DMA = 0xf0000004,     // IRQ3
    EVENT_RTC0 = 0xf0000005,    // IRQ4 - Timer 0
    EVENT_RTC1 = 0xf0000006,    // IRQ5 - Timer 1 or 2
    //  0xf0000007 - unused, should be Timer 2
    EVENT_CONTROLLER = 0xf0000008,  // IRQ7
    EVENT_SPU = 0xf0000009,         // IRQ9
    EVENT_PIO = 0xf000000a,         // IRQ10
    EVENT_SIO = 0xf000000b,         // IRQ8
    EVENT_CARD = 0xf0000011,
    EVENT_BU = 0xf4000001,
};

enum event_mode {
    EVENT_MODE_CALLBACK = 0x1000,
    EVENT_MODE_NO_CALLBACK = 0x2000,
};

enum event_flag {
    EVENT_FLAG_FREE = 0x0000,
    EVENT_FLAG_DISABLED = 0x1000,
    EVENT_FLAG_ENABLED = 0x2000,
    EVENT_FLAG_PENDING = 0x4000,
};

struct sEventState {
    u32 m_class = 0;
    u32 m_flags = EVENT_FLAG_FREE;
    u32 m_spec = 0;
    u32 m_mode = 0;;
    eventFunc m_function = nullptr;
};

std::vector<sEventState> g_Events(10, {});

sEventState* getEventForDesc(u32 desc) {
    for (int i = 0; i < g_Events.size(); i++) {
        if (g_Events[i].m_class == desc) {
            return &g_Events[i];
        }
    }

    return nullptr;
}

int getFreeEvCBSlot() {
    for (int i = 0; i < g_Events.size(); i++) {
        if (g_Events[i].m_flags == EVENT_FLAG_FREE)
            return i;
    }

    return -1;
}

u32 OpenEvent(u32 desc, u32 spec, u32 mode, eventFunc func) {
    int eventSlot = getFreeEvCBSlot();
    if (eventSlot == -1)
        return -1;

    sEventState* pEventState = &g_Events[eventSlot];
    pEventState->m_class = desc;
    pEventState->m_spec = spec;
    pEventState->m_mode = mode;
    pEventState->m_flags = EVENT_FLAG_DISABLED;
    pEventState->m_function = func;
    return 0xF1000000 | eventSlot;
}

u32 CloseEvent(u32 event) {
    if ((event & 0xFFFF0000) != 0xF1000000)
        return 0;
    sEventState* pEventState = &g_Events[event & 0xFFFF];
    pEventState->m_flags = EVENT_FLAG_FREE;
    return 1;
}

u32 EnableEvent(u32 event) {
    sEventState* pEventState = &g_Events[event & 0xFFFF];
    if (pEventState->m_flags) {
        pEventState->m_flags = EVENT_FLAG_FREE;
    }
    return 1;
}

struct TMR_DOTCLOCK {
    u32 m0_val;
    u32 m4_mode;
    u32 m8_max;
};

std::array<TMR_DOTCLOCK, 3> Counters;
std::array<std::thread, 3> CounterThreads;
 
u32 COUNTER_OBJ_74(u32 param_1, u32 param_2, u32 param_3)
{
    ushort uVar1;

    uVar1 = (ushort)param_2;
    if (param_1 != 0) {
        uVar1 = uVar1 | 0x10;
    }
    Counters[param_3].m4_mode = uVar1;
    return 1;
}


u32 SetRCnt(u32 spec, u16 target, u32 mode) {
    u32 counterIndex = spec & 0xFFFF;
    if (counterIndex > 2)
        return 0;

    u16 _mode2 = 0x48;
    Counters[counterIndex].m4_mode = 0;
    Counters[counterIndex].m8_max = target;

    if (counterIndex < 2) {
        if ((mode & 0x10) != 0) {
            _mode2 = 0x49;
        }
        if ((mode & 1) == 0) {
            return COUNTER_OBJ_74(mode & 0x1000, _mode2 | 0x100, counterIndex);
        }
    }
    else if ((counterIndex == 2) && ((mode & 1) == 0)) {
        _mode2 = 0x248;
    }
    u16 _mode = (u16)_mode2;
    if ((mode & 0x1000) != 0) {
        _mode = _mode | 0x10;
    }
    Counters[counterIndex].m4_mode = _mode;
    return 1;
}

void rootCounterThread(u32 spec) {
    while (true) {
        sEventState* pEventState = getEventForDesc(spec);
        assert(pEventState);

        pEventState->m_function();

        u32 counterIndex = spec & 0xFFFF;

        int baseFreq = 33800000;
        int baseFreq8 = baseFreq / 8;
        float numMili = ((float)Counters[counterIndex].m8_max) / baseFreq8;
        float numMicro = numMili * 1000 * 16 * 16 * 2;

        std::this_thread::sleep_for(std::chrono::microseconds((int)numMicro));
    }
}

u32 StartRCnt(u32 spec) {
    u32 counterIndex = spec & 0xFFFF;
    if (counterIndex > 2)
        return 0;

    CounterThreads[counterIndex] = std::thread(rootCounterThread, spec);
    return 1;
}