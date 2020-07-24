#include "noahLib.h"
#include "gameState.h"
#include "filesystem.h"

sGameState gameState;
sGameState* pKernelGameState = nullptr;

void initModel3(int, int); // TODO: cleanup

void loadInitialGameState()
{
    setCurrentDirectory(0x10, 0);
    initModel3(2, 0);
    int size = getFileSizeAligned(3);

    {
        std::vector<u8> rawGameState;
        rawGameState.resize(size);
        readFile(3, rawGameState, 0, 0x80);
        waitReadCompletion(0);

        // NOTE: this was originally just a cast
        for (int i = 0; i < 11; i++)
        {
            gameState.m294[i].m78_partyData_gearNum = READ_LE_S8(rawGameState.begin() + 0x294 + 0xA4 * i + 0x78);
        }

        for (int i=0; i<3; i++)
        {
            gameState.m1D34_currentParty[i] = READ_LE_S8(rawGameState.begin() + 0x1D34 + i);
        }
    }

    MissingCode();
}

void initGameState()
{
    MissingCode();

    loadInitialGameState();
    waitReadCompletion(0);

    MissingCode();
}