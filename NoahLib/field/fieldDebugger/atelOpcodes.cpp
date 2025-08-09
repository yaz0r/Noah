#include "noahLib.h"
#include "atelOpcodes.h"
#include "imgui.h"

std::array<sOpcodeInfo, 256> m_opcode;
std::array<sOpcodeInfo, 256> m_extendedOpcode;

void initOpcodeTable()
{
	m_opcode[0x00]
		.setName("STOP")
		.setOpcodeBreakExecutionFlow()
		.end();

	m_opcode[0x01]
		.setName("JUMP")
		.addArgumentJumpLocation()
		.end();

	// special case IF

	m_opcode[0x03]
		.setName("SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE2")
		.addArgumentDialogId()
		.addArgumentByte()
		.end();

	m_opcode[0x04]
		.end();

	m_opcode[0x05]
		.setName("CALL")
		.addArgumentFunctionEntryPoint("Call destination")
		.setComment("Call a sub function")
		.end();

	m_opcode[0x07]
		.setName("RUN_ENTITY_SCRIPT_ASYNC")
		.setComment("ScriptNumber is & 0x1F, other param in upper bits")
		.addArgumentCharacter("FieldScriptEntity")
		.addArgumentByte("ScriptId")
		.end();

	m_opcode[0x08]
		.setName("RUN_ENTITY_SCRIPT_UNKMODE")
		.setComment("ScriptNumber is & 0x1F, other param in upper bits")
		.addArgumentCharacter("FieldScriptEntity")
		.addArgumentByte("ScriptId")
		.end();

	m_opcode[0x09]
		.setName("RUN_ENTITY_SCRIPT_BLOCKING")
		.setComment("ScriptNumber is & 0x1F, other param in upper bits")
		.addArgumentCharacter("FieldScriptEntity")
		.addArgumentByte("ScriptId")
		.end();

	m_opcode[0xA]
		.setName("CALL_IF_IN_TRIGGER")
		.addArgumentTriggerIndex()
		.addArgumentFunctionEntryPoint()
		.end();

	m_opcode[0xB]
		.setName("INIT_ENTITY_NPC")
		.addArgumentU16OrVar("Id")
		.setComment("Init as NPC")
		.end();

	m_opcode[0x0C]
		.setName("UPDATE_CHARACTER_INFINITLY")
		.setOpcodeBreakExecutionFlow()
		.end();

	m_opcode[0x0D]
		.setName("RETURN")
		.setOpcodeBreakExecutionFlow()
		.end();

	m_opcode[0xE]
		.setComment("Doesn't do anything")
		.end();

	m_opcode[0xF]
		.setComment("Doesn't do anything")
		.end();

    // 0x10 is dynamic

	m_opcode[0x12]
        .setName("CUSTOM_FIELD_TRANSITION")
		.addArgumentU16OrVar("fieldId")
		.addArgumentU16OrVar("param")
		.addArgumentU16OrVar("transitionMode")
		.addArgumentU16OrVar("fadeInLength")
		.end();

	m_opcode[0x14]
		.setName("DISABLE_RANDOM_BATTLE")
		.end();

	m_opcode[0x15]
		.end();

	m_opcode[0x16]
		.setName("INIT_ENTITY_PC")
		.addArgumentU16OrVar("Id")
		.setComment("Init as PC")
		.end();

	m_opcode[0x17]
        .setName("SETUP_MOVEMENT_BOUNDING_ZONE")
		.addArgumentS16OrVar(0x80, "X0")
		.addArgumentS16OrVar(0x40, "Y0")
		.addArgumentS16OrVar(0x20, "X1")
		.addArgumentS16OrVar(0x10, "Y1")
		.addArgumentS16OrVar(0x08, "X2")
		.addArgumentS16OrVar(0x04, "Y2")
		.addArgumentS16OrVar(0x02, "X3")
		.addArgumentS16OrVar(0x01, "Y3")
		.addSignControlByte()
		.end();

	m_opcode[0x18]
        .setName("SET_BOUNDING_VOLUME")
		.addArgumentByte("VolumeX")
		.addArgumentByte("VolumeZ")
		.addArgumentByte("VolumeY")
		.addArgumentByte("CollisionRadius")
		.end();

	m_opcode[0x19]
		.setName("SET_ACTOR_POSITION_2D")
		.addArgumentS16OrVar(0x80, "X")
		.addArgumentS16OrVar(0x40, "Y")
		.addSignControlByte()
		.end();

	m_opcode[0x1A]
        .setName("SET_ENTITY_WALKMESH_LAYER")
		.addArgumentByte()
		.end();

	m_opcode[0x1C]
		.setName("SET_CURRENT_ACTOR_ELEVATION")
		.addArgumentS16OrVar(0x80, "Y")
		.addSignControlByte()
		.end();

	m_opcode[0x1D]
		.setName("SET_CURRENT_ACTOR_POSITION_3D")
		.addArgumentS16("X")
		.addArgumentS16("Z")
		.addArgumentS16("Y")
		.end();

	m_opcode[0x1F]
		.addArgumentByte()
		.end();

	m_opcode[0x20]
		.setName("SET_CURRENT_ACTOR_FLAGS")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x21]
        .setName("SET_TIME_SCALE")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x22]
		.setName("MAKE_VISIBLE_ACTIVE")
		.end();

	m_opcode[0x23]
		.setName("MAKE_INVISIBLE_ACTIVE")
		.end();

	m_opcode[0x24]
        .setName("MAKE_VISIBLE_BY_ID")
		.addArgumentCharacter()
		.end();

	m_opcode[0x25]
        .setName("MAKE_INVISIBLE_BY_ID")
		.addArgumentCharacter()
		.end();

	m_opcode[0x26]
		.setName("WAIT")
		.addArgumentU16OrVar("Amount")
		.end();

	m_opcode[0x27]
		.setName("RESET_CHARACTER")
		.addArgumentCharacter()
		.end();

	m_opcode[0x28]
        .setName("ENABLE_VM_FOR_CHARACTER")        
		.addArgumentCharacter()
		.end();

	m_opcode[0x29]
		.setName("DELETE_ENTITY")
		.addArgumentCharacter()
		.end();

	m_opcode[0x2A]
        .setName("DISABLE_DIALOG_ACTIVATION")
        .setComment("Toggle flag 0x20000 on current field script entity")
		.end();

	m_opcode[0x2C]
		.setName("PLAY_ANIMATION")
		.addArgumentByte()
		.end();

	m_opcode[0x2D]
		.setName("GET_ACTOR_POSITION")
		.addArgumentCharacter()
		.addArgumentVarIndex()
		.addArgumentVarIndex()
		.addArgumentVarIndex()
		.end();

	m_opcode[0x2E]
		.setName("GET_ACTOR_FACING_ANGLE")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x31]
		.setName("JUMP_IF_PAD_MASK")
		.addArgumentU16()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0x35]
		.setName("SET_VAR_FROM_S16")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x36]
		.setName("SET_VAR_TRUE")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x37]
		.setName("SET_VAR_FALSE")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x38]
		.setName("SET_VAR_ADD")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x39]
		.setName("SET_VAR_ADD")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x3A]
		.setName("SET_VARBIT_TRUE")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x3B]
		.setName("SET_VARBIT_FALSE")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x3C]
		.setName("SET_VAR_INC")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x3D]
		.setName("SET_VAR_DEC")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x3E]
		.setName("SET_VAR_AND")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x43]
		.setName("SET_VAR_RANDOM")
		.addArgumentVarIndex()
		.end();

	m_opcode[0x44]
		.addArgumentU16OrVar("Angle")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x46]
		.end();

	m_opcode[0x47]
		.setName("WALK_AND_CHANGE_FIELD")
		.addArgumentByte()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.setComment("TODO: the first byte is unused?")
		.end();

	m_opcode[0x4A]
		.setName("SPRITE_WALK_TO_POSITION_AND_WAIT")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_opcode[0x4B]
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x4C]
		.setComment("Decoding seems to depend from current execution slots flags?!")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x52]
		.setComment("Walk to destination related (somewhat dynamic!)")
		.addArgumentCharacter()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x53]
        .setName("FOLLOW_CHARACTER")
		.addArgumentCharacter()
        .addArgumentU16OrVar()
		.end();

	m_opcode[0x56]
        .setName("WORLDMAP")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addArgumentS16OrVar(0x10)
		.addSignControlByte()
		.end();

	// opcode 0x57 is dynamic

    m_opcode[0x58]
        .setName("SET_ANGLE_AXIS_ROTATION")
        .addArgumentU16OrVar("angle")
        .addArgumentByte("axis")
        .end();

	m_opcode[0x59]
		.setName("WALK_RANDOM_DIRECTION")
		.end();

	m_opcode[0x5A]
        .setName("RESET_FIELD_SCRIPT_ACTOR")
		.end();

	m_opcode[0x5B]
        .setName("RESET_FIELD_SCRIPT_ACTOR_FOREVER")
        .setOpcodeBreakExecutionFlow()
		.end();

    m_opcode[0x5C]
        .addArgumentU16OrVar()
        .end();

	m_opcode[0x5D]
		.setName("PLAY_ANIMATION_EX")
		.addArgumentByte()
		.end();

	m_opcode[0x5E]
		.setName("WAIT_ANIMATION_END")
		.end();

	m_opcode[0x5F]
		.setName("SET_CURRENT_ACTOR_CARDINAL_DIRECTION")
		.addArgumentByte()
		.end();

	m_opcode[0x60]
		.end();

	m_opcode[0x61]
		.setName("SET_CAMERA_TARGET_OVERRIDE")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x63]
		.setName("SET_DESIRED_CAMERA_TARGET")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x64]
		.end();

	m_opcode[0x65]
		.setName("SET_CAMERA_POSITION_OVERRIDE")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x67]
        .setName("SET_CURRENT_ACTOR_ROTATION_WORLD_RELATIVE")
		.addArgumentCharacter()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x69]
		.setName("SET_CURRENT_ACTOR_ROTATION")
		.addArgumentU16OrVar("Angle")
		.end();

	m_opcode[0x6A]
		.setName("SET_ACTOR_ROTATION2")
		.addArgumentU16OrVar("Angle")
		.end();

	m_opcode[0x6B]
		.setName("ROTATE_ACTOR_CLOCKWISE")
		.addArgumentU16OrVar("Angle")
		.end();

	m_opcode[0x6C]
		.setName("ROTATE_ACTOR_ANTICLOCKWISE")
		.addArgumentU16OrVar("Angle")
		.end();

	m_opcode[0x6D]
		.setName("GET_ANGLE_COS")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x6E]
		.setName("GET_ANGLE_SIN")
		.addArgumentVarIndex()
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0x6F]
		.setName("ROTATE_TO_ACTOR")
		.addArgumentCharacter()
		.end();

	m_opcode[0x70]
		.setName("ROTATE_AWAY_FROM_ACTOR")
		.addArgumentCharacter()
		.end();

	m_opcode[0x72]
		.setComment("Wait for something")
		.addArgumentU16OrVar()
		.end();

    m_opcode[0x74]
        .setName("PLAY_SOUND_EFFECT")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x75]
		.setName("PLAY_MUSIC")
		.addArgumentU16OrVar("MusicId")
		.end();

	m_opcode[0x76]
		.end();

	m_opcode[0x77]
		.end();

	m_opcode[0x79]
		.setName("RESTORE_HP")
		.end();

	m_opcode[0x7A]
		.setName("RESTORE_MP")
		.end();

	m_opcode[0x7C]
		.addArgumentS16OrVar(0x80)
		.addSignControlByte()
		.end();

	m_opcode[0x80]
		.addArgumentByte()
		.addArgumentByte()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x84]
		.setName("IF_GAMEPROGRESS_LESS")
		.addArgumentU16OrVar()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0x85]
		.setName("IF_GAMEPROGRESS_GREATER")
		.addArgumentU16OrVar()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0x86]
		.setName("IF_GAMEPROGRESS_EQUAL")
		.addArgumentU16OrVar()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0x87]
		.setName("SET_GAMEPROGRESS")
		.addArgumentU16OrVar("NewGameProgress")
		.end();

	m_opcode[0x8B]
		.setName("IF_OBJECT_IN_INVENTORY")
		.addArgumentU16OrVar()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0x8C]
		.setName("ADD_OBJECT_TO_INVENTORY")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x8D]
		.setName("REMOVE_OBJECT_FROM_INVENTORY")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x8F]
		.setName("ADD_GOLD")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x91]
		.setName("IF_CHARACTER_IN_PARTY")
		.addArgumentByte()
		.addArgumentJumpLocation()
		.end();

    m_opcode[0x92]
        .end();

	m_opcode[0x93]
		.setName("ADD_ENDITY_TO_MECHA_LIST")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x97]
		.setName("DOLLY_SET")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x98]
		.setName("CHANGE_FIELD_WHEN_READY2")
        .addArgumentU16OrVar("fieldId")
        .addArgumentU16OrVar("param")
		.end();

	m_opcode[0x99]
		.end();

	m_opcode[0x9A]
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x9B]
		.setName("SET_CAMERA_INTERPOLATION_RATE")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0x9C]
		.setName("WAIT_DIALOG")
		.end();

	m_opcode[0x9D]
		.addArgumentU16OrVar()
		.addArgumentByte()
		.end();

	m_opcode[0xA0]
		.setName("SET_SCREEN_GEOMETRY")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

    m_opcode[0xA1]
        .setName("SET_DOLLY_STOP")
        .addArgumentU16OrVar()
        .end();

	m_opcode[0xA2]
        .setName("WAIT_FOR_CAMERA_ANIMATION_FLAG_CLEAR")
		.setComment("Wait for mask to clear")
		.addArgumentByte()
		.end();

	m_opcode[0xA3]
		.setName("SET_DESIRED_CAMERA_POSITION")
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_opcode[0xA4]
		.addArgumentS16OrVar(0x80)
		.addSignControlByte()
		.end();

	m_opcode[0xA7]
		.setName("UPDATE_CHARACTER")
		.end();

	m_opcode[0xA8]
		.setName("RAND_MODULO")
		.addArgumentVarIndex()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xA9]
		.setName("SETUP_MULTICHOICE")
		.setComment("Lower nibble is number of entries")
		.addArgumentByte()
		.end();

	m_opcode[0xAA]
        .setName("ROTATE_CAMERA_RELATIVE")
		.addArgumentByte()
		.end();

	m_opcode[0xAC]
		.setName("SETUP_CAMERA_INTERPOLATION_STEPS")
		.addArgumentByte()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xB3]
		.setName("START_FADE_IN")
		.addArgumentU16OrVar("Duration")
		.end();

	m_opcode[0xB4]
		.setName("START_FADE_TO_BLACK")
		.addArgumentU16OrVar("Duration")
		.end();

	m_opcode[0xB5]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

    m_opcode[0xB6]
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

	m_opcode[0xB7]
		.end();

	m_opcode[0xB8]
        .setName("ENABLE_CAMERA_HEIGHT_CHECK")
		.end();

	m_opcode[0xB9]
		.setName("IF_CHARACTER_IN_AVAILABLE_PARTY")
		.addArgumentPC()
		.addArgumentJumpLocation()
		.end();

	m_opcode[0xBA]
		.setName("ADD_TO_AVAILABLE_PARTY")
		.addArgumentByte()
		.end();

	m_opcode[0xBB]
		.setName("REMOVE_FROM_AVAILABLE_PARTY")
		.addArgumentByte()
		.end();

	m_opcode[0xBC]
		.setName("INIT_ENTITY_SCRIPT")
		.end();

	m_opcode[0xBD]
		.setName("ROTATION_3D_YAW_ADD")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xBE]
		.setName("ROTATION_3D_YAW_SUBSTRACT")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xBF]
		.setName("ROTATION_3D_PITCH_ADD")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xC0]
		.setName("ROTATION_3D_PITCH_SUBSTRACT")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xC1]
		.setName("ROTATION_3D_ROLL_ADD")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xC2]
		.setName("ROTATION_3D_ROLL_SUBSTRACT")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xC4]
        .setName("OPEN_DOOR")
		.addArgumentByte()
		.end();

	m_opcode[0xC5]
        .setName("CLOSE_DOOR")
		.addArgumentByte()
		.end();

	m_opcode[0xC6]
		.setName("INCREASE_FIELD_EXECUTION_MAX_CYCLES")
		.end();

	m_opcode[0xC7]
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xCB]
		.setName("IF_PLAYER_IN_TRIGGER")
		.addArgumentByte("triggerId")
		.addArgumentJumpLocation()
		.end();

	m_opcode[0xCD]
		.setComment("Set flag 0x800000 on current entity")
		.end();

	m_opcode[0xCE]
		.setComment("Clear flag 0x800000 on current entity")
		.end();

	m_opcode[0xD0]
		.setName("SET_DIALOG_WINDOW_PARAM")
		.addArgumentU16OrVar("x")
		.addArgumentU16OrVar("y")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xD2]
		.setName("SHOW_DIALOG_WINDOW_MODE0")
		.addArgumentDialogId()
		.addArgumentByte()
		.end();

	m_opcode[0xD4]
		.setName("SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0")
		.addArgumentCharacter()
		.addArgumentDialogId()
		.addArgumentByte()
		.end();

    m_opcode[0xD6]
        .addArgumentU16OrVar()
        .end();

	m_opcode[0xD7]
        .setName("SET_OBJECT_SWIVEL_ON_X_AXIS")
		.addArgumentU16OrVar()
		.end();

    m_opcode[0xD8]
        .setName("SET_OBJECT_SWIVEL_ON_Y_AXIS")
        .addArgumentU16OrVar()
        .end();

    m_opcode[0xD9]
        .setName("SET_OBJECT_SWIVEL_ON_Z_AXIS")
        .addArgumentU16OrVar()
        .end();

    m_opcode[0xDA]
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .end();

	m_opcode[0xDB]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xE1]
		.setName("VRAM_OPERATION")
		.addArgumentS16OrVar(0x80, "destX")
		.addArgumentS16OrVar(0x40, "destY")
		.addArgumentS16OrVar(0x20, "sourceX")
		.addArgumentS16OrVar(0x10, "sourceY")
		.addArgumentS16OrVar(0x08, "width")
		.addArgumentS16OrVar(0x04, "height")
		.addSignControlByte()
		.end();

    m_opcode[0xE5]
        .setName("SETUP_FOG")
        .addArgumentU16OrVar("NearColor_R")
        .addArgumentU16OrVar("NearColor_G")
        .addArgumentU16OrVar("NearColor_B")
        .addArgumentU16OrVar("FarColor_R")
        .addArgumentU16OrVar("FarColor_G")
        .addArgumentU16OrVar("FarColor_B")
        .addArgumentU16OrVar("FogNear")
        .addArgumentU16OrVar("FogFar")
        .end();

	m_opcode[0xE6]
        .setName("SET_CAMERA_LIMITS")
		.addArgumentS16()
		.addArgumentS16()
		.addArgumentS16()
		.addArgumentS16()
		.end();

	m_opcode[0xE7]
        .setName("SET_BACKGROUND_CLEAR_COLOR")
		.addArgumentU16OrVar("r")
		.addArgumentU16OrVar("g")
		.addArgumentU16OrVar("b")
		.end();

	m_opcode[0xEC]
        .setName("ORBIT")
		.addArgumentByte("mode")
		.addArgumentS16OrVar(0x80, "yOrbit")
		.addArgumentS16OrVar(0x40, "angle")
		.addArgumentS16OrVar(0x20, "magnitude")
		.addSignControlByte()
		.addArgumentVarIndex("OutputX")
		.addArgumentVarIndex("OutputY")
		.addArgumentVarIndex("OutputZ")
		.setComment("Mode0: cameraTargetOverride, Mode1: cameraTarget, Mode2: cameraPositionOverride, Mode3: desiredCameraPosition")
		.end();

	m_opcode[0xEE]
		.addArgumentByte()
		.addArgumentByte()
		.end();

	m_opcode[0xEF]
		.setName("WAIT_CAMERA")
		.addArgumentU16OrVar()
		.end();

	m_opcode[0xF1]
		.setName("SETUP_SCREEN_EFFECT1")
		.addArgumentU16OrVar("mode")
		.addArgumentU16OrVar("b")
		.addArgumentU16OrVar("g")
		.addArgumentU16OrVar("r")
		.addArgumentU16OrVar("duration")
		.end();

	m_opcode[0xF2]
		.setName("SETUP_CAMERA_SHAKE")
		.addArgumentU16OrVar("x")
		.addArgumentU16OrVar("z")
		.addArgumentU16OrVar("y")
		.addArgumentU16OrVar("duration")
		.end();

	m_opcode[0xF3]
		.setComment("Compute 3d params between 2 positions")
		.addArgumentVarIndex("atanOutput")
		.addArgumentVarIndex("")
		.addArgumentVarIndex("distanceOutput")
		.end();

	m_opcode[0xF4]
		.setName("CLOSE_CURRENT_ACTOR_DIALOG")
		.addArgumentByte()
		.end();

	m_opcode[0xF5]
		.setName("SHOW_DIALOG_WINDOW_MODE3")
		.addArgumentDialogId()
		.addArgumentByte()
		.end();

	m_opcode[0xF6]
		.addArgumentByte()
		.end();

    m_opcode[0xF7]
        .setName("SET_ENCOUNTER_DATA")
        .addArgumentVarIndex("encounterTime")
        .addArgumentVarIndex("encounterTimersCount")
        .end();

	m_opcode[0xF8]
		.setName("SET_FLAGS_DYNAMIC")
		.addArgumentByte("mode")
		.addArgumentU16("FlagValue")
		.end();

    m_opcode[0xF9]
        .addArgumentCharacter()
        .end();

	m_opcode[0xFC]
		.addArgumentCharacter()
		.addArgumentDialogId()
		.addArgumentByte()
		.end();

    m_opcode[0xFF]
        .end();
}

