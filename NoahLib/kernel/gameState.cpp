#include "noahLib.h"
#include "gameState.h"
#include "filesystem.h"
#include "field/dialogWindows.h"
#include "battle/battle.h"

sGameState gameState;
sGameState* pKernelGameState = nullptr;

void resetMemoryAllocStats(int, int); // TODO: cleanup

void loadInitialGameState()
{
    setCurrentDirectory(0x10, 0);
    resetMemoryAllocStats(2, 0);
    int size = getFileSizeAligned(3);

    {
        sLoadableDataRaw rawGameState;
        rawGameState.resize(size);
        readFile(3, rawGameState, 0, 0x80);
        waitReadCompletion(0);

        gameState.deserialize(rawGameState.getRawData());

        for (int i = 0; i < 31; i++) {
            gameState.m0_names[i].clear();
            gameState.m0_names[i].resize(20, 0);
            for (int j = 0; j < 20/2; j++) {
                u8 character = READ_LE_S8(rawGameState.getRawData().begin() + 20 * i + j * 2);
                if (character == 0xF)
                    break;
                gameState.m0_names[i][j] = character;
            }
        }
    }

    MissingCode();
}

void initGameState()
{
    battleInitVar1 = 1;
    MissingCode();

    loadInitialGameState();
    waitReadCompletion(0);
	dialogWindowColor[0] = 0x88;
	dialogWindowColor[1] = 0x76;
	dialogWindowColor[2] = 0x54;
    dialogBattleWindowAbr = 2;
}
