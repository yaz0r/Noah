#include "noahLib.h"
#include "fieldInspector.h"
#include "imgui.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"

#include "../imgui_club/imgui_memory_editor/imgui_memory_editor.h"

#include <array>
#include <vector>

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

void dump(const std::vector<u8>& buffer, const char* outputName)
{
    FILE* fHandle = fopen(outputName, "wb+");
    if (fHandle)
    {
        fwrite(&buffer[0], 1, buffer.size(), fHandle);

        fclose(fHandle);
    }
}

void LoadAndInspectField(int fieldIndex)
{
    setCurrentDirectory(4, 0);
    readFile(fieldIndex * 2 + 184, rawField);

    {
        int rawFieldImageBundleSize = READ_LE_U32(&rawField[0x10C]);
        rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
        fieldDecompress(rawFieldImageBundleSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x130]), rawFieldImageBundle);
        dump(rawFieldImageBundle, "0_image1.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x110]);
        rawFieldWalkMesh.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x134]), rawFieldWalkMesh);
        dump(rawFieldWalkMesh, "1_walkmesh.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x114]);
        rawFieldModels.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x138]), rawFieldModels);
        dump(rawFieldModels, "2_models.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x118]);
        rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x13C]), rawFieldActorSetupParams);
        dump(rawFieldActorSetupParams, "3_fieldEntities.bin");
    }

    {
        int rawFieldImageBundle2Size = READ_LE_U32(&rawField[0x11C]);
        rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
        fieldDecompress(rawFieldImageBundle2Size + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x140]), rawFieldImageBundle2);
        dump(rawFieldImageBundle2, "4_image2.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x120]);
        rawFieldScriptData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x144]), rawFieldScriptData);
        dump(rawFieldScriptData, "5_fieldScript.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x128]);
        rawFieldDialogBundle.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x14C]), rawFieldDialogBundle);
        dump(rawFieldDialogBundle, "6_dialog.bin");
    }

    {
        int rawFieldSize = READ_LE_U32(&rawField[0x12C]);
        rawFieldTriggerData.resize(rawFieldSize + 0x10);
        // fieldDecompress(rawFieldSize + 0x10, rawField.begin() + READ_LE_U32(&rawField[0x150]), rawFieldTriggerData);
    }


}

void fieldInspector_info()
{
    ImGui::Text("Num image bundle 1 entries: %d", READ_LE_U32(rawFieldImageBundle.begin()));
    ImGui::Text("Num image bundle 2 entries: %d", READ_LE_U32(rawFieldImageBundle2.begin()));
    ImGui::Text("Num bundle3 entries: %d", READ_LE_U32(rawFieldModels.begin()));
    ImGui::Text("Num fieldScript num entities: %d", READ_LE_U32(rawFieldScriptData.begin() + 0x80));
    ImGui::Text("Num triggers: %d", rawFieldTriggerData.size() / 24);
    ImGui::Text("Dialog bundle size: %d", rawFieldDialogBundle.size());
    ImGui::Text("Walkmesh entries: %d", READ_LE_U8(rawFieldWalkMesh.begin()));
    ImGui::Text("Map entities size: %d", rawFieldActorSetupParams.size());

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
}

void fieldInspector_decodeDialog(std::vector<u8>::iterator& dialogStream, std::string& decodedString, int& numLines)
{
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
}

void fieldInspector_dialogs()
{
    int NumEntries = READ_LE_U32(rawFieldDialogBundle.begin());

    for (int i = 0; i < NumEntries; i++)
    {
        int numLines = 1;
        std::string decodedString;
        std::vector<u8>::iterator dialogStream = rawFieldDialogBundle.begin() + READ_LE_U16(rawFieldDialogBundle.begin() + 4 + i * 2);

        fieldInspector_decodeDialog(dialogStream, decodedString, numLines);

        ImVec2 child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * numLines);
        if (ImGui::BeginChildFrame(i + 1, child_size))
        {
            ImGui::Text(decodedString.c_str());
        }
        ImGui::EndChildFrame();

    }
}

void displayDialog(int index)
{
    int numLines = 1;
    std::string decodedString;
    std::vector<u8>::iterator dialogStream = rawFieldDialogBundle.begin() + READ_LE_U16(rawFieldDialogBundle.begin() + 4 + index * 2);

    fieldInspector_decodeDialog(dialogStream, decodedString, numLines);

    ImVec2 child_size = ImVec2(200, ImGui::GetTextLineHeightWithSpacing() * numLines);
    if (ImGui::BeginChildFrame(index + 1, child_size))
    {
        ImGui::Text(decodedString.c_str());
    }
    ImGui::EndChildFrame();
}

u16 startOfInstruction;
u16 currentPC = -1;

u8 readScriptByte(int offset)
{
    int numScriptEntity = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
    return rawFieldScriptData[0x84 + numScriptEntity * 0x40 + currentPC + offset];
}

u16 readU16FromScript(int offset)
{
    int numScriptEntity = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
    return READ_LE_U16(rawFieldScriptData.begin() + 0x84 + numScriptEntity * 0x40 + currentPC + offset);
}

s16 readS16FromScript(int offset)
{
    int numScriptEntity = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
    return READ_LE_S16(rawFieldScriptData.begin() + 0x84 + numScriptEntity * 0x40 + currentPC + offset);
}

void getVariable(int index)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
    ImGui::Text("VAR[0x%04X]", index);
    ImGui::PopStyleColor();
}

void getImmediateOrVariableUnsigned(int offset)
{
    u16 value = readU16FromScript(offset);
    if (value & 0x8000)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
        ImGui::Text("0x%04X", value & 0x7FFF);
        ImGui::PopStyleColor();
    }
    else
    {
        getVariable(value & 0x7FFF);
    }
}

void readCharacter(int offset)
{
    u8 value = readScriptByte(offset);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1, 0, 1));
    ImGui::Text("0x%02X", value);
    ImGui::PopStyleColor();
}

void decodeGenericOpcode(const char* opcodeName, const char* paramterTypes)
{
    ImGui::Text("%s(", opcodeName); ImGui::SameLine(0, 0);
    int offsetToArgs = 1;
    while (*paramterTypes)
    {
        switch (*paramterTypes)
        {
            case 'b':
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
                ImGui::Text("%d", readScriptByte(offsetToArgs)); ImGui::SameLine(0, 0);
                ImGui::PopStyleColor();
                offsetToArgs++;
                break;
            case 's':
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
                ImGui::Text("0x%04X", readU16FromScript(offsetToArgs)); ImGui::SameLine(0, 0);
                ImGui::PopStyleColor();
                offsetToArgs+=2;
                break;
            case 'c':
                readCharacter(offsetToArgs); ImGui::SameLine(0, 0);
                offsetToArgs++;
                break;
            case 'i':
                getImmediateOrVariableUnsigned(offsetToArgs); ImGui::SameLine(0, 0);
                offsetToArgs += 2;
                break;
            case 'v':
                getVariable(offsetToArgs); ImGui::SameLine(0, 0);
                offsetToArgs += 2;
                break;
            default:
                assert(0);
                break;
        }

        // last parameter?
        if (*(++paramterTypes))
        {
            ImGui::Text(", "); ImGui::SameLine(0, 0);
        }

        ImGui::SameLine(0, 0);
    }
    ImGui::Text(")");
}

struct sByteType
{
    u8 mType;
    std::string mLabel;
    std::string mComment;
};

std::vector<sByteType> m_byteTypeTable;
std::vector<u16> mToExplore;

void addToExploreStack(u16 newPCToExplore)
{
    if (newPCToExplore > m_byteTypeTable.size())
    {
        return;
    }
    if (m_byteTypeTable[newPCToExplore].mType == 0)
    {
        mToExplore.push_back(newPCToExplore);
        m_byteTypeTable[newPCToExplore].mType = 1;
    }
}

void markFunctionStart(u16 functionStart, std::string name)
{
    addToExploreStack(functionStart);

    if (name.length() == 0)
    {
        char buffer[256];
        sprintf(buffer, "Function 0x%04X", functionStart);
        name = buffer;
    }
    m_byteTypeTable[functionStart].mLabel = name;
}

void addComment(std::string comment)
{
    m_byteTypeTable[startOfInstruction].mComment = comment;
}

void decodeJumpIf()
{
    u16 var0Raw = readU16FromScript(1);
    u16 var1Raw = readU16FromScript(3);
    u8 controlByte = readScriptByte(5);
    u16 jumpDestination = readU16FromScript(6);
 
    ImGui::Text("IF("); ImGui::SameLine(0,0);

    switch (controlByte & 0xF0)
    {
    case 0x0:
        getVariable(var0Raw);
        break;
    case 0x40:
        getVariable(var0Raw);
        break;
    case 0x80:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
        ImGui::Text("0x%04X", var0Raw);
        ImGui::PopStyleColor();
        break;
    case 0xC0:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
        ImGui::Text("0x%04X", var0Raw);
        ImGui::PopStyleColor();
        break;
    default:
        assert(0);
        break;
    }

    ImGui::SameLine();

    switch (controlByte & 0x0F)
    {
    case 0:
        ImGui::Text("==");
        break;
    case 1:
        ImGui::Text("!=");
        break;
    case 2:
        ImGui::Text(">");
        break;
    case 3:
        ImGui::Text("<");
        break;
    case 4:
        ImGui::Text(">=");
        break;
    case 5:
        ImGui::Text("<=");
        break;
    case 6:
        ImGui::Text("&");
        break;
    case 7:
        ImGui::Text("!=");
        break;
    case 8:
        ImGui::Text("|");
        break;
    default:
        assert(0);
        break;
    }

    ImGui::SameLine();

    switch (controlByte & 0xF0)
    {
    case 0x0:
        getVariable(var1Raw);
        break;
    case 0x40:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
        ImGui::Text("0x%04X", var1Raw);
        ImGui::PopStyleColor();
        break;
    case 0x80:
        getVariable(var1Raw);
        break;
    case 0xC0:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
        ImGui::Text("0x%04X", var1Raw);
        ImGui::PopStyleColor();
        break;
    default:
        assert(0);
        break;
    }

    ImGui::SameLine(0, 0); ImGui::Text(") GOTO "); ImGui::SameLine(0, 0);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 1, 1));
    ImGui::Text("0x%04X", jumpDestination);
    ImGui::PopStyleColor();

    addToExploreStack(jumpDestination);
}


void printVarAssignementFromU16(int offset)
{
    u16 index = readU16FromScript(offset);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
    ImGui::Text("VAR[0x%04X] = ", index);
    ImGui::PopStyleColor();
    ImGui::SameLine();
}

void getVarWithFlag(int flag, int valueOffset, int controlOffset)
{
    u8 controlValue = readScriptByte(controlOffset);

    int value;

    if (controlValue & flag)
    {
        value = readS16FromScript(valueOffset);
    }
    else
    {
        value = readU16FromScript(valueOffset);
    }

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
    ImGui::Text("%d", value);
    ImGui::PopStyleColor();
}

void getVar80(int valueOffset, int controlOffset)
{
    getVarWithFlag(0x80, valueOffset, controlOffset);
}

void getVar40(int valueOffset, int controlOffset)
{
    getVarWithFlag(0x40, valueOffset, controlOffset);
}

void getVar20(int valueOffset, int controlOffset)
{
    getVarWithFlag(0x20, valueOffset, controlOffset);
}

void getVar10(int valueOffset, int controlOffset)
{
    getVarWithFlag(0x10, valueOffset, controlOffset);
}

struct sOpcodeArg
{
    enum type
    {
        unknown = 0,
        entryPoint,
        jumpLocation,
        immediateS16,
        immediateOrVar,
        controlByte,
        byte,
        character,
        dialogId,
    } m_type = unknown;
    std::string m_name;
    u8 m_offset;
};

struct sOpcodeInfo
{
    bool m_isInitialized = false;
    u8 m_opcodeSize = 1;
    std::string m_name;
    std::string m_comment;
    std::vector<sOpcodeArg> m_args;

    void end()
    {
        m_isInitialized = true;
    }

    sOpcodeInfo& setName(const char* name)
    {
        m_name = name;
        return *this;
    }

    sOpcodeInfo& setComment(const char* comment)
    {
        m_comment = comment;
        return *this;
    }

    sOpcodeArg& addAgument(sOpcodeArg::type opcodeType, const char* argumentName = nullptr)
    {
        m_args.push_back(sOpcodeArg());

        sOpcodeArg& newArg = m_args[m_args.size() - 1];

        newArg.m_type = opcodeType;
        newArg.m_offset = m_opcodeSize;
        if (argumentName)
        {
            newArg.m_name = argumentName;
        }

        return newArg;
    }

    sOpcodeInfo& addArgumentJumpLocation(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::jumpLocation, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentFunctionEntryPoint(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::entryPoint, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentS16(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateS16, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentImmediateOrVar(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateOrVar, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addSignControlByte()
    {
        addAgument(sOpcodeArg::controlByte);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentDialogId(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::dialogId, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentByte(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::byte, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentCharacter(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::character, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }
};

std::array<sOpcodeInfo, 256> m_opcode;
std::array<sOpcodeInfo, 256> m_extendedOpcode;

u16 findControlByteOffset(const sOpcodeInfo& opcode, u16 offsetToVarArg)
{
    for (int i = 0; i < opcode.m_args.size(); i++)
    {
        if (opcode.m_args[i].m_type == sOpcodeArg::controlByte)
            return opcode.m_args[i].m_offset;
    }

    assert(0);

    return 0;
}

bool handleGenericOpcode(const sOpcodeInfo& opcode, bool isExtendedOpcode = false)
{
    if (opcode.m_name.length())
    {
        ImGui::Text("%s(", opcode.m_name.c_str()); ImGui::SameLine(0, 0);
    }
    else
    {
        u8 opcode = readScriptByte(0);
        if (isExtendedOpcode)
        {
            ImGui::Text("OP_FE%02X(", opcode); ImGui::SameLine(0, 0);
        }
        else
        {
            ImGui::Text("OP_%02X(", opcode); ImGui::SameLine(0, 0);
        }
    }
    
    int offsetToArgs = 1;
    u8 currentS16ControlFlag = 0x80;
    int dialogIdToDisplay = -1;
    for(int i=0; i<opcode.m_args.size(); i++)
    {
        const sOpcodeArg& argument = opcode.m_args[i];
        switch (argument.m_type)
        {
        case sOpcodeArg::jumpLocation:
        case sOpcodeArg::entryPoint:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 1, 1));
            ImGui::Text("0x%04X", readU16FromScript(offsetToArgs));
            ImGui::PopStyleColor();
            addToExploreStack(readU16FromScript(offsetToArgs));
            offsetToArgs += 2;
            break;
        case sOpcodeArg::immediateOrVar:
            getImmediateOrVariableUnsigned(offsetToArgs);
            offsetToArgs += 2;
            break;
        case sOpcodeArg::immediateS16:
            assert(currentS16ControlFlag);
            getVarWithFlag(currentS16ControlFlag, offsetToArgs, findControlByteOffset(opcode, offsetToArgs));
            currentS16ControlFlag >>= 1;
            offsetToArgs += 2;
            break;
        case sOpcodeArg::controlByte:
            offsetToArgs++;
            break;
        case sOpcodeArg::byte:
        case sOpcodeArg::character:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
            ImGui::Text("%d", readScriptByte(offsetToArgs)); ImGui::SameLine(0, 0);
            ImGui::PopStyleColor();
            offsetToArgs++;
            break;

        case sOpcodeArg::dialogId:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
            ImGui::Text("0x%04X", readU16FromScript(offsetToArgs)); ImGui::SameLine(0, 0);
            ImGui::PopStyleColor();
            dialogIdToDisplay = readU16FromScript(offsetToArgs);
            offsetToArgs += 2;
            break;

            /*
        case 'b':
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
            ImGui::Text("%d", readScriptByte(offsetToArgs)); ImGui::SameLine(0, 0);
            ImGui::PopStyleColor();
            offsetToArgs++;
            break;
        case 's':
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0, 1));
            ImGui::Text("0x%04X", readU16FromScript(offsetToArgs)); ImGui::SameLine(0, 0);
            ImGui::PopStyleColor();
            offsetToArgs += 2;
            break;
        case 'c':
            readCharacter(offsetToArgs); ImGui::SameLine(0, 0);
            offsetToArgs++;
            break;
        case 'i':
            getImmediateOrVariableUnsigned(offsetToArgs); ImGui::SameLine(0, 0);
            offsetToArgs += 2;
            break;
        case 'v':
            getVariable(offsetToArgs); ImGui::SameLine(0, 0);
            offsetToArgs += 2;
            break;
            */
        default:
            assert(0);
            break;
        }

        ImGui::SameLine(0, 0);

        // last parameter?
        if (i != opcode.m_args.size() - 1)
        {
            ImGui::Text(", "); ImGui::SameLine(0, 0);
        }
    }
    ImGui::Text(")");

    if (dialogIdToDisplay >= 0)
    {
        displayDialog(dialogIdToDisplay);
    }

    currentPC += offsetToArgs;

    return true;
}

void initOpcodeTable()
{
    m_opcode[0x01]
        .setName("JUMP")
        .addArgumentJumpLocation()
        .end();

    m_opcode[0x04]
        .end();

    m_opcode[0x05]
        .setName("CALL")
        .addArgumentFunctionEntryPoint("Call destination")
        .setComment("Call a sub function")
        .end();

    m_opcode[0xB]
        .setName("INIT_ENTITY_NPC")
        .addArgumentImmediateOrVar("Id")
        .setComment("Init as NPC")
        .end();

    m_opcode[0x15]
        .end();

    m_opcode[0x16]
        .setName("INIT_ENTITY_PC")
        .addArgumentImmediateOrVar("Id")
        .setComment("Init as PC")
        .end();

    m_opcode[0x19]
        .setName("SET_ACTOR_POSITION_2D")
        .addArgumentS16("X")
        .addArgumentS16("Y")
        .addSignControlByte()
        .end();

    m_opcode[0x1F]
        .addArgumentByte()
        .end();

    m_opcode[0x26]
        .setName("Wait")
        .addArgumentImmediateOrVar("Amount")
        .end();

    m_opcode[0x28]
        .addArgumentCharacter()
        .end();

    m_opcode[0x2A]
        .end();

    m_opcode[0x2C]
        .addArgumentByte()
        .end();

    m_opcode[0x35]
        .addArgumentImmediateOrVar()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x3C]
        .setName("SET_VAR_INC")
        .addArgumentByte()
        .end();

    m_opcode[0x46]
        .end();

    m_opcode[0x47]
        .setName("FIELD_CHANGE_WHEN_READY")
        .addArgumentByte()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .setComment("TODO: the first byte is unused?")
        .end();

    m_opcode[0x57]
        .setName("FIELD_CHANGE_WHEN_READY")
        .addArgumentByte()
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x5A]
        .setName("OP_5A")
        .end();

    m_opcode[0x5B]
        .end();

    m_opcode[0x5D]
        .addArgumentByte()
        .end();

    m_opcode[0x5E]
        .end();

    m_opcode[0x5F]
        .addArgumentByte()
        .end();

    m_opcode[0x60]
        .end();

    m_opcode[0x63]
        .addArgumentS16()
        .addArgumentS16()
        .addArgumentS16()
        .addSignControlByte()
        .end();

    m_opcode[0x64]
        .end();

    m_opcode[0x6F]
        .setName("ROTATE_TO_ACTOR")
        .addArgumentCharacter()
        .end();

    m_opcode[0x74]
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0x75]
        .addArgumentImmediateOrVar()
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

    m_opcode[0x91]
        .setName("IF_CHARACTER_IN_PARTY")
        .addArgumentByte()
        .addArgumentJumpLocation()
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
        .end();

    m_opcode[0xA0]
        .setName("OP_CONFIG_SCREEN")
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xA3]
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

    m_opcode[0xB5]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();

    m_opcode[0xBA]
        .setName("REMOVE_FROM_AVAILABLE_PARTY")
        .addArgumentByte()
        .end();

    m_opcode[0xBC]
        .setName("INIT_ENTITY_SCRIPT")
        .end();

    m_opcode[0xC4]
        .setName("REMOVE_FROM_AVAILABLE_PARTY")
        .addArgumentByte()
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

    m_opcode[0xEE]
        .addArgumentByte()
        .addArgumentByte()
        .end();
}

void initExtendedOpcodeTable()
{
    m_extendedOpcode[0xD]
        .setName("SET_DIALOG_AVATAR")
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x18]
        .setComment("Load something, skip next opcode if already loaded")
        .addArgumentByte()
        .end();

    m_extendedOpcode[0x1A]
        .setComment("Wait for loading to finish")
        .end();

    m_extendedOpcode[0x53]
        .end();

    m_extendedOpcode[0x54]
        .end();

    m_extendedOpcode[0x64]
        .addArgumentImmediateOrVar()
        .end();

    m_extendedOpcode[0x8E]
        .addArgumentImmediateOrVar()
        .addArgumentImmediateOrVar()
        .end();
}

bool decompileOpcode(u16& inputPC)
{
    static bool bOpcodesInitialized = false;
    if (!bOpcodesInitialized)
    {
        initOpcodeTable();
        initExtendedOpcodeTable();
        bOpcodesInitialized = true;
    }

    startOfInstruction = inputPC;
    currentPC = inputPC;
    ImGui::Text("    0x%04X\t", currentPC); ImGui::SameLine();

    u16 opcode = readScriptByte(0);

    if (opcode == 0xFE)
    {
        currentPC++;
        opcode <<= 8;
        opcode |= readScriptByte(0);
        if (m_extendedOpcode[opcode& 0xFF].m_isInitialized)
        {
            return handleGenericOpcode(m_extendedOpcode[opcode&0xFF], true);
        }
    }
    else if (m_opcode[opcode].m_isInitialized)
    {
        return handleGenericOpcode(m_opcode[opcode]);
    }

    switch (opcode)
    {
    case 0:
        ImGui::Text("RETURN()");
        return false;
    case 2:
        decodeJumpIf();
        currentPC += 8;
        return true;
    case 0xC: //12
        ImGui::Text("UPDATE_CHARACTER_INFINITLY()");
        return false;

    default:
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
        ImGui::Text("Failed to decode opcode 0x%02X (%d)", opcode, opcode&0xFF);
        ImGui::PopStyleColor();
        return false;
    }

    return true;
}

int previousEntityInspected = -1;

void fieldInspector_scriptsForEntity(int entityId)
{
    if (previousEntityInspected != entityId)
    {
        m_byteTypeTable.clear();
        previousEntityInspected = entityId;
    }

    if (m_byteTypeTable.size() == 0)
    {
        m_byteTypeTable.resize(rawFieldScriptData.size());
        for (int i = 0; i < rawFieldScriptData.size(); i++)
        {
            m_byteTypeTable[i].mType = 0;
        }

        //for (int entityId = 0; entityId < numScriptEntity; entityId++)
        {
            for (int scriptId = 0; scriptId < 0x20; scriptId++)
            {
                u16 scriptStart = READ_LE_U16(rawFieldScriptData.begin() + 0x84 + entityId * 0x40 + scriptId * 2);
                if (scriptStart)
                {
                    char buffer[256];
                    sprintf(buffer, "Entity %d Function %d", entityId, scriptId);
                    markFunctionStart(scriptStart, std::string(buffer));
                }
            }
        }
    }

    for (int i = 0; i < m_byteTypeTable.size(); i++)
    {
        if (m_byteTypeTable[i].mType == 1)
        {
            if (m_byteTypeTable[i].mLabel.length())
            {
                ImGui::Separator();
                ImGui::Text("%s:", m_byteTypeTable[i].mLabel.c_str());
            }

            u16 startPC = i;
            while (decompileOpcode(startPC)) {
                if (m_byteTypeTable[startPC].mComment.length())
                {
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
                    ImGui::Text("// %s", m_byteTypeTable[startPC].mComment.c_str());
                    ImGui::PopStyleColor();
                }
                startPC = currentPC;
                m_byteTypeTable[startPC].mType = 1;
            }
            i = currentPC;
        }
    }

    for (int i = 0; i < mToExplore.size(); i++)
    {
        u16 startPC = mToExplore[i];
        if (m_byteTypeTable[startPC].mType == 1)
            continue; // already handled

        m_byteTypeTable[startPC].mType = 1;

        while (decompileOpcode(startPC)) {
            startPC = currentPC;
            m_byteTypeTable[startPC].mType = 1;
        }
    }

    mToExplore.clear();
}

void fieldInspector_scripts()
{
    int numScriptEntity = READ_LE_U32(rawFieldScriptData.begin() + 0x80);

    if (ImGui::BeginTabBar("Scripts Tabs"))
    {
        for (int entityId = 0; entityId < numScriptEntity; entityId++)
        {
            char name[256];
            sprintf(name, "%d", entityId);
            if (ImGui::BeginTabItem(name))
            {
                fieldInspector_scriptsForEntity(entityId);

                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

void fieldInspector_frame()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Field"))
        {
            if (ImGui::MenuItem("Load&Inspect"))
            {
                LoadAndInspectField(0);
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
                fieldInspector_info();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dialog"))
            {
                fieldInspector_dialogs();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Scripts"))
            {
                fieldInspector_scripts();

                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

