#pragma once

#include "noahLib.h"

void movieEntryPoint();

// Movie parameters set before entering movie mode
extern u8 movieType;      // 0=picture only, 1=picture+ADPCM, 2=ADPCM only
extern u8 movieNumber;    // which movie to play
extern u8 movieReturnMode; // game mode to return to after movie
extern u8 movieFadeParam;  // fade parameter
