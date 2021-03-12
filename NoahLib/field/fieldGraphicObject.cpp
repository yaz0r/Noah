#include "noahLib.h"
#include "fieldGraphicObject.h"

void OP_21_sub(sFieldEntitySub4* param_1, int param_2)
{
    param_1->mAC = param_1->mAC & 0xfff8007f | (param_2 & 0xfff) << 7;
}
