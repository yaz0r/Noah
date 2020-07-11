#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"
#include "isoManager.h"
#include "fileSystem.h"

#include "../imgui_club/imgui_memory_editor/imgui_memory_editor.h"

bool noahInit(int argc, char* argv[])
{
    initBgfxGlue(argc, argv);
    c_isoManager::init();

    //////////////////////////////////////////////////////////////////////////
    // start the origin code

    /*
    stuff
    */

    initCDAndFileSystem(&fatFileTableBuffer, &fatDirectoryTableBuffer, -1);

    /*
    stuff
    */

    return true;
}

void decompress(std::vector<u8>::iterator& inputStream, std::vector<u8>& output)
{
    int totalSize = READ_LE_U32(inputStream);
    if (totalSize == 0)
    {
        output.resize(0);
        return;
    }

    if (output.size() <= 16)
    {
        output.resize(0);
        return;
    }

    inputStream += 4;
    int controlByte = READ_LE_U8(inputStream);

    std::vector<u8>::iterator outputStream = output.begin();
    do 
    {
        u8 controlBit = controlByte & 1;
        u8 bitCounter = 8;
        inputStream++;

        do 
        {
            controlByte >>= 1;
            bitCounter--;
            if (controlBit == 0)
            {
                *outputStream++ = *inputStream++;
            }
            else
            {
                u8 controlByte1 = *inputStream++;
                u8 controlByte0 = *inputStream++;

                int offset = ((controlByte0 & 0xF) << 8) | controlByte1;
                int length = (controlByte0 >> 4) + 3;

                std::vector<u8>::iterator readBackIterator = outputStream - offset;

                for (int i = 0; i < length; i++)
                {
                    *outputStream++ = *readBackIterator++;
                }
            }
            controlBit = controlByte & 1;
        } while (bitCounter != 0);
        controlByte = READ_LE_U8(inputStream);
    } while (outputStream - output.begin() < totalSize);

    output.resize(totalSize);
}

void fieldDecompress(int size, std::vector<u8>::iterator& inputStream, std::vector<u8>& output)
{
    decompress(inputStream, output);
}

std::vector<u8> rawField;
std::vector<u8> rawFieldImageBundle;
std::vector<u8> rawFieldImageBundle2;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldTriggerData;
std::vector<u8> rawFieldDialogBundle;
std::vector<u8> rawFieldWalkMesh;
std::vector<u8> rawFieldActorSetupParams;
MemoryEditor mFieldMemoryEditor;

void LoadAndInspectField(int fieldIndex)
{
    setCurrentDirectory(4, 0);
    readFile(fieldIndex + 184, rawField);

    {
        int rawFieldImageBundleSize = READ_LE_U32(&rawField[0x10C]);
        rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
        fieldDecompress(rawFieldImageBundleSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x130]), rawFieldImageBundle);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x110]);
        rawFieldWalkMesh.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x134]), rawFieldWalkMesh);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x114]);
        rawFieldModels.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x138]), rawFieldModels);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x118]);
        rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x13C]), rawFieldActorSetupParams);
    }

    {
        int rawFieldImageBundle2Size = READ_LE_U32(&rawField[0x11C]);
        rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
        fieldDecompress(rawFieldImageBundle2Size + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x140]), rawFieldImageBundle2);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x120]);
        rawFieldScriptData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x144]), rawFieldScriptData);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x128]);
        rawFieldDialogBundle.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x14C]), rawFieldDialogBundle);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x12C]);
        rawFieldTriggerData.resize(rawFieldSize + 0x10);
       // fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x150]), rawFieldTriggerData);
    }


}



