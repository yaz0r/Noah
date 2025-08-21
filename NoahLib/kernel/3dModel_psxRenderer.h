#pragma once

#include "psx/gpuprims.h"
#include "kernel/3dModel.h"

extern std::vector<struct sTag*>::iterator currentModelInstanceDrawPrims;
extern u8* currentModeBlock18;
extern std::vector<SVECTOR>* currentModelBlockVertices;
extern std::vector<SVECTOR>* currentModelBlockNormals;
extern s32 fieldPolyCount;
extern s32 objectClippingMask;
extern s32 fieldPolyCount2;
extern int overrideTPageMode;
extern int initModel2Sub0Var1;
extern u8* currentModelBlockDisplayLists;
extern u8* g_currentModelBlockSubBlocks;

typedef void(*t_primRenderFunc)(u8* meshSubBlock, int count);
typedef int(*t_primInitFunc)(u8* displayList, u8* meshBlock, int initParam);

struct sPolyTypeRenderDefinition
{
    t_primRenderFunc m0_type0;
    t_primRenderFunc m4_type1;
    t_primRenderFunc m8_type2;
    t_primRenderFunc mC_type3;
    t_primRenderFunc m10_type4;
    t_primRenderFunc m14_type5;
    t_primInitFunc m18_init;
    int m1C_size;
    int m20_sizeDisplayList;
    int m24_sizeInstanceArray;
};

extern const std::array<sPolyTypeRenderDefinition, 17> polyRenderDefs;

bool submitModelForRendering(sModelBlock* param_1, std::vector<sTag*>& param_2, OTTable::iterator OT, int renderMode);
void setupOverrideTPage(uint x, uint y);
void setupOverrideClut(uint x, uint y);
void initFieldDrawEnvsSub0(int param_1, int param_2);