#pragma once

extern short screenDistortionRunning;
extern short screenDistortionFadeOut;
extern u32 screenDistrotionAllocated;

void freeScreenDistortion();
void setupScreenDistortion(int param_1);
void updateAndRenderScreenDistortion(void);
void setScreenDistortionParams(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7);