void InspectField()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Field"))
        {
            if (ImGui::MenuItem("Load&Inspect"))
            {
                LoadAndInspectField(2);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (rawField.size() == 0)
        return;

    mFieldMemoryEditor.DrawWindow("Field raw memory", &rawField[0], rawField.size());

    if (ImGui::Begin("FieldInspector"))
    {
        if (ImGui::BeginTabBar("Field Tabs"))
        {
            if (ImGui::BeginTabItem("Info"))
            {
                ImGui::Text("Num image bundle 1 entries: %d", READ_LE_U32(rawFieldImageBundle.begin()));
                ImGui::Text("Num image bundle 2 entries: %d", READ_LE_U32(rawFieldImageBundle2.begin()));
                ImGui::Text("Num bundle3 entries: %d", READ_LE_U32(rawFieldModels.begin()));
                ImGui::Text("Num fieldScript num entities: %d", READ_LE_U32(rawFieldScriptData.begin() + 0x80));
                ImGui::Text("Num triggers: %d", rawFieldTriggerData.size() / 24);
                ImGui::Text("Dialog bundle size: %d", rawFieldDialogBundle.size());
                ImGui::Text("Dialog 7 num entries: %d", READ_LE_U8(rawFieldWalkMesh.begin()));
                ImGui::Text("Bundle8 (character init params) size: %d", rawFieldActorSetupParams.size());

                ImGui::Separator();
                ImGui::Text("Scene setup");
                std::vector<u8>::iterator sceneParams = rawField.begin() + 0x154;
                ImGui::Text("(%d %d %d)", READ_LE_S16(sceneParams + 0), READ_LE_S16(sceneParams + 2), READ_LE_S16(sceneParams + 4));
                ImGui::Text("(%d %d)", READ_LE_S16(sceneParams + 8), READ_LE_S16(sceneParams + 10));
                ImGui::Text("(%d %d %d)", READ_LE_S16(sceneParams + 0x10), READ_LE_S16(sceneParams + 0x12), READ_LE_S16(sceneParams + 0x14));
                ImGui::Text("(%d %d %d)", READ_LE_S16(sceneParams + 0x30), READ_LE_S16(sceneParams + 0x32), READ_LE_S16(sceneParams + 0x34));

                ImGui::Separator();
                ImGui::Text("Actors count: %d", READ_LE_U16(rawField.begin() + 0x18C));
                for (int i = 0; i < READ_LE_U16(rawField.begin() + 0x18C); i++)
                {
                    std::vector<u8>::iterator actorDefinition = rawField.begin() + 0x190 + 16 * i;
                    char name[256];
                    sprintf(name, "Actor %d", i);
                    if (ImGui::CollapsingHeader(name))
                    {
                        ImGui::Text("Flags: 0x%04X", READ_LE_U16(actorDefinition + 0));
                        ImGui::Text("(%d, %d, %d)", READ_LE_S16(actorDefinition + 2), READ_LE_S16(actorDefinition + 4), READ_LE_S16(actorDefinition + 6));
                        ImGui::Text("(%d, %d, %d)", READ_LE_S16(actorDefinition + 8), READ_LE_S16(actorDefinition + 10), READ_LE_S16(actorDefinition + 12));
                        ImGui::Text("Index in bundle3: %d", READ_LE_U16(actorDefinition + 14));
                    }

                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dialog"))
            {
                int NumEntries = READ_LE_U32(rawFieldDialogBundle.begin());

                for (int i = 0; i < NumEntries; i++)
                {
                    int numLines = 1;
                    std::string decodedString;
                    std::vector<u8>::iterator dialogStream = rawFieldDialogBundle.begin() + READ_LE_U16(rawFieldDialogBundle.begin() + 4 + i * 2);
                    while (*dialogStream)
                    {
                        u8 xenoChar = *dialogStream++;

                        if ((xenoChar >= 0x16) && (xenoChar <= 0x1F))
                        {
                            decodedString += (char)(xenoChar - 0x16 + '0');
                        }
                        else if ((xenoChar >= 0x20) && (xenoChar <= 0x39))
                        {
                            decodedString += (char)(xenoChar - 0x20 + 'A');
                        }
                        else if ((xenoChar >= 0x3D) && (xenoChar <= 0x56))
                        {
                            decodedString += (char)(xenoChar - 0x3D + 'a');
                        }
                        else
                        {
                            switch (xenoChar)
                            {
                            case 0x1:
                                decodedString += '\n';
                                numLines++;
                                break;
                            case 0x2:
                                decodedString += "<end>\n";
                                numLines++;
                                break;
                            case 0xF:
                                switch (*dialogStream++)
                                {
                                case 0:
                                    // delay
                                    break;
                                case 5:
                                    switch (*dialogStream++)
                                    {
                                    case 0:
                                        decodedString += "<Fei>";
                                        break;
                                    case 1:
                                        decodedString += "<Elly>";
                                        break;
                                    case 2:
                                        decodedString += "<Citan>";
                                        break;
                                    case 3:
                                        decodedString += "<Bart>";
                                        break;
                                    case 4:
                                        decodedString += "<Billy>";
                                        break;
                                    case 5:
                                        decodedString += "<Rico>";
                                        break;
                                    case 6:
                                        decodedString += "<Emeralda>";
                                        break;
                                    case 7:
                                        decodedString += "<Chu-Chu>";
                                        break;
                                    case 8:
                                        decodedString += "<Maria>";
                                        break;
                                    case 9:
                                        decodedString += "<Citan2>";
                                        break;
                                    case 10:
                                        decodedString += "<Emeralda2>";
                                        break;
                                    case 11:
                                        decodedString += "<Weltall>";
                                        break;
                                    case 12:
                                        decodedString += "<Weltall-2>";
                                        break;
                                    case 13:
                                        decodedString += "<Vierge>";
                                        break;
                                    case 14:
                                        decodedString += "<Heimdal>";
                                        break;
                                    case 15:
                                        decodedString += "<Brigandier>";
                                        break;
                                    case 16:
                                        decodedString += "<Renmazuo>";
                                        break;
                                    case 17:
                                        decodedString += "<Stier>";
                                        break;
                                    case 18:
                                        decodedString += "<BigChu-chu>";
                                        break;
                                    case 19:
                                        decodedString += "<Seibzehn>";
                                        break;
                                    case 20:
                                        decodedString += "<Crescens>";
                                        break;
                                    case 21:
                                        decodedString += "<Regurus>";
                                        break;
                                    case 22:
                                        decodedString += "<Fenrir>";
                                        break;
                                    case 23:
                                        decodedString += "<Andvari>";
                                        break;
                                    case 24:
                                        decodedString += "<Renmazuo2>";
                                        break;
                                    case 25:
                                        decodedString += "<Stier-2>";
                                        break;
                                    case 26:
                                        decodedString += "<Xenogears>";
                                        break;
                                    case 27:
                                        decodedString += "<BARTHOS>";
                                        break;
                                    case 28:
                                        decodedString += "<Yggdra>";
                                        break;
                                    case 128:
                                        decodedString += "<Perso1>";
                                        break;
                                    case 129:
                                        decodedString += "<Perso2>";
                                        break;
                                    case 130:
                                        decodedString += "<Perso3>";
                                        break;
                                    }
                                }
                                break;
                            case 0x10:
                                decodedString += ' ';
                                break;
                            case 0x11:
                                decodedString += "+";
                                break;
                            case 0x12:
                                decodedString += ",";
                                break;
                            case 0x13:
                                decodedString += "-";
                                break;
                            case 0x14:
                                decodedString += ".";
                                break;
                            case 0x15:
                                decodedString += "/";
                                break;
                            case 0x3A:
                                decodedString += "[";
                                break;
                            case 0x3B:
                                decodedString += "]";
                                break;
                            case 0x3C:
                                decodedString += "=";
                                break;
                            case 0x57:
                                decodedString += "!";
                                break;
                            case 0x58:
                                decodedString += "\"";
                                break;
                            case 0x59:
                                decodedString += "#";
                                break;
                            case 0x5A:
                                decodedString += "%";
                                break;
                            case 0x5B:
                                decodedString += "&";
                                break;
                            case 0x5C:
                                decodedString += "'";
                                break;
                            case 0x5D:
                                decodedString += "(";
                                break;
                            case 0x5E:
                                decodedString += ")";
                                break;
                            case 0x5F:
                                decodedString += ":";
                                break;
                            case 0x60:
                                decodedString += "?";
                                break;
                            case 0x61:
                                decodedString += "<0>";
                                break;
                            case 0x62:
                                decodedString += "<1>";
                                break;
                            case 0x63:
                                decodedString += "<2>";
                                break;
                            case 0x64:
                                decodedString += "<3>";
                                break;
                            case 0x65:
                                decodedString += "<4>";
                                break;
                            case 0x66:
                                decodedString += "<5>";
                                break;
                            case 0x67:
                                decodedString += "<6>";
                                break;
                            case 0x68:
                                decodedString += "<7>";
                                break;
                            case 0x69:
                                decodedString += "<8>";
                                break;
                            case 0x6A:
                                decodedString += "<9>";
                                break;
                            case 0x6B:
                                decodedString += "<%%>";
                                break;
                            case 0x6C:
                                decodedString += "<&>";
                                break;
                            case 0x6D:
                                decodedString += "*";
                                break;
                            case 0x6E:
                                decodedString += "<C>";
                                break;
                            case 0x6F:
                                decodedString += "<S>";
                                break;
                            case 0x70:
                                decodedString += "<T>";
                                break;
                            case 0x71:
                                decodedString += "<*>";
                                break;
                            case 0x72:
                                decodedString += "<R>";
                                break;
                            case 0x73:
                                decodedString += "<L>";
                                break;
                            case 0x74:
                                decodedString += "<U>";
                                break;
                            case 0x75:
                                decodedString += "<D>";
                                break;
                            case 0x76:
                                decodedString += "<.>";
                                break;
                            case 0x77:
                                decodedString += "<:>";
                                break;
                            case 0x79:
                                decodedString += "</>";
                                break;
                            case 0x7A:
                                decodedString += "<..>";
                                break;
                            case 0x7B:
                                decodedString += "<`>";
                                break;
                            case 0x7D:
                                decodedString += "<+>";
                                break;
                            case 0x7E:
                                decodedString += "<->";
                                break;
                            case 0x7F:
                                decodedString += "<X>";
                                break;
                            case 0x80:
                                decodedString += "<[>";
                                break;
                            case 0x81:
                                decodedString += "<]>";
                                break;
                            case 0x82:
                                decodedString += "<%%>";
                                break;
                            case 0x83:
                                decodedString += "<&>";
                                break;
                            case 0x84:
                                decodedString += "<(>";
                                break;
                            case 0x85:
                                decodedString += "<)>";
                                break;
                            case 0x86:
                                decodedString += "<#>";
                                break;
                            case 0x87:
                                decodedString += "<`>";
                                break;
                            case 0x88:
                                decodedString += "<°>";
                                break;
                            case 0x89:
                                decodedString += "<=>";
                                break;
                            case 0x8A:
                                decodedString += "<?>";
                                break;
                            case 0x8B:
                                decodedString += "<!>";
                                break;
                            case 0x8C:
                                decodedString += "<_>";
                                break;
                            case 0x8D:
                                decodedString += "<~>";
                                break;
                            case 0x8E:
                                decodedString += "<...>";
                                break;
                            case 0x8F:
                                decodedString += "<'>";
                                break;
                            default:
                                {
                                    char temp[256];
                                    sprintf(temp, "<0x%02X>", xenoChar);
                                    decodedString += temp;
                                }
                                break;
                            }
                        }
                    }

                    ImVec2 child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * numLines);
                    if (ImGui::BeginChildFrame(i + 1, child_size))
                    {
                        ImGui::Text(decodedString.c_str());
                    }
                    ImGui::EndChildFrame();
                    
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

bool noahFrame()
{
    StartFrame();

    c_filesystemExplorer::frame();

    InspectField();

    EndFrame();

    return gCloseApp;
}

void noahExit()
{
    deleteBgfxGlue();
}

u16 READ_LE_U16(void* ptr)
{
    return *(u16*)ptr;
}

s16 READ_LE_S16(void* ptr)
{
    return *(s16*)ptr;
}

u32 READ_LE_U32(void* ptr)
{
    return *(u32*)ptr;
}

s32 READ_LE_S32(void* ptr)
{
    return *(s32*)ptr;
}

u8 READ_LE_U8(std::vector<u8>::iterator& inputStream)
{
    return *inputStream;
}

u16 READ_LE_U16(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_U16(&inputStream[0]);
}

s16 READ_LE_S16(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_S16(&inputStream[0]);
}

u32 READ_LE_U32(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_U32(&inputStream[0]);
}
