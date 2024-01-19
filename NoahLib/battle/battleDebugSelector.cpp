#include "noahLib.h"
#include "battle.h"
#include "battleConfig.h"
#include "battleDebugSelector.h"
#include "battleRenderContext.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "kernel/debugText.h"
#include "kernel/memory.h"

std::array<std::array<int, 3>, 4> battleDebugSelectorTableValues;
std::array<std::array<u8, 3>, 4> battleDebugSelectorIsCellEnabled;

static const std::array<const char*, 4> battleDebugSelectorLines = { {
        "SceneNo ",
        "Party ",
        "Robo ",
        "FileNo ",
} };

static const std::array<const char*, 0xC> battleDebugSelectorCharacterNames = { {
        "Fei",
        "Elly",
        "Shitan",
        "Baltho",
        "Billy",
        "Lico",
        "Emerada",
        "Chuchu",
        "Maria",
        "Shitan2",
        "Emerada2"
} };

void battleDebugSelectorInit()
{
    PutDrawEnv(&battleRenderStructs[0].m0_drawEnv);
    PutDrawEnv(&battleRenderStructs[1].m0_drawEnv);
    PutDispEnv(&battleRenderStructs[0].m5C_dispEnv);
    PutDispEnv(&battleRenderStructs[1].m5C_dispEnv);
    SetDispMask(1);

    gameState.m1D34_currentParty[0] = 0;
    gameState.m1D34_currentParty[1] = 1;
    gameState.m1D34_currentParty[2] = 2;

    // SceneNo
    battleDebugSelectorTableValues[0][0] = 3; // TODO: this was 3 in original binary

    // Party
    battleDebugSelectorTableValues[1][0] = 0;
    battleDebugSelectorTableValues[1][1] = 2;
    battleDebugSelectorTableValues[1][2] = 0xFF; // TODO: this was originally setup as 0,1,2

    // Robo
    battleDebugSelectorTableValues[2][2] = 0;
    battleDebugSelectorTableValues[2][1] = 0;
    battleDebugSelectorTableValues[2][0] = 0;

    // FileNo
    battleDebugSelectorTableValues[3][0] = 2; // TODO: this was 0 in original binary

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            battleDebugSelectorIsCellEnabled[i][j] = 1;
        }
    }

    battleDebugSelectorIsCellEnabled[0][1] = 0;
    battleDebugSelectorIsCellEnabled[0][2] = 0;
    battleDebugSelectorIsCellEnabled[3][1] = 0;
    battleDebugSelectorIsCellEnabled[3][2] = 0;
}

