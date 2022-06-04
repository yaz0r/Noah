#include "noahLib.h"
#include "field/fieldGraphicObject.h"
#include "worldmap.h"



s32 worldmapMode0_taskPlayer_init(int param_1)
{
    sSpriteActor* pSprite = createSpriteActor(worldmapPartySprites[0], 0x100, 0x1e0, 0x140, 0x100, 0x40);
    gWorldmapState->m0[param_1].m4C = pSprite;
    spriteActorSetPlayingAnimation(pSprite, 0);
    setGraphicEntityScale(&pSprite->m0_spriteActorCore, 0x1800);

    MissingCode();
    return 1;
}

s32 worldmapMode0_taskPlayer_update(int param_1)
{
    MissingCode();
    return 1;
}