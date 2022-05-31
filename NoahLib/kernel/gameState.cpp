#include "noahLib.h"
#include "gameState.h"
#include "filesystem.h"
#include "field/dialogWindows.h"

sGameState gameState;
sGameState* pKernelGameState = nullptr;

void resetMemoryAllocStats(int, int); // TODO: cleanup

void loadInitialGameState()
{
    setCurrentDirectory(0x10, 0);
    resetMemoryAllocStats(2, 0);
    int size = getFileSizeAligned(3);

    {
        std::vector<u8> rawGameState;
        rawGameState.resize(size);
        readFile(3, rawGameState, 0, 0x80);
        waitReadCompletion(0);

        for (int i = 0; i < 31; i++) {
            gameState.m0_names[i].clear();
            gameState.m0_names[i].resize(20, 0);
            for (int j = 0; j < 20/2; j++) {
                u8 character = READ_LE_S8(rawGameState.begin() + 20 * i + j * 2);
                if (character == 0xF)
                    break;
                gameState.m0_names[i][j] = character;
            }
        }

        // NOTE: this was originally just a cast
        for (int i = 0; i < 11; i++)
        {
            gameState.m294[i].m1C = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x1C);
            gameState.m294[i].m20 = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x20);
            gameState.m294[i].m24_HP = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x24);
            gameState.m294[i].m26_MaxHP = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x24);
            gameState.m294[i].m28_MP= READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x28);
            gameState.m294[i].m2A_MaxMP = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x2A);
            gameState.m294[i].m3A_Level = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x3A);
            gameState.m294[i].m3B_Level2 = READ_LE_S16(rawGameState.begin() + 0x294 + 0xA4 * i + 0x3B);
            gameState.m294[i].m78_partyData_gearNum = READ_LE_S8(rawGameState.begin() + 0x294 + 0xA4 * i + 0x78);
        }

        gameState.m1924_Gold = READ_LE_U32(rawGameState.begin() + 0x1924);

        for (int i=0; i<3; i++)
        {
            gameState.m1D34_currentParty[i] = READ_LE_U8(rawGameState.begin() + 0x1D34 + i);
        }


        gameState.m231A_fieldID = READ_LE_U16(rawGameState.begin() + 0x231A);
        gameState.m2320_worldmapMode = READ_LE_U16(rawGameState.begin() + 0x2320);
        //gameState.m231A_fieldID = 0; // override to boot in debug room
    }

    MissingCode();
}

void initGameState()
{
    MissingCode();

    loadInitialGameState();
    waitReadCompletion(0);
	dialogWindowColor[0] = 0x88;
	dialogWindowColor[1] = 0x76;
	dialogWindowColor[2] = 0x54;

    MissingCode();
}