void battleDebugSelectorMainLoop()
{
    int cursorX = 0;
    int cursorY = 0;
    bool continueMenu = true;
    do {
        battleHandleInput();

        // double buffer the ordering table
        if (pCurrentBattleRenderStruct == &battleRenderStructs[0]) {
            pCurrentBattleOT = &battleRenderStructs[1].m70_OT;
            pCurrentBattleRenderStruct = &battleRenderStructs[1];
        }
        else {
            pCurrentBattleOT = &battleRenderStructs[0].m70_OT;
            pCurrentBattleRenderStruct = &battleRenderStructs[0];
        }

        ClearOTagR(*pCurrentBattleOT, 0x1000);

        switch (battleInputButton) {
        case 0: // right
            cursorX = cursorX + 1;
            if (2 < cursorX) {
                cursorX = 0;
            }
            if (battleDebugSelectorIsCellEnabled[cursorY][cursorX] == 0) {
                cursorX = 0;
            }
            break;
        case 0x1: // down
            cursorY = cursorY + 1;
            if (3 < cursorY) {
                cursorY = 0;
            }
            if (battleDebugSelectorIsCellEnabled[cursorY][cursorX] == 0) {
                cursorX = 0;
            }
            break;
        case 0x2: // left
            cursorX = cursorX + -1;
            if (cursorX < 0) {
                cursorX = 0;
            }
            if (battleDebugSelectorIsCellEnabled[cursorY][cursorX] == 0) {
                cursorX = 0;
            }
            break;
        case 0x3: // up
            cursorY = cursorY + -1;
            if (cursorY < 0) {
                cursorY = 3;
            }
            if (battleDebugSelectorIsCellEnabled[cursorY][cursorX] == 0) {
                cursorX = 0;
            }
            break;
        case 0x4:
            battleDebugSelectorTableValues[cursorY][cursorX]++;
            switch (cursorY) {
            case 0:
                if(battleDebugSelectorTableValues[cursorY][cursorX] >= 0x10)
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0;
                break;
            case 1:
                if (battleDebugSelectorTableValues[cursorY][cursorX] >= 0xC)
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0;
                break;
            case 2:
                if (battleDebugSelectorTableValues[cursorY][cursorX] >= 3)
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0;
                break;
            case 3:
                if (battleDebugSelectorTableValues[cursorY][cursorX] >= 0x100)
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0;
                break;
            }
            break;
        case 0x5:
            battleDebugSelectorTableValues[cursorY][cursorX]--;
            if (battleDebugSelectorTableValues[cursorY][cursorX] < 0) {
                switch (cursorY) {
                case 0:
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0xF;
                    break;
                case 1:
                case 3:
                    battleDebugSelectorTableValues[cursorY][cursorX] = 0xff;
                    break;
                case 2:
                    battleDebugSelectorTableValues[2][cursorX] = 2;
                    break;
                }
            }
            break;
        case 8:
            break;
        case 0xE:
            continueMenu = false;
            break;
        default:
            assert(0);
        }

        logToScreen("\n\n");

        for (int currentLine = 0; currentLine < 4; currentLine++) {
            logToScreen("\n\n%s", battleDebugSelectorLines[currentLine]);

            for (int column = 0; column < 3; column++) {
                if (battleDebugSelectorIsCellEnabled[currentLine][column] == 0)
                    continue;

                switch (currentLine) {
                case 0: // sceneNo
                    if ((cursorY == 0) && (column == cursorX)) {
                        logToScreen("[%d] ", battleDebugSelectorTableValues[0][0]);
                    }
                    else {
                        logToScreen("%d ", battleDebugSelectorTableValues[0][0]);
                    }
                    break;
                case 1: // party
                    if ((cursorY == 1) && (column == cursorX)) {
                        if (battleDebugSelectorTableValues[1][column] < 0xC) {
                            logToScreen("[%s] ", battleDebugSelectorCharacterNames[battleDebugSelectorTableValues[1][column]]);
                        }
                        else {
                            logToScreen("[%s] ", "");
                        }
                    }
                    else {
                        if (battleDebugSelectorTableValues[1][column] < 0xC) {
                            logToScreen("%s ", battleDebugSelectorCharacterNames[battleDebugSelectorTableValues[1][column]]);
                        }
                        else {
                            logToScreen("%s ", "");
                        }
                    }
                    break;
                case 2: // Robo
                    if ((cursorY == 2) && (column == cursorX)) {
                        switch (battleDebugSelectorTableValues[2][column]) {
                        case 0:
                            logToScreen("[Off] ");
                            break;
                        case 1:
                            logToScreen("[Nml] ");
                            break;
                        case 2:
                            logToScreen("[Bar] "); //Bar for Bara, since omni-gears are called Gear-bara in JP
                            break;
                        default:
                            assert(0);
                        }
                    }
                    else {
                        switch (battleDebugSelectorTableValues[2][column]) {
                        case 0:
                            logToScreen("Off ");
                            break;
                        case 1:
                            logToScreen("Nml ");
                            break;
                        case 2:
                            logToScreen("Bar "); //Bar for Bara, since omni-gears are called Gear-bara in JP
                            break;
                        default:
                            assert(0);
                        }
                    }
                    break;
                case 3: // fileNo
                    if ((cursorY == 3) && (column == cursorX)) {
                        if (battleDebugSelectorTableValues[3][0] < 253) {
                            logToScreen("[%d] ", battleDebugSelectorTableValues[3][0]);
                        }
                        else {
                            switch (battleDebugSelectorTableValues[3][0]) {
                            case 255:
                                logToScreen("[Event1] ");
                                break;
                            case 254:
                                logToScreen("[Event2] ");
                                break;
                            case 253:
                                logToScreen("[Event3] ");
                                break;
                            }
                        }
                        
                    }
                    else {
                        if (battleDebugSelectorTableValues[3][0] < 253) {
                            logToScreen("%d ", battleDebugSelectorTableValues[3][0]);
                        }
                        else {
                            switch (battleDebugSelectorTableValues[3][0]) {
                            case 255:
                                logToScreen("Event1 ");
                                break;
                            case 254:
                                logToScreen("Event2 ");
                                break;
                            case 253:
                                logToScreen("Event3 ");
                                break;
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }

        logToScreen("\n\n     LU       Start  to Battle");
        logToScreen("\n   LL  LR     Maru   +");
        logToScreen("\n     LD       Batsu  -");

        printDebugTextOnScreen(&pCurrentBattleRenderStruct->m70_OT[0]);
        DrawSync(0);
        VSync(0);
        PutDrawEnv(&pCurrentBattleRenderStruct->m0_drawEnv);
        PutDispEnv(&pCurrentBattleRenderStruct->m5C_dispEnv);
        DrawOTag(&pCurrentBattleRenderStruct->m70_OT[0xFFF]);
        if (!continueMenu) {
            return;
        }
    } while (true);
}

static const std::array<s8, 16> battleDebugSelectorGearMapping = { {
    0,2,3,4,5,6,9,7,8,16,15,-1,-1,-1,-1,-1
} };

static const std::array<s8, 16> battleDebugSelectorOmniGearMapping = { {
    1,2,11,12,13,14,9,7,8,11,18,-1,-1,-1,-1,-1
} };

void battleSetDebugInventory() {
    MissingCode();
}

void battleDebugSelector()
{
    battleDebugSelectorInit();
    battleDebugSelectorMainLoop();

    for (int i = 0; i < 3; i++) {
        gameState.m22B1_isOnGear[i] = 0;
        gameState.m1D34_currentParty[i] = 0xff;

        if (battleDebugSelectorTableValues[1][i] < 0xB) {
            gameState.m1D34_currentParty[i] = battleDebugSelectorTableValues[1][i];
            switch (battleDebugSelectorTableValues[2][i]) {
            case 0:
                gameState.m22B1_isOnGear[i] = 0;
                break;
            case 1: // normal gears
                gameState.m26C_party[i].mA0_partyData_gearNum = battleDebugSelectorGearMapping[i];
                gameState.m22B1_isOnGear[i] = 1;
                break;
            case 2: // omni gears
                gameState.m26C_party[i].mA0_partyData_gearNum = battleDebugSelectorOmniGearMapping[i];
                gameState.m22B1_isOnGear[i] = 1;
                break;
            default:
                assert(0);
            }
        }
    }

    setCurrentDirectory(0x20, 3);
    int fileNo = battleDebugSelectorTableValues[3][0] + 7;
    if (battleDebugSelectorTableValues[3][0] > 0xFC) {
        fileNo = battleDebugSelectorTableValues[3][0] - 0xF9;
    }

    requestedBattleConfig = battleDebugSelectorTableValues[0][0];
    if ((battleDebugSelectorTableValues[0][0] == 0xf) && (fileNo == 7)) {
        requestedBattleConfig = 3;
        battleInitVar0 = 3;
    }

    // load new battle config from file
    {
        std::vector<u8> newBattleConfig;
        resetMemoryAllocStats(2, 0);
        newBattleConfig.resize(getFileSizeAligned(fileNo));
        readFile(fileNo, newBattleConfig, 0, 0x80);
        for (int i = 0; i < 16; i++) {
            battleConfigs[i].init(newBattleConfig.begin() + i * 32);
        }
    }

    MissingCode(); // load battle music

    battleSetDebugInventory();

    for (int i = 0; i < 0xB; i++) {
        gameState.m26C_party[i].m4C_HP = 999;
        gameState.m26C_party[i].m4E_MaxHP = 999;
        gameState.m26C_party[i].m50_MP = 99;
        gameState.m26C_party[i].m52_MaxMP = 99;

        gameState.m16C0[i].m0_unlockedAbilitiesBitField[1] = 0xFFFF; // TODO: added to debug abilities
    }

    gameState.m1D30_partyMemberBitField = 0xffff;
    deinitDebugFont();
    battleTransitionEffect = requestedBattleConfig & 3;
}