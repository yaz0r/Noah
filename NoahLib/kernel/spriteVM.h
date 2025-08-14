#pragma once

void SPRITE_VM_NAME(sSpriteActorCore* param_1) {
    do
    {
        if (param_1->m9E_wait != 0)
        {
            return;
        }

#if !defined(IMPLEMENT_BATTLE_SPECIFIC_CASES)
        VALIDATE_FIELD(FCT_KernelSpriteVM, 0x8002491c);
#endif

        std::span<u8>::iterator startOfOpcode = param_1->m64_spriteByteCode.value();
        u8 bytecode = READ_LE_U8(startOfOpcode);
        std::span<u8>::iterator pEndOfOpcode = startOfOpcode + 1;

#if !defined(IMPLEMENT_BATTLE_SPECIFIC_CASES)
        VALIDATE_FIELD(FCT_KernelSpriteVM, 0x80024930);
        VALIDATE_REG(FCT_KernelSpriteVM, S2, bytecode);
#endif

        if (bytecode < 0x80)
        {
            param_1->m64_spriteByteCode = pEndOfOpcode;

            int waitDelay = 0;

            if (bytecode < 0x10)
            {
                addToSpriteTransferList(param_1, param_1->m34_currentSpriteFrame + 1);
                waitDelay = (bytecode & 0xf) + 1;
            }
            else if (bytecode < 0x20)
            {
                param_1->mA8.mxB++;
                executeSpriteBytecode2Sub1(param_1);
                waitDelay = (bytecode & 0xf) + 1;
            }
            else if (bytecode < 0x30)
            {
                param_1->m34_currentSpriteFrame--;
                executeSpriteBytecode2Sub1(param_1);
                waitDelay = (bytecode & 0xf) + 1;
            }

            if (bytecode < 0x40)
            {
                waitDelay = (bytecode & 0xf) + 1;
            }

            int iVar12 = waitDelay * param_1->mAC.mx7_timeScale;
            if (iVar12 < 0) {
                iVar12 = iVar12 + 0xff;
            }
            iVar12 = iVar12 >> 8;
            if (iVar12 == 0) {
                iVar12 = 1;
            }

            param_1->m9E_wait += iVar12;
            param_1->mA8.mx16++;

            if (param_1->mA8.mx16 != 0)
            {
                return;
            }

            param_1->mA8.mx16 = 0x3F;
            return;
        }

        switch (bytecode)
        {
        case 0x80:
            param_1->mA8.mx1C = 0;
            if (param_1->m68 != nullptr) {
                param_1->m68(param_1);
                return;
            }
            if (param_1->mB0.mx0_animationId > -1) {
                spriteActorSetPlayingAnimation(param_1, param_1->mB0.mx0_animationId);
            }
            param_1->mA8.mx1C = 0;
            return;
        case 0x98:
            if (param_1->m70) {
                param_1->m9E_wait = 1;
                if ((param_1->m70->mAC.mx18 == param_1->m8D) && (param_1->m70->mA8.mx1C == 2)) {
                    return;
                }
            }
            break;
        case 0xD4:
        case 0xFA:
            assert(0);
            break;
        case 0x85:
            param_1->m64_spriteByteCode = popPointerFromAnimationStack(param_1);
            break;
        case 0xBE:
        {
            bytecode = READ_LE_U8(pEndOfOpcode);
            int iVar15 = (((int)bytecode >> 0xb & 0xfU) + 1) * (param_1->mAC.mx7_timeScale);
            int _uVar13 = bytecode & 0x1ff;
            if (false) {
                iVar15 = iVar15 + 0xff;
            }
            int sVar12 = (short)((uint)iVar15 >> 8);
            if (iVar15 >> 8 == 0) {
                sVar12 = 1;
            }
            if ((param_1->m3C & 3) == 1) {
                if (((int)bytecode < 0) && (_uVar13 != 0)) {
                    //_uVar13 = (uint) * (byte*)((int)param_1->m60 + (_uVar13 - 1));
                    assert(0);
                }
                int uVar5 = (int)bytecode >> 9 & 1;
                int uVar10 = param_1->m3C;
                int uVar6 = ((uVar5) ^ (param_1->mAC.mx2_facing)) << 3;
                int uVar14 = uVar10 & 0xfffffff7 | uVar6;
                param_1->mAC.mx3 = uVar5;
                param_1->m3C = uVar14;
                if (((int)bytecode >> 10 & 1U) == 0) {
                    uVar14 = uVar10 & 0xffffffe7 | uVar6;
                }
                else {
                    uVar14 = uVar14 | 0x10;
                }
                param_1->m3C = uVar14;
                addToSpriteTransferList(param_1, (short)_uVar13);
            }
            else {
                param_1->m34_currentSpriteFrame = (short)_uVar13;
            }
            param_1->m9E_wait += sVar12;
            param_1->m64_spriteByteCode.value() += 3;
            return;
        }
        case 0x81:
            if (((param_1->mAC.mx18) & 0xFF) == 0x3F)
            {
                assert(0);
            }
            param_1->m9E_wait = 0;
            if (param_1->m68) {
                param_1->m68(param_1);
            }
            param_1->mA8.mx1C = 1;
            return;
        case 0x82: // happen at end of walk cycle animation for player
            if (param_1->m68) {
                param_1->m68(param_1);
            }
            {
                int iVar15 = (param_1->mC_step).vy;
                spriteActorSetPlayingAnimation(param_1, param_1->mAC.mx18);
                (param_1->mC_step).vy = iVar15;
            }
            param_1->m9E_wait = 0;
            SPRITE_VM_NAME(param_1);
            return;
        case 0x86: // Happen when Fei jumps in cutscene after lahan battle. (or when jumping during gameplay)
            if (param_1->mC_step.vy < 0)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0x87: // Happen when Fei jumps in cutscene after lahan battle. (or when jumping during gameplay)
            if ((param_1->m0_position.vy.getIntegerPart()) < param_1->m84_maxY)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xA7:
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            if ((READ_LE_U8(pEndOfOpcode) & 0x80) != 0) {
                spritesCallback2Delay = (READ_LE_U8(pEndOfOpcode) & 0x7f) + 1;
                param_1->m9E_wait = param_1->m9E_wait + 1;
                return;
            }
            {
                int iVar13 = (READ_LE_U8(pEndOfOpcode) + 2) * (param_1->mAC.mx7_timeScale);
                if (false) {
                    iVar13 = iVar13 + 0xff;
                }
                int sVar11 = (short)((uint)iVar13 >> 8);
                if (iVar13 >> 8 == 0) {
                    sVar11 = 1;
                }
                param_1->m9E_wait = param_1->m9E_wait + sVar11;
            }
            return;
        case 0xCA:
            MissingCode();
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xE1: // jump in bytecode
            param_1->m64_spriteByteCode.value() += READ_LE_S16(pEndOfOpcode);
            break;
        case 0xE2: // Looks like a call
            pushBytecodePointerOnAnimationStack(param_1, param_1->m64_spriteByteCode.value() + 3);
            param_1->m64_spriteByteCode.value() += READ_LE_S16(pEndOfOpcode);
            break;
        case 0xe4: // loop number of time the last byte on the stack is
        {
            s8 cVar4 = popByteFromAnimationStack(param_1);
            if (cVar4 != 0) {
                pushByteOnAnimationStack(param_1, cVar4 - 1);
                param_1->m64_spriteByteCode.value() += READ_LE_S16(pEndOfOpcode);
            }
            else
            {
                // end of loop
                param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            }
        }
        break;

        // BATTLE SPECIFIC!
#ifdef IMPLEMENT_BATTLE_SPECIFIC_CASES
        case 0x8E:
            param_1->m64_spriteByteCode.reset();
            return;
        case 0x8F:
            param_1->m64_spriteByteCode.reset();
            param_1->mA8.mx1C = 0;
            return;
        case 0x89:
            battleSpriteOp89(param_1);
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0x8B:
            createAllDamageDisplays();
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0x97:
            MissingCode();
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0x9D:
            if ((param_1->m40 >> 0xd & 0xf) == 10) {
                battleCameraEye.vx = param_1->m0_position.vx >> 0x10;
                battleCameraEye.vy = param_1->m0_position.vy >> 0x10;
                battleCameraEye.vz = param_1->m0_position.vz >> 0x10;
            }
            else {
                battleCameraAt.vx = param_1->m0_position.vx >> 0x10;
                battleCameraAt.vy = param_1->m0_position.vy >> 0x10;
                battleCameraAt.vz = param_1->m0_position.vz >> 0x10;
            }
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xA4:
            if (param_1->m74_pTargetEntitySprite->m48_defaultAnimationbundle == nullptr) {
                assert(0);
            }
            else {
                spriteActorSetPlayingAnimation(param_1->m74_pTargetEntitySprite, READ_LE_S8(pEndOfOpcode));
            }
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xC2:
        {
            param_1->m0_position.vy = (param_1->m84_maxY - 1) * 0x10000;
            int scale = READ_LE_S8(pEndOfOpcode) * param_1->m2C_scale;
            if (scale < 0) {
                scale += 0xFFF;
            }
            scale >>= 0xC;
            if ((param_1->mAC.mx2_facing) == 0) {
                scale = -scale;
            }
            param_1->mA0.vx = param_1->m74_pTargetEntitySprite->m0_position.vx.getIntegerPart() + scale;
            param_1->mA0.vy = 0;
            param_1->mA0.vz = param_1->m74_pTargetEntitySprite->m0_position.vx.getIntegerPart();
            battleSpriteOp89(param_1);
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        }
        case 0xC3:
        case 0xEC:
        case 0xF9:
            battleSpriteEffect(param_1, READ_LE_S8(pEndOfOpcode), startOfOpcode + 2);
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xC8:
        {
            MissingCode();
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        }
        case 0xE3:
            assert(0);
            param_1->m74_pTargetEntitySprite->mAC.mx18 = 0x3F;
            setCurrentAnimationPtr(param_1->m74_pTargetEntitySprite, param_1->m64_spriteByteCode.value() + READ_LE_S16(param_1->m64_spriteByteCode.value() + 1));
            break;
        case 0xE8: // Battle sprite effect
            MissingCode();
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        case 0xF8: // switch for battle
        {
            u8 attack = battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m18_damageType
                [(param_1->m74_pTargetEntitySprite->mAC.mx0_entityIdUpper2bit) << 2 |
                param_1->m74_pTargetEntitySprite->mA8.mx1E_entityId_bottom2bit];

            bool bVar6;
            u8 attackType = READ_LE_U8(pEndOfOpcode + 2) & 0x7F;
            switch (attackType) {
            case 0:
                bVar6 = attack == 0;
                break;
            case 4:
                bVar6 = attack == 4;
                break;
            case 5:
                bVar6 = attack == 5;
                break;
            default:
                assert(0);
            }

            if (READ_LE_U8(pEndOfOpcode + 2) & 0x80) {
                bVar6 = !bVar6;
            }
            if (bVar6) {
                param_1->m64_spriteByteCode.value() += READ_LE_S16(pEndOfOpcode);
            }
            else {
                param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            }
            break;
        }
        case 0xfb:
            battleSpriteOpcode_FB(param_1, param_1->m64_spriteByteCode.value()[3] << 1, param_1->m64_spriteByteCode.value() + READ_LE_S16(pEndOfOpcode));
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
#endif

        default:
            executeSpriteBytecode2Extended(param_1, bytecode, pEndOfOpcode);
            param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
            break;
        }
    } while (1);
}
