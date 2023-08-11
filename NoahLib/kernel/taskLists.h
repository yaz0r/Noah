#pragma once

struct sTaskHeader
{
    virtual ~sTaskHeader() = default;

    void* m0_owner;
    void* m4;
    void (*m8_updateCallback)(sTaskHeader*);
    void (*mC_deleteCallback)(sTaskHeader*);
    u32 m10;
    u32 m14;
    sTaskHeader* m18_pNext;
};


struct sTaskHeaderPair : public sTaskHeader {
    sTaskHeader m1C;
};

extern sTaskHeader* spriteCallbackHead;
extern sTaskHeader* spriteCallbacksCurrentEntry;
extern sTaskHeader* spriteCallback2Var2;

extern int spritesCallback2Delay;
extern u8 spriteBytecode2ExtendedE0_Var0;

void execSpritesCallbacksList1();
void execSpritesCallbacksList2();

void registerSpriteCallback2(sTaskHeader* param_1, sTaskHeader* param_2);
void registerSpriteCallback2Sub0(sTaskHeader* param_1);
void allocateSavePointMeshDataSub0_callback(sTaskHeader* param_1);