void initExtendedOpcodeTable()
{
	m_extendedOpcode[0x1]
		.setName("RAND_ROTATION")
		.end();

	m_extendedOpcode[0x2]
        .setName("IF_CHARACTER_IN_VIEW")
		.addArgumentCharacter()
		.addArgumentJumpLocation()
		.end();

	m_extendedOpcode[0x3]
		.setName("SET_CURRENT_ACTOR_SCALE")
		.addArgumentU16OrVar()
		.end();

    m_extendedOpcode[0x4]
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x7]
		.addArgumentByte()
		.end();

    m_extendedOpcode[0x8]
        .setName("SET_3D_SCALE")
        .addArgumentU16OrVar("scaleX")
        .addArgumentU16OrVar("scaleY")
        .addArgumentU16OrVar("scaleZ")
        .end();

    m_extendedOpcode[0x9]
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0xA]
        .setName("SET_FLAG_BIT_FIELD")
        .addArgumentU16()
        .end();

    m_extendedOpcode[0xC]
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .addArgumentU16()
        .end();

	m_extendedOpcode[0xD]
		.setName("SET_DIALOG_PORTRAIT")
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0xE]
		.setName("SET_MUSIC_PARAMS")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0xF]
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x10]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x13]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x15]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x18]
		.setName("ADD_TO_CURRENT_PARTY")
		.setComment("Triggers async loading")
		.addArgumentByte()
		.end();

    m_extendedOpcode[0x17]
        .setName("ROTATE_CHARACTER_TO_CHARACTER")
        .addArgumentCharacter()
        .addArgumentCharacter()
        .end();

	m_extendedOpcode[0x19]
		.setName("REMOVE_FROM_CURRENT_PARTY")
		.addArgumentByte()
		.end();

	m_extendedOpcode[0x1A]
		.setName("FINALIZE_PARTY_CHARACTER_LOADING")
		.setComment("Wait for loading to finish")
		.end();

	m_extendedOpcode[0x1C]
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

    m_extendedOpcode[0x1E]
        .setName("SWITCH_MAP_TO_GEARS")
        .addArgumentByte()
        .end();

	m_extendedOpcode[0x23]
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addArgumentS16OrVar(0x10)
		.addArgumentS16OrVar(0x08)
		.addArgumentS16OrVar(0x04)
		.addSignControlByte()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x24]
		.setComment("Wait for something about player X/Z coordinates and other 2 PC")
		.end();

	m_extendedOpcode[0x26]
		.setName("SETUP_SCREEN_DISTORTION")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	// 0x27 is dynamic

    m_extendedOpcode[0x3A]
        .setName("SET_PARTY_FRAME_MASK")
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x3B]
		.setName("CLEAR_PARTY_FRAME_MASK")
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x3C]
        .setName("MECHA_PLAY_ANIMATION")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

    m_extendedOpcode[0x3D]
        .setName("SET_MECHA_MATRIX_ELEMENTS_0")
        .addArgumentS16OrVar(0x80)
        .addArgumentS16OrVar(0x40)
        .addArgumentS16OrVar(0x20)
        .addArgumentS16OrVar(0x10)
        .addSignControlByte()
        .end();

    m_extendedOpcode[0x3E]
        .setName("SET_MECHA_MATRIX_ELEMENTS_1")
        .addArgumentS16OrVar(0x80)
        .addArgumentS16OrVar(0x40)
        .addArgumentS16OrVar(0x20)
        .addArgumentS16OrVar(0x10)
        .addSignControlByte()
        .end();

    m_extendedOpcode[0x3F]
        .setName("SETUP_MECHA_BACK_COLOR")
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x41]
		.setName("SET_ON_GEAR")
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x42]
		.setName("SET_OFF_GEAR")
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x45]
		.setName("SET_OVERRIDE_ANIMATION")
		.setComment("Set field mE6 of current entity")
		.addArgumentByte()
		.end();

    m_extendedOpcode[0x47]
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x4A]
		.setName("LOAD_SPECIAL_2D_ANIMATION")
		.setComment("Load a file from disk and assign it to current scriptActor (from folder 4 + 1914)")
		.addArgumentU16OrVar("FileId")
		.end();

	m_extendedOpcode[0x4B]
		.setName("WAIT_LOADING_SPECIAL_2D_ANIMATION")
		.setComment("Wait for loading completion")
		.end();

	m_extendedOpcode[0x4D]
		.setName("PLAY_SPECIAL_2D_ANIMATION")
		.addArgumentByte()
		.end();

	m_extendedOpcode[0x4E]
		.setName("FREE_SPECIAL_2D_ANIMATION")
		.end();

	m_extendedOpcode[0x4F]
        .setName("ENABLE_MENU")
		.end();

	m_extendedOpcode[0x50]
        .setName("DISABLE_MENU")
		.end();

	m_extendedOpcode[0x51]
		.setName("ENABLE_COMPASS")
		.end();

	m_extendedOpcode[0x52]
		.setName("DISABLE_COMPASS")
		.end();

	m_extendedOpcode[0x53]
		.setName("ENABLE_PLAYER_CONTROLS")
		.end();

	m_extendedOpcode[0x54]
        .setName("START_CINEMATIC")
		.end();

    m_extendedOpcode[0x55]
        .setName("OPEN_NORMAL_MENU")
        .end();

    m_extendedOpcode[0x56]
        .setName("OPEN_PARTY_MENU")
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x57]
        .setName("OPEN_LOAD_GAME_MENU")
		.end();

    m_extendedOpcode[0x56]
        .setName("OPEN_RENAME_MENU")
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x59]
        .setName("OPEN_STORE_MENU")
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x5A]
		.addArgumentU16OrVar()
		.end();

    m_extendedOpcode[0x5B]
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x5D]
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

    // opcode 0xFE57 is dynamic

	m_extendedOpcode[0x5E]
		.setName("SET_TRANSPARENCY_MODE")
		.addArgumentU16OrVar()
		.end();


    m_extendedOpcode[0x5F]
        .addArgumentByte()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x60]
		.setName("SET_2D_BACKGROUND")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x61]
		.end();

    m_extendedOpcode[0x62]
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x64]
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x65]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x66]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x69]
		.addArgumentVarIndex()
		.addArgumentCharacter()
		.end();

    m_extendedOpcode[0x6A]
        .setName("SET_LINK_OT")
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0x6B]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentCharacter()
		.end();

    m_extendedOpcode[0x74]
        .setName("OPX_DEBUG_PRINT_FLAG_VALUE")
        .addArgumentVarIndex()
        .end();

    // 0x77 is dynamic

	m_extendedOpcode[0x80]
		.addArgumentU16()
		.addArgumentU16()
		.addArgumentU16()
		.addArgumentU16()
		.addArgumentU16()
		.addArgumentU16()
		.addArgumentU16()
		.end();

	m_extendedOpcode[0x81]
		.addArgumentS16OrVar(0x80)
		.addArgumentS16OrVar(0x40)
		.addArgumentS16OrVar(0x20)
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x82]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x83]
		.setName("SET_BOOTMODE")
		.addArgumentU16("Flags")
		.setComment("&0x80: reload new game state, &0x7F: boot mode")
		.end();

	m_extendedOpcode[0x85]
		.setName("GET_VIDEO_PLAYBACK_FRAME")
		.addArgumentVarIndex("Destination")
		.end();

	m_extendedOpcode[0x86]
		.setName("PLAY_VIDEO")
		.addArgumentByte()
		.end();

	m_extendedOpcode[0x87]
		.setName("WAIT_MENU")
		.end();

    m_extendedOpcode[0x88]
        .setName("SETUP_MECHA_FIELD_ARGS")
        .addArgumentS16OrVar(0x80)
        .addArgumentS16OrVar(0x40)
        .addArgumentS16OrVar(0x20)
        .addArgumentS16OrVar(0x10)
        .addArgumentS16OrVar(0x8)
        .addArgumentS16OrVar(0x4)
        .addArgumentS16OrVar(0x2)
        .addArgumentS16OrVar(0x1)
        .addSignControlByte()
        .end();

	m_extendedOpcode[0x8C]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x8E]
        .setName("SET_SCREEN_BOUNDS_PADDING")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x8F]
        .setName("RESET_PARTICLE_CONFIGS")
		.addArgumentCharacter()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.addArgumentU16OrVar()
		.end();

	m_extendedOpcode[0x90]
        .setName("SETUP_PARTICLE")
		.addArgumentU16OrVar("slot")
		.addArgumentU16OrVar("maxParticles")
		.addArgumentU16OrVar("sWait")
		.addArgumentU16OrVar("eWait")
		.end();

	m_extendedOpcode[0x91]
        .setName("SETUP_PARTICLE_POSITION")
		.addArgumentS16OrVar(0x80, "sPosX")
		.addArgumentS16OrVar(0x40, "sPosY")
		.addArgumentS16OrVar(0x20, "sPosZ")
		.addArgumentS16OrVar(0x10, "ePosX")
		.addArgumentS16OrVar(0x8, "ePosY")
		.addArgumentS16OrVar(0x4, "ePosZ")
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x92]
        .setName("SETUP_LIFE1")
		.addArgumentS16OrVar(0x80, "speed")
        .addArgumentS16OrVar(0x40, "gravityX")
        .addArgumentS16OrVar(0x20, "gravityY")
        .addArgumentS16OrVar(0x10, "gravityZ")
		.addArgumentS16OrVar(0x8, "sRange")
		.addArgumentS16OrVar(0x4, "eRange")
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x93]
        .setName("SETUP_LIFE2")
		.addArgumentU16OrVar("psWait")
		.addArgumentU16OrVar("peWait")
		.addArgumentU16OrVar("shape")
		.addArgumentU16OrVar("flags1")
		.addArgumentU16OrVar("flags2")
		.end();

	m_extendedOpcode[0x94]
        .setName("SETUP_SCALE")
		.addArgumentS16OrVar(0x80, "scaleX")
        .addArgumentS16OrVar(0x40, "scaleY")
        .addArgumentS16OrVar(0x20, "scaleOffsetX")
		.addArgumentS16OrVar(0x10, "scaleOffsetY")
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x95]
        .setName("SETUP_PARTICLE_COLOR")
		.addArgumentS16OrVar(0x80,"R")
		.addArgumentS16OrVar(0x40,"G")
		.addArgumentS16OrVar(0x20,"B")
		.addArgumentS16OrVar(0x10, "offsetR")
		.addArgumentS16OrVar(0x8, "offsetG")
		.addArgumentS16OrVar(0x4, "offsetB")
		.addSignControlByte()
		.end();

	m_extendedOpcode[0x96]
		.setName("CREATE_PARTICLE_EFFECT_FOR_CURRENT_ACTOR")
		.end();

	m_extendedOpcode[0x97]
        .setName("STOP_PARTICLE_EFFECT")
		.addArgumentByte()
		.end();

    m_extendedOpcode[0x99]
        .addArgumentByte()
        .end();

    m_extendedOpcode[0x9B]
        .setName("TRANSITION_EFFECT_1")
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x9C]
        .setName("TRANSITION_EFFECT_2")
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x9D]
        .setName("TRANSITION_EFFECT_3")
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x9E]
        .setName("SET_DRAW_ENV_CLIP")
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();

    m_extendedOpcode[0x9F]
        .setName("TOGGLE_FRAME_LOCK")
        .addArgumentByte()
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0xA1]
		.setName("ASSIGN_GEAR")
		.addArgumentU16OrVar("Character")
		.addArgumentU16OrVar("Gear")
		.end();

	m_extendedOpcode[0xA2]
		.setComment("Wait something")
		.end();

	m_extendedOpcode[0xA4]
		.setName("RESTORE_GEAR")
		.end();

	m_extendedOpcode[0xA5]
        .setName("SETUP_PARTICLE_ANGLE")
		.addArgumentU16OrVar()
		.addArgumentU16OrVar("flags")
		.addArgumentU16OrVar("angle")
		.end();

    m_extendedOpcode[0xB5]
        .end();

    m_extendedOpcode[0xB8]
        .addArgumentByte()
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0xBD]
		.addArgumentU16OrVar()
		.addArgumentU16OrVar("unused?")
		.addArgumentU16OrVar("unused?")
		.end();

    m_extendedOpcode[0xBF]
        .setName("SETUP_BATTLING")
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .addArgumentU16OrVar("GearId_0")
        .addArgumentU16OrVar("GearId_1")
        .addArgumentU16OrVar()
        .addArgumentU16OrVar()
        .end();
    
    m_extendedOpcode[0xC8]
        .addArgumentS16OrVar(0x80)
        .addArgumentS16OrVar(0x40)
        .addArgumentS16OrVar(0x20)
        .addArgumentS16OrVar(0x10)
        .addArgumentS16OrVar(0x8)
        .addArgumentS16OrVar(0x4)
        .addArgumentS16OrVar(0x2)
        .addArgumentS16OrVar(0x1)
        .addSignControlByte()
        .end();

    m_extendedOpcode[0xCE]
        .setName("SET_MAX_MECHA")
        .addArgumentU16OrVar()
        .end();

	m_extendedOpcode[0xCD]
		.setName("GET_CURRENT_DISC_NUMBER")
		.addArgumentVarIndex()
		.end();

	m_extendedOpcode[0xE0]
        .setName("SET_PAUSE_DISABLED")
		.addArgumentByte()
		.end();
}
