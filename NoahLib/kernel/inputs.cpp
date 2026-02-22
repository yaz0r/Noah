#include "noahLib.h"
#include "inputs.h"
#include "playTime.h"

#include "SDL_gamepad.h"
#include "SDL_keyboard.h"

bool isGameInFocus();

std::array<sGameController, 2> newPadButtonForScripts;

u16 newPadButtonForDialogs;
u16 newPadButtonForDialogs2;
u16 newPadButtonForField;
u16 newPadButtonForField2;

struct sInputReplayEntry
{
    u16 m0_newPadButtonForScripts0[16];
    u16 m20_newPadButtonForScripts1[16];
    u16 m40_newPadButtonForDialogs[16];
    u16 m60_newPadButtonForDialogs2[16];
    u16 m80_newPadButtonForField[16];
    u16 mA0_newPadButtonForField2[16];
} inputReplayBuffer;

int inputReplayPosition = 0;
int inputReplayPosition2 = 0;
int inputOverflowed = 0;
int inputReplayOffset = 0;

extern bool bIsDebuggerViewEnabled;

bool isControllerConnected(int port) {
    return true;
}

void getInputDuringVsync(void)
{
    MissingCode();

#ifndef SHIPPING_BUILD
    if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false))
    {
        bIsDebuggerViewEnabled = !bIsDebuggerViewEnabled;
    }
#endif

    if (!isGameInFocus())
        return;

    static SDL_Gamepad* controller = nullptr;
    if (controller == nullptr)
    {
        int num_joysticks;
        if (SDL_JoystickID* joysticks = SDL_GetGamepads(&num_joysticks)) {
            for (int i = 0; i < num_joysticks; ++i) {
                if (SDL_IsGamepad(joysticks[i])) {
                    controller = SDL_OpenGamepad(joysticks[i]);
                    if (controller) {
                        break;
                    }
                    else {
                        fprintf(stderr, "Could not open game controller %i: %s\n", i, SDL_GetError());
                    }
                }
            }
        }
    }

    u16 buttonMask = 0;
    if (controller)
    {
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) ? controllerButtons::L1 : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER) ? controllerButtons::R1 : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_NORTH) ? controllerButtons::TRIANGLE : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_EAST) ? controllerButtons::INTERACT : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_SOUTH) ? controllerButtons::CROSS : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_WEST) ? controllerButtons::JUMP : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_BACK) ? controllerButtons::SELECT : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_START) ? controllerButtons::START : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_DPAD_UP) ? controllerButtons::UP : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_DPAD_RIGHT) ? controllerButtons::RIGHT : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_DPAD_DOWN) ? controllerButtons::DOWN : 0;
        buttonMask |= SDL_GetGamepadButton(controller, SDL_GAMEPAD_BUTTON_DPAD_LEFT) ? controllerButtons::LEFT : 0;


        /*
                static bool pressRight = false;
                if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                {
                    pressRight = true;
                }
                if (pressRight)
                {
                    buttonMask |= 0x2000;
                }
                */
    }
    //else
    {
        const bool* keyState = SDL_GetKeyboardState(NULL);

        for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
        {
            if (keyState[i])
            {
                switch (i)
                {
                case SDL_SCANCODE_Z:
                    buttonMask |= controllerButtons::CROSS; // CROSS
                    break;
                case SDL_SCANCODE_X:
                    buttonMask |= controllerButtons::INTERACT; // CIRCLE
                    break;
                case SDL_SCANCODE_A:
                    buttonMask |= controllerButtons::JUMP; // SQUARE
                    break;
                case SDL_SCANCODE_S:
                    buttonMask |= controllerButtons::TRIANGLE; // TRIANGLE
                    break;
                case SDL_SCANCODE_UP:
                    buttonMask |= controllerButtons::UP;
                    break;
                case SDL_SCANCODE_RIGHT:
                    buttonMask |= controllerButtons::RIGHT;
                    break;
                case SDL_SCANCODE_DOWN:
                    buttonMask |= controllerButtons::DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    buttonMask |= controllerButtons::LEFT;
                    break;
                case SDL_SCANCODE_RETURN:
                    buttonMask |= controllerButtons::START;
                    break;
                default:
                    break;
                }
            }
        }
    }

    static u32 previousDownButtons = 0;
    static u32 numVsyncButtonHeld = 0;

    newPadButtonForScripts[0].m0_buttons = buttonMask;
    newPadButtonForDialogs = (newPadButtonForScripts[0].m0_buttons ^ previousDownButtons) & newPadButtonForScripts[0].m0_buttons;

    if (newPadButtonForDialogs != 0) {
        numVsyncButtonHeld = 0;
    }
    newPadButtonForField = newPadButtonForScripts[0].m0_buttons;

    if (numVsyncButtonHeld < 0x20)
    {
        previousDownButtons++;
        newPadButtonForField = newPadButtonForDialogs;
    }
    else if (g_playTimeInVsync & 3)
    {
        newPadButtonForField = newPadButtonForDialogs;
    }

    MissingCode();

    previousDownButtons = newPadButtonForScripts[0].m0_buttons;

}

void saveInputs()
{
    ushort uVar1;
    ushort uVar2;
    ushort uVar3;
    ushort uVar4;
    short sVar5;
    uint uVar6;

    sVar5 = newPadButtonForScripts[1].m0_buttons;
    uVar4 = newPadButtonForField2;
    uVar3 = newPadButtonForField;
    uVar2 = newPadButtonForDialogs2;
    uVar1 = newPadButtonForDialogs;
    if (inputReplayPosition < 0x10) {
        uVar6 = inputReplayPosition2 & 0xf;
        inputReplayPosition2 = inputReplayPosition2 + 1;
        inputReplayPosition = inputReplayPosition + 1;
        inputReplayBuffer.m0_newPadButtonForScripts0[uVar6] = newPadButtonForScripts[0].m0_buttons;
        inputReplayBuffer.m20_newPadButtonForScripts1[uVar6] = sVar5;
        inputReplayBuffer.m40_newPadButtonForDialogs[uVar6] = uVar1;
        inputReplayBuffer.m60_newPadButtonForDialogs2[uVar6] = uVar2;
        inputReplayBuffer.m80_newPadButtonForField[uVar6] = uVar3;
        inputReplayBuffer.mA0_newPadButtonForField2[uVar6] = uVar4;
    }
    else {
        inputOverflowed = 1;
    }
}

int loadInputFromVSyncBuffer()
{
    int iVar1;
    uint uVar2;

    if (inputReplayPosition == 0) {
        iVar1 = 0;
    }
    else {
        uVar2 = inputReplayOffset & 0xf;
        newPadButtonForScripts[0].m0_buttons = inputReplayBuffer.m0_newPadButtonForScripts0[uVar2];
        newPadButtonForScripts[1].m0_buttons = inputReplayBuffer.m20_newPadButtonForScripts1[uVar2];
        newPadButtonForDialogs = inputReplayBuffer.m40_newPadButtonForDialogs[uVar2];
        newPadButtonForDialogs2 = inputReplayBuffer.m60_newPadButtonForDialogs2[uVar2];
        newPadButtonForField = inputReplayBuffer.m80_newPadButtonForField[uVar2];
        newPadButtonForField2 = inputReplayBuffer.mA0_newPadButtonForField2[uVar2];
        inputReplayOffset = inputReplayOffset + 1;
        iVar1 = inputReplayPosition;
        inputReplayPosition = inputReplayPosition + -1;
    }
    return iVar1;
}


void resetInputs()
{
    MissingCode();

    inputOverflowed = 0;
    inputReplayPosition = 0;
    inputReplayPosition2 = 0;
    inputReplayOffset = 0;
    newPadButtonForDialogs = 0;
    newPadButtonForDialogs2 = 0;
    newPadButtonForField = 0;
    newPadButtonForField2 = 0;

    MissingCode();

    newPadButtonForScripts[0].m0_buttons = 0;
    newPadButtonForScripts[1].m0_buttons = 0;

    MissingCode();
}

int getInputOverflowed() {
    return inputOverflowed;
}