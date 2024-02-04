#include "noahLib.h"
#include "events.h"

struct sEventState {
    bool m_isOpen = false;
    bool m_isEnabled = false;
    eventFunc m_function = nullptr;

    std::thread m_thread;
};

sEventState rootCounter2;

sEventState* getEventForDesc(u32 desc) {
    switch (desc) {
    case RCntCNT2:
        return &rootCounter2;
        break;
    default:
        assert(0);
    }
    return nullptr;
}

u32 OpenEvent(u32 desc, u32 spec, u32 mode, eventFunc func) {
    sEventState* pEventState = getEventForDesc(desc);

    if (pEventState) {
        pEventState->m_isOpen = true;
        pEventState->m_function = func;
    }
    return 1;
}

u32 CloseEvent(u32 event) {
    assert(0);
    return 1;
}

u32 EnableEvent(u32 event) {
    sEventState* pEventState = getEventForDesc(event);
    if (pEventState) {
        pEventState->m_isEnabled = true;
    }
    return 1;
}

u32 SetRCnt(u32 spec, u16 target, u32 mode) {
    MissingCode();
    return 1;
}

void rootCounterThread(u32 spec) {
    while (true) {
        sEventState* pEventState = getEventForDesc(spec);
        assert(pEventState);

        pEventState->m_function();
        std::this_thread::sleep_for(std::chrono::nanoseconds(240));
    }
}

u32 StartRCnt(u32 spec) {
    sEventState* pEventState = getEventForDesc(spec);
    if (pEventState) {
        pEventState->m_thread = std::thread(rootCounterThread, spec);
    }
    return 1;
}