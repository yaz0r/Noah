#pragma once

#include "psx/gpuprims.h"

extern std::array<u16, 81> worldmapGrid;
extern std::array<u16, 81> worldmapGrid2;

extern u32 worldmapGridUpdateMask;
extern s32 worldmapWaterWave;

extern std::array<u8*, 256> worldmapChunks;

void allocateWorldmapData(void);
void streamWorldmap0(void);
void streamWorldmap1(void);
int isWorldStreamingDone(void);
int loadWorldmapChunk(void);
void setupInitialGrid(VECTOR* param_1);
void updateWorldmapGrids(VECTOR* param_1);
void setWorldmapGridUpdateMask(VECTOR* param_1);

void worldmapDotInit(void);
void worldmapGroundPrepareRenderingTable(VECTOR* param_1);
void drawWorldmapGround(std::vector<sTag>& param_1, std::vector<POLY_FT3>::iterator& param_2, VECTOR* param_3);