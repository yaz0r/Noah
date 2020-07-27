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
        .setComment("Test against 3d quad (in 2d)")
        .addArgumentOffsetToQuadCoordinates()
        .addArgumentJumpLocation()
        .end();

    m_opcode[0xB]
        .setName("INIT_ENTITY_NPC")
        .addArgumentImmediateOrVar("Id")
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

    m_opcode[0x14]
        .setName("DISABLE_RANDOM_BATTLE")
        .end();

    m_opcode[0x15]
        .end();

    m_opcode[0x16]
        .setName("INIT_ENTITY_PC")
        .addArgumentImmediateOrVar("Id")
        .setComment("Init as PC")
        .end();

    m_opcode[0x18]
        .addArgumentByte()
        .addArgumentByte()
        .addArgumentByte()
        .addArgumentByte()
        .end();

    m_opcode[0x19]
        .setName("SET_ACTOR_POSITION_2D")
        .addArgumentS16("X")
        .addArgumentS16("Y")
        .addSignControlByte()
        .end();

    m_opcode[0x1C]
        .setName("SET_CURRENT_ACTOR_ELEVATION")
        .addArgumentS16("Y")
        .addSignControlByte()
        .end();

    m_opcode[0x1D]
        .setName("SET_CURRENT_ACTOR_POSITION_3D")
        .addArgumentRawS16("X")
        .addArgumentRawS16("Y")
        .addArgumentRawS16("Z")
        .end();

    m_opcode[0x1F]
        .addArgumentByte()
        .end();

    m_opcode[0x20]
        .setName("SET_CURRENT_ACTOR_FLAGS")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x21]
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x22]
        .setComment("Set current fieldEntity m58_flags &= ~0x20")
        .end();

    m_opcode[0x23]
        .setComment("Set current fieldEntity m58_flags |= 0x20")
        .end();

    m_opcode[0x24]
        .addArgumentCharacter()
        .end();

    m_opcode[0x25]
        .addArgumentCharacter()
        .end();

    m_opcode[0x26]
        .setName("WAIT")
        .addArgumentImmediateOrVar("Amount")
        .end();

    m_opcode[0x28]
        .addArgumentCharacter()
        .end();

    m_opcode[0x2A]
        .end();

    m_opcode[0x2C]
        .setName("PLAY_ANIMATION")
        .addArgumentByte()
        .end();

    m_opcode[0x31]
        .addArgumentU16()
        .addArgumentJumpLocation()
        .end();

    m_opcode[0x35]
        .setName("SET_VAR_FROM_S16")
        .addArgumentVarIndex()
        .addArgumentS16()
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
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x39]
        .setName("SET_VAR_ADD")
        .addArgumentVarIndex()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x3A]
        .setName("SET_VARBIT_TRUE")
        .addArgumentVarIndex()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x3B]
        .setName("SET_VARBIT_FALSE")
        .addArgumentVarIndex()
        .addArgumentS16()
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

    m_opcode[0x46]
        .end();

    m_opcode[0x47]
        .setName("WALK_AND_CHANGE_FIELD")
        .addArgumentByte()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .setComment("TODO: the first byte is unused?")
        .end();

    m_opcode[0x4A]
        .setName("SPRITE_WALK_TO_POSITION_AND_WAIT")
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    // opcode 0x57 is dynamic

    m_opcode[0x5A]
        .setName("OP_5A")
        .end();

    m_opcode[0x5B]
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

    m_opcode[0x63]
        .setName("SET_DESIRED_CAMERA_TARGET")
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x64]
        .end();

    m_opcode[0x65]
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x69]
        .setName("SET_CURRENT_ACTOR_ROTATION")
        .addArgumentImmediateOrVar("Angle")
        .end();

    m_opcode[0x6B]
        .setName("ROTATE_ACTOR_CLOCKWISE")
        .addArgumentImmediateOrVar("Angle")
        .end();

    m_opcode[0x6C]
        .setName("ROTATE_ACTOR_ANTICLOCKWISE")
        .addArgumentImmediateOrVar("Angle")
        .end();

    m_opcode[0x6F]
        .setName("ROTATE_TO_ACTOR")
        .addArgumentCharacter()
        .end();

    m_opcode[0x72]
        .setComment("Wait for something")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x74]
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x75]
        .setName("PLAY_MUSIC")
        .addArgumentImmediateOrVar("MusicId")
        .end();

    m_opcode[0x79]
        .setComment("Backup data 1 of all PC")
        .end();

    m_opcode[0x7A]
        .setComment("Backup data 2 of all PC")
        .end();

    m_opcode[0x7C]
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x80]
        .addArgumentByte()
        .addArgumentByte()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x87]
        .setName("SET_GAMEPROGRESS")
        .addArgumentImmediateOrVar("NewGameProgress")
        .end();

    m_opcode[0x8C]
        .setName("ADD_TO_GAMESTATE_ARRAYS")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x8F]
        .setName("ADD_GOLD")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x91]
        .setName("IF_CHARACTER_IN_PARTY")
        .addArgumentByte()
        .addArgumentJumpLocation()
        .end();

    m_opcode[0x98]
        .setName("CHANGE_FIELD_WHEN_READY")
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x99]
        .end();

    m_opcode[0x9B]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x9C]
        .setName("WAIT_DIALOG")
        .end();

    m_opcode[0x9D]
        .addArgumentImmediateOrVar()
        .addArgumentByte()
        .end();

    m_opcode[0xA0]
        .addArgumentVarIndex()
        .addArgumentVarIndex()
        .addArgumentVarIndex()
        .end();

    m_opcode[0xA2]
        .setComment("Wait for mask to clear")
        .addArgumentByte()
        .end();

    m_opcode[0xA3]
        .setName("SET_DESIRED_CAMERA_POSITION")
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0xA7]
        .setName("UPDATE_CHARACTER")
        .end();

    m_opcode[0xA9]
        .addArgumentByte()
        .end();

    m_opcode[0xAA]
        .addArgumentByte()
        .end();

    m_opcode[0xAC]
        .addArgumentByte()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xB3]
        .setName("START_FADE_IN")
        .addArgumentImmediateOrVar("Duration")
        .end();

    m_opcode[0xB4]
        .setName("START_FADE_TO_BLACK")
        .addArgumentImmediateOrVar("Duration")
        .end();

    m_opcode[0xB5]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
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
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xBE]
        .setName("ROTATION_3D_YAW_SUBSTRACT")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xBF]
        .setName("ROTATION_3D_PITC_ADD")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xC0]
        .setName("ROTATION_3D_PITCH_SUBSTRACT")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xC1]
        .setName("ROTATION_3D_ROLL_ADD")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xC2]
        .setName("ROTATION_3D_ROLL_SUBSTRACT")
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xC4]
        .setName("REMOVE_FROM_AVAILABLE_PARTY")
        .addArgumentByte()
        .end();

    m_opcode[0xC5]
        .addArgumentByte()
        .end();

    m_opcode[0xC6]
        .end();

    m_opcode[0xD0]
        .setName("SET_DIALOG_WINDOW_PARAM")
        .addArgumentImmediateOrVar("x")
        .addArgumentImmediateOrVar("y")
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xD2]
        .setName("SHOW_DIALOG_WINDOW_MODE0")
        .addArgumentDialogId()
        .addArgumentByte()
        .end();

    m_opcode[0xE7]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xEC]
        .addArgumentByte()
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .addArgumentVarIndex()
        .addArgumentVarIndex()
        .addArgumentVarIndex()
        .end();

    m_opcode[0xEE]
        .addArgumentByte()
        .addArgumentByte()
        .end();

    m_opcode[0xEF]
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xF1]
        .setName("SETUP_SCREEN_EFFECT1")
        .addArgumentImmediateOrVar("mode")
        .addArgumentImmediateOrVar("b")
        .addArgumentImmediateOrVar("g")
        .addArgumentImmediateOrVar("r")
        .addArgumentImmediateOrVar("duration")
        .end();

    m_opcode[0xF3]
        .setComment("Compute 3d params between 2 positions")
        .addArgumentVarIndex("atanOutput")
        .addArgumentVarIndex("")
        .addArgumentVarIndex("distanceOutput")
        .end();

    m_opcode[0xF4]
        .addArgumentByte()
        .end();

    m_opcode[0xF6]
        .addArgumentByte()
        .end();

    m_opcode[0xF5]
        .setName("SHOW_DIALOG_WINDOW_MODE3")
        .addArgumentDialogId()
        .addArgumentByte()
        .end();
}

void initExtendedOpcodeTable()
{
    m_extendedOpcode[0x7]
        .addArgumentByte()
        .end();

    m_extendedOpcode[0xD]
        .setName("SET_DIALOG_AVATAR")
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0xE]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0xF]
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_extendedOpcode[0x10]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x18]
        .setName("ADD_TO_CURRENT_PARTY")
        .setComment("Triggers async loading")
        .addArgumentByte()
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
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_extendedOpcode[0x3B]
        .setName("CLEAR_PARTY_FRAME_MASK")
        .addArgumentImmediateOrVar()
        .addArgumentCharacter()
        .end();

    m_extendedOpcode[0x41]
        .setName("SET_ON_GEAR")
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x42]
        .setName("SET_OFF_GEAR")
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x4A]
        .setName("LOAD_SPECIAL_2D_ANIMATION")
        .setComment("Load a file from disk and assign it to current scriptActor (from folder 4 + 1914)")
        .addArgumentImmediateOrVar("FileId")
        .end();

    m_extendedOpcode[0x4B]
        .setName("WAIT_LOADING_SPECIAL_2D_ANIMATION")
        .setComment("Wait for loading completion")
        .end();

    m_extendedOpcode[0x4D]
        .setComment("Clear some flags in current field actor")
        .addArgumentByte()
        .end();

    m_extendedOpcode[0x4E]
        .setName("FREE_SPECIAL_2D_ANIMATION")
        .end();

    m_extendedOpcode[0x4F]
        .end();

    m_extendedOpcode[0x50]
        .end();

    m_extendedOpcode[0x53]
        .end();

    m_extendedOpcode[0x54]
        .end();

    m_extendedOpcode[0x5A]
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x5E]
        .setName("SET_TRANSPARENCY_MODE")
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x64]
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x6B]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentCharacter()
        .end();

    m_extendedOpcode[0x87]
        .setComment("Wait for something")
        .end();

    m_extendedOpcode[0x8E]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x8F]
        .addArgumentCharacter()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x90]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x91]
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_extendedOpcode[0x97]
        .addArgumentByte()
        .end();

    m_extendedOpcode[0xA1]
        .setName("ASSIGN_GEAR")
        .addArgumentImmediateOrVar("Character")
        .addArgumentImmediateOrVar("Gear")
        .end();

    m_extendedOpcode[0xA2]
        .setComment("Wait something")
        .end();

    m_extendedOpcode[0xA4]
        .setComment("Backup something in list of 20 in gamestate")
        .end();
}
