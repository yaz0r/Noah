#include "noahLib.h"
#include "taskLists.h"

sTaskHeader* spriteCallbackHead = nullptr;
sTaskHeader* spriteCallbacksCurrentEntry = nullptr;
sTaskHeader* spriteCallbacksPreviousEntry = nullptr;
sTaskHeader* spriteCallback2Var2 = nullptr;
int spritesCallback2Delay = 0;
int spriteCallback2Var4 = 0;
u32 registerSpriteCallback2Counter2 = 0;
u8 spriteBytecode2ExtendedE0_Var0 = 0;
u32 allocateSavePointMeshDataSub0_var0 = 0;
u32 allocateSavePointMeshDataSub0_var1 = 0;
u32 registerSpriteCallback2Counter = 0;

void resetSpriteCallbacks(void)
{
    allocateSavePointMeshDataSub0_var1 = 0;
    registerSpriteCallback2Counter = 0;
    spritesCallback2Delay = 0;
    spriteCallback2Var2 = (sTaskHeader*)0x0;
    spriteCallbackHead = (sTaskHeader*)0x0;
    return;
}

void execSpritesCallbacksList1(void)
{
    spriteCallbacksCurrentEntry = spriteCallbackHead;
    while (spriteCallbacksCurrentEntry) {
        spriteCallbacksPreviousEntry = spriteCallbacksCurrentEntry;
        spriteCallbacksCurrentEntry = spriteCallbacksCurrentEntry->m18_pNext;
        if (spriteCallbacksPreviousEntry->m8_updateCallback) {
            spriteCallbacksPreviousEntry->m8_updateCallback(spriteCallbacksPreviousEntry);
        }
    }
}

void execSpritesCallbacksList2()
{
    if (spritesCallback2Delay == 0) {
        spriteCallbacksCurrentEntry = spriteCallback2Var2;
        while (spriteCallbacksCurrentEntry) {
            spriteCallbacksPreviousEntry = spriteCallbacksCurrentEntry;
            spriteCallbacksCurrentEntry = spriteCallbacksCurrentEntry->m18_pNext;
            if (spriteCallbacksPreviousEntry->m8_updateCallback) {
                spriteCallbacksPreviousEntry->m8_updateCallback(spriteCallbacksPreviousEntry);
            }
        }
    }
    else {
        if (--spritesCallback2Delay == 0) {
            spriteCallback2Var4 = 0;
        }
    }
}

void registerSpriteCallback2Sub0(sTaskHeader* param_1) {
    /*
    sTaskHeader* pPrevious = nullptr;
    sTaskHeader* pCurrent = spriteCallbackHead;
    while (pCurrent) {
        if (pCurrent->m0_owner == param_1) {
            if (((pCurrent->m14 >> 0x1E) & 1) == 0) {
                if ((pCurrent->m14 & 0x1fffffff) == (param_1->m10 & 0x1fffffff)) {
                    if (pPrevious == nullptr) {
                        spriteCallbackHead = pCurrent->m18_pNext;
                    }
                    else {
                        pPrevious->m18_pNext = pCurrent->m18_pNext;
                    }
                    if (spriteCallbacksCurrentEntry == pCurrent) {
                        spriteCallbacksCurrentEntry = pCurrent->m18_pNext;
                    }
                    registerSpriteCallback2Counter--;
                    break;
                }
            }
        }

        pPrevious = pCurrent;
        pCurrent = pCurrent->m18_pNext;
    }*/

    sTaskHeader* pCurrent = spriteCallbackHead;
    sTaskHeader* pPrevious = nullptr;
    sTaskHeader* pEntry;
    if (pCurrent) {
        do {
            pEntry = pCurrent;
            if (pEntry == param_1) {
                if (pPrevious == nullptr) {
                    spriteCallbackHead = param_1->m18_pNext;
                }
                else {
                    pPrevious->m18_pNext = param_1->m18_pNext;
                }
                if (spriteCallbacksCurrentEntry == param_1) {
                    spriteCallbacksCurrentEntry = param_1->m18_pNext;
                }
                break;
            }
            pCurrent = pEntry->m18_pNext;
            pPrevious = pEntry;
        } while (pEntry->m18_pNext);
    }

    if (pCurrent) {
        registerSpriteCallback2Counter++;
    }

    registerSpriteCallback2Counter--;
}

void registerSpriteCallback2(sTaskHeader* param_1, sTaskHeader* param_2) {
    param_2->m0_owner = param_1;
    param_2->m10 = param_2->m10 & 0xe0000000 | (registerSpriteCallback2Counter2 & 0x1fffffff);
    registerSpriteCallback2Counter2 = registerSpriteCallback2Counter2 + 1;
    param_2->m18_pNext = spriteCallbackHead;
    spriteCallbackHead = param_2;
    param_2->m8_updateCallback = nullptr;
    param_2->mC_deleteCallback = registerSpriteCallback2Sub0;
    registerSpriteCallback2Counter = registerSpriteCallback2Counter + 1;
    param_2->m14 = param_1->m10 & 0x1fffffff;
}

void allocateSavePointMeshDataSub0_callback(sTaskHeader* param_1)
{
    sTaskHeader* pCurrentHead = spriteCallback2Var2;
    sTaskHeader* pPrevious = nullptr;
    sTaskHeader* pEntry;
    if (pCurrentHead) {
        do {
            pEntry = pCurrentHead;
            if (pEntry == param_1) {
                if (pPrevious == nullptr) {
                    spriteCallback2Var2 = param_1->m18_pNext;
                }
                else {
                    pPrevious->m18_pNext = param_1->m18_pNext;
                }
                if (spriteCallbacksCurrentEntry == param_1) {
                    spriteCallbacksCurrentEntry = param_1->m18_pNext;
                }
                break;
            }
            pCurrentHead = pEntry->m18_pNext;
            pPrevious = pEntry;
        } while (pEntry->m18_pNext);
    }

    if (param_1->m14 & 0x80000000) {
        allocateSavePointMeshDataSub0_var0--;
    }
    allocateSavePointMeshDataSub0_var1--;
}

void allocateSavePointMeshDataSub0(sTaskHeader* param_1, sTaskHeader* param_2)
{
    param_2->m0_owner = param_1;
    param_2->mC_deleteCallback = allocateSavePointMeshDataSub0_callback;
    param_2->m8_updateCallback = 0;
    param_2->m18_pNext = spriteCallback2Var2;
    spriteCallback2Var2 = param_2;
    if (param_1) // hack: this wasn't in original, and it would read from pointer 0x10 in ram (that happens to be 0)
    {
        param_2->m14 = param_1->m10 & 0x1fffffff;
    }
    else {
        param_2->m14 = 0;
    }
        
    param_2->m10 = param_2->m10 & 0xe0000000 | (registerSpriteCallback2Counter2 & 0x1fffffff);
    registerSpriteCallback2Counter2++;

    if (spriteBytecode2ExtendedE0_Var0 == 0) {
        param_2->m14 &= ~0x80000000;
    }
    else {
        allocateSavePointMeshDataSub0_var0++;
        param_2->m14 |= 0x80000000;
    }
    allocateSavePointMeshDataSub0_var1++;
}

void regCallback8(sTaskHeader* param_1, void (*param_2)(sTaskHeader*))
{
    param_1->m8_updateCallback = param_2;
}

void regCallbackC(sTaskHeader* param_1, void (*param_2)(sTaskHeader*))
{
    param_1->mC_deleteCallback = param_2;
}