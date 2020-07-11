#pragma once

int initBgfxGlue(int argc, char* argv[]);
void deleteBgfxGlue();

void StartFrame();
void EndFrame();

extern int gFrameLimit;
extern bool gCloseApp;

