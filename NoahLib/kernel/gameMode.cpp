#include "noahLib.h"
#include "gameMode.h"
#include "field/field.h"
#include "menus/menuHandler.h"
#include "kernel/kernelBootMenu.h"
#include "worldmap/worldmap.h"
#include "battle/battle.h"

s32 bootMode = 0;
s32 oldBootMode = 0;

std::vector<u8> currentGameModeOverlay;

struct sBootMode {
    void (*entryPoint)();
    u32 startOfZeroInit;
    u32 endOfZeroInit;
    u32 needOverlay;
};

void unimplementedGameMode() {
    //assert(0);
}

static const std::array<sBootMode, 7> gameModes = { {
    {&kernelBootMenu, 0, 0, 0}, // kernel menu
    {&fieldEntryPoint, 0, 0, 1}, // field
    {&battleEntryPoint, 0, 0, 1}, // battle
    {&worldmapEntryPoint, 0, 0, 1}, // worldmap
    {&unimplementedGameMode, 0, 0, 1}, // battling
    {&enterMenu, 0, 0, 0}, // menu
    {&unimplementedGameMode, 0, 0, 1}, // movie
} };

void setGameMode(int newGameMode) {
    bootMode = newGameMode;
    if (newGameMode != oldBootMode) {
        currentGameModeOverlay.clear();
        oldBootMode = -1;
    }
}

void bootGame(int param_1)
{
    static bool bootGameRecurse = 0;
    if (bootGameRecurse == 0) {
        MissingCode();
        int currentBootMode = bootMode;
        MissingCode();
        setGameMode(0);
    
        bootGameRecurse = 1;
        gameModes[currentBootMode].entryPoint();
        assert(bootGameRecurse == 1);
        bootGameRecurse = 0;
    }
    
    //bootGame(0); HACK: no recurion
}