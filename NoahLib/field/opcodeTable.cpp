#include "noahLib.h"
#include "opcodeTables.h"
#include "opcodes.h"

std::array<tOpcode, 256> fieldScriptOpcodes;
std::array<tOpcode, 256> fieldScriptOpcodes_EX;

void initOpcodes()
{
    fieldScriptOpcodes[0x00] = OP_STOP;
    fieldScriptOpcodes[0x01] = OP_JUMP;
    fieldScriptOpcodes[0x02] = OP_JUMP_IF;
    fieldScriptOpcodes[0x04] = OP_4;
    fieldScriptOpcodes[0x05] = OP_CALL;
    fieldScriptOpcodes[0x07] = OP_RUN_ENTITY_SCRIPT_ASYNC;
    fieldScriptOpcodes[0x08] = OP_RUN_ENTITY_SCRIPT_UNKMODE;
    fieldScriptOpcodes[0x0A] = OP_A;
    fieldScriptOpcodes[0x0B] = OP_INIT_ENTITY_NPC;
    fieldScriptOpcodes[0x0C] = OP_UPDATE_CHARACTER_INFINITLY;
    fieldScriptOpcodes[0x0D] = OP_RETURN;
    fieldScriptOpcodes[0x16] = OP_INIT_ENTITY_PC;
    fieldScriptOpcodes[0x15] = OP_15;
    fieldScriptOpcodes[0x17] = OP_17;
    fieldScriptOpcodes[0x18] = OP_18;
    fieldScriptOpcodes[0x19] = OP_SET_ACTOR_POSITION_2D;
    fieldScriptOpcodes[0x1A] = OP_1A;
    fieldScriptOpcodes[0x1C] = OP_SET_CURRENT_ACTOR_ELEVATION;
    fieldScriptOpcodes[0x1D] = OP_SET_CURRENT_ACTOR_POSITION_3D;
    fieldScriptOpcodes[0x1F] = OP_1F;
    fieldScriptOpcodes[0x20] = OP_SET_CURRENT_ACTOR_FLAGS;
    fieldScriptOpcodes[0x21] = OP_21;
    fieldScriptOpcodes[0x23] = OP_23;
    fieldScriptOpcodes[0x26] = OP_WAIT;
    fieldScriptOpcodes[0x27] = OP_RESET_CHARACTER;
    fieldScriptOpcodes[0x2A] = OP_2A;
    fieldScriptOpcodes[0x2C] = OP_PLAY_ANIMATION;
    fieldScriptOpcodes[0x2D] = OP_GET_ACTOR_POSITION;
    fieldScriptOpcodes[0x2E] = OP_GET_ACTOR_FACING_ANGLE;
    fieldScriptOpcodes[0x31] = OP_JUMP_IF_PAD_MASK;
    fieldScriptOpcodes[0x35] = OP_SET_VAR_FROM_S16;
    fieldScriptOpcodes[0x36] = OP_SET_VAR_TRUE;
    fieldScriptOpcodes[0x37] = OP_SET_VAR_FALSE;
    fieldScriptOpcodes[0x38] = OP_SET_VAR_ADD;
    fieldScriptOpcodes[0x39] = OP_SET_VAR_SUBSTRACT;
    fieldScriptOpcodes[0x3C] = OP_SET_VAR_INC;
    fieldScriptOpcodes[0x3D] = OP_SET_VAR_DEC;
    fieldScriptOpcodes[0x43] = OP_SET_VAR_RANDOM;
    fieldScriptOpcodes[0x44] = OP_44;
    fieldScriptOpcodes[0x46] = OP_46;
    fieldScriptOpcodes[0x4A] = OP_4A;
    fieldScriptOpcodes[0x4B] = OP_4B;
    fieldScriptOpcodes[0x52] = OP_52;
    fieldScriptOpcodes[0x53] = OP_53;
    fieldScriptOpcodes[0x57] = OP_57;
    fieldScriptOpcodes[0x59] = OP_59;
    fieldScriptOpcodes[0x5A] = OP_5A;
    fieldScriptOpcodes[0x5B] = resetFieldScriptActor;
    fieldScriptOpcodes[0x5F] = OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION;
    fieldScriptOpcodes[0x60] = OP_60;
    fieldScriptOpcodes[0x61] = OP_SET_CAMERA_TARGET_OVERRIDE;
    fieldScriptOpcodes[0x63] = OP_SET_DESIRED_CAMERA_TARGET;
    fieldScriptOpcodes[0x64] = OP_64;
    fieldScriptOpcodes[0x65] = OP_SET_CAMERA_POSITION_OVERRIDE;
    fieldScriptOpcodes[0x69] = OP_SET_CURRENT_ACTOR_ROTATION;
    fieldScriptOpcodes[0x6A] = OP_SET_ACTOR_ROTATION2;
    fieldScriptOpcodes[0x6B] = OP_ROTATE_ACTOR_CLOCKWISE;
    fieldScriptOpcodes[0x6C] = OP_ROTATE_ACTOR_ANTICLOCKWISE;
    fieldScriptOpcodes[0x6D] = OP_GET_ANGLE_COS;
    fieldScriptOpcodes[0x6E] = OP_GET_ANGLE_SIN;
    fieldScriptOpcodes[0x74] = OP_74;
    fieldScriptOpcodes[0x75] = OP_PLAY_MUSIC;
    fieldScriptOpcodes[0x76] = OP_76;
    fieldScriptOpcodes[0x79] = OP_RESTORE_HP;
    fieldScriptOpcodes[0x7A] = OP_RESTORE_MP;
    fieldScriptOpcodes[0x84] = OP_IF_GAMEPROGRESS_LESS;
    fieldScriptOpcodes[0x93] = OP_ADD_ENDITY_TO_FIELD1721_LIST;
    fieldScriptOpcodes[0x99] = OP_99;
    fieldScriptOpcodes[0x9B] = OP_SET_CAMERA_INTERPOLATION_RATE;
    fieldScriptOpcodes[0x9D] = OP_9D;
    fieldScriptOpcodes[0xA0] = OP_SET_SCREEN_GEOMETRY;
    fieldScriptOpcodes[0xA3] = OP_SET_DESIRED_CAMERA_POSITION;
    fieldScriptOpcodes[0xA7] = OP_UPDATE_CHARACTER;
    fieldScriptOpcodes[0xA8] = OP_A8;
    fieldScriptOpcodes[0xAC] = OP_AC;
    fieldScriptOpcodes[0xBC] = OP_INIT_ENTITY_SCRIPT;
    fieldScriptOpcodes[0xBE] = OP_ROTATION_3D_YAW_SUBSTRACT;
    fieldScriptOpcodes[0xBF] = OP_ROTATION_3D_PITCH_ADD;
    fieldScriptOpcodes[0xC0] = OP_ROTATION_3D_PITCH_SUBSTRACT;
    fieldScriptOpcodes[0xC1] = OP_ROTATION_3D_ROLL_ADD;
    fieldScriptOpcodes[0xC2] = OP_ROTATION_3D_ROLL_SUBSTRACT;
    fieldScriptOpcodes[0xC6] = OP_INCREASE_FIELD_EXECUTION_MAX_CYCLES;
    fieldScriptOpcodes[0xCB] = OP_IF_PLAYER_IN_TRIGGER;
    fieldScriptOpcodes[0xD0] = OP_SET_DIALOG_WINDOW_PARAM;
    fieldScriptOpcodes[0xDB] = OP_DB;
    fieldScriptOpcodes[0xE5] = OP_SETUP_FOG;
    fieldScriptOpcodes[0xE6] = OP_E6;
    fieldScriptOpcodes[0xE7] = OP_E7;
    fieldScriptOpcodes[0xEF] = OP_WAIT_CAMERA;
    fieldScriptOpcodes[0xF1] = OP_SETUP_SCREEN_EFFECT1;
    fieldScriptOpcodes[0xF5] = OP_SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE3;
    fieldScriptOpcodes[0xF8] = OP_SET_FLAGS_DYNAMIC;
    fieldScriptOpcodes[0xFC] = OP_FC;
    fieldScriptOpcodes[0xFE] = OP_EXTENDED_OPCODE;

    fieldScriptOpcodes_EX[0x01] = OP_RAND_ROTATION;
    fieldScriptOpcodes_EX[0x03] = OP_SET_CURRENT_ACTOR_SCALE;
    fieldScriptOpcodes_EX[0x07] = OPX_07;
    fieldScriptOpcodes_EX[0x0D] = OP_SET_DIALOG_AVATAR;
    fieldScriptOpcodes_EX[0x0E] = OP_SET_MUSIC_PARAMS;
    fieldScriptOpcodes_EX[0x13] = OPX_13;
    fieldScriptOpcodes_EX[0x15] = OPX_15;
    fieldScriptOpcodes_EX[0x18] = OP_ADD_TO_CURRENT_PARTY;
    fieldScriptOpcodes_EX[0x1C] = OPX_1C;
    fieldScriptOpcodes_EX[0x3C] = OPX_3C;
    fieldScriptOpcodes_EX[0x42] = OP_SET_OFF_GEAR;
    fieldScriptOpcodes_EX[0x47] = OPX_47;
    fieldScriptOpcodes_EX[0x4A] = OP_LOAD_SPECIAL_2D_ANIMATION;
    fieldScriptOpcodes_EX[0x4B] = OP_WAIT_LOADING_SPECIAL_2D_ANIMATION;
    fieldScriptOpcodes_EX[0x4D] = OPX_4D;
    fieldScriptOpcodes_EX[0x50] = OPX_50;
    fieldScriptOpcodes_EX[0x52] = OPX_52;
    fieldScriptOpcodes_EX[0x54] = OPX_54;
    fieldScriptOpcodes_EX[0x80] = OPX_80;
    fieldScriptOpcodes_EX[0x81] = OPX_81;
    fieldScriptOpcodes_EX[0x82] = OPX_82;
    fieldScriptOpcodes_EX[0x86] = OPX_86;
    fieldScriptOpcodes_EX[0x8E] = OPX_8E;
    fieldScriptOpcodes_EX[0xA1] = OP_ASSIGN_GEAR;
    fieldScriptOpcodes_EX[0xA4] = OP_RESTORE_GEAR;
    fieldScriptOpcodes_EX[0xCD] = OP_GET_CURRENT_DISC_NUMBER;
    fieldScriptOpcodes_EX[0xA2] = OPX_A2;
    fieldScriptOpcodes_EX[0xE0] = OPX_E0;
}

class fakeConstructor
{
public:
    fakeConstructor()
    {
        initOpcodes();
    }
} gFakeConstructor;

