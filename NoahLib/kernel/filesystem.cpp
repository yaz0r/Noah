#include "noahLib.h"
#include "filesystem.h"
#include "isoManager.h"
#include "imgui.h"
#include "tinyxml2.h"
#include "kernel/decompress.h"

const char filesystemDebugFileName[] = "debuginfo/filesystem.xml";

#include "../imgui_club/imgui_memory_editor/imgui_memory_editor.h"

std::vector<s_fileTableEntry> fatFileTableBuffer;
std::vector<s_directoryEntry> fatDirectoryTableBuffer;

void readRawSectorsFromDisk(int startSector, std::vector<u8>& output, int size, int, int)
{
    c_isoManager::getCurrentDisc()->readData(startSector, size, output);
}

int isCDBusy()
{
    MissingCode();
    return 0;
}

int waitReadCompletion(int param_1)
{
    int iVar1;

    if (param_1 == 0) {
        do {
            iVar1 = isCDBusy();
        } while (0 < iVar1);
    }
    iVar1 = isCDBusy();
    return iVar1;
}

int getNumFilesInDirectory(int directory)
{
    if (fatDirectoryTableBuffer[directory].m0_firstFileIndex < 0)
        return 0;

    return fatDirectoryTableBuffer[directory].mNumFilesInDirectory;
}


struct sfilesysttemDebugInfo
{
    std::vector<std::string> mDirectories;
    std::vector<std::string> mFiles;
} gFilesystemDebugInfo;

void filessystemDebugInfo_save()
{
    tinyxml2::XMLDocument filesystemDebugInfo;

    // write directories
    {
        tinyxml2::XMLElement* directoriesXML = filesystemDebugInfo.NewElement("directories");
        filesystemDebugInfo.InsertEndChild(directoriesXML);

        for (int i = 0; i < gFilesystemDebugInfo.mDirectories.size(); i++)
        {
            if (gFilesystemDebugInfo.mDirectories[i].length())
            {
                tinyxml2::XMLElement* directoryXML = directoriesXML->InsertNewChildElement("directory");

                directoryXML->SetAttribute("ID", i);
                directoryXML->SetAttribute("Name", gFilesystemDebugInfo.mDirectories[i].c_str());
            }
        }
    }

    // write files
    {
        tinyxml2::XMLElement* filesXML = filesystemDebugInfo.NewElement("files");
        filesystemDebugInfo.InsertEndChild(filesXML);

        for (int i = 0; i < gFilesystemDebugInfo.mFiles.size(); i++)
        {
            if (gFilesystemDebugInfo.mFiles[i].length())
            {
                tinyxml2::XMLElement* fileXML = filesXML->InsertNewChildElement("file");

                fileXML->SetAttribute("ID", i);
                fileXML->SetAttribute("Name", gFilesystemDebugInfo.mFiles[i].c_str());
            }
        }
    }

    filesystemDebugInfo.SaveFile(filesystemDebugFileName);

}

void filessystemDebugInfo_load()
{
    tinyxml2::XMLDocument filesystemDebugInfo;
    if (filesystemDebugInfo.LoadFile(filesystemDebugFileName) != tinyxml2::XML_SUCCESS)
    {
        // build debug field debug info

        gFilesystemDebugInfo.mDirectories.clear();
        gFilesystemDebugInfo.mDirectories.resize(fatDirectoryTableBuffer.size());

        gFilesystemDebugInfo.mFiles.clear();
        gFilesystemDebugInfo.mFiles.resize(fatFileTableBuffer.size());

        filessystemDebugInfo_save();
    }
    else
    {
        // init empty structure
        gFilesystemDebugInfo.mDirectories.clear();
        gFilesystemDebugInfo.mDirectories.resize(fatDirectoryTableBuffer.size());

        gFilesystemDebugInfo.mFiles.clear();
        gFilesystemDebugInfo.mFiles.resize(fatFileTableBuffer.size());

        // load directories
        {
            tinyxml2::XMLElement* directoriesXML = filesystemDebugInfo.FirstChildElement("directories");
            tinyxml2::XMLElement* directoryXML = directoriesXML->FirstChildElement("directory");

            while (directoryXML)
            {
                int directoryId = directoryXML->IntAttribute("ID");
                const char* nameAttribute = nullptr;
                directoryXML->QueryStringAttribute("Name", &nameAttribute);
                gFilesystemDebugInfo.mDirectories[directoryId] = nameAttribute;

                directoryXML = directoryXML->NextSiblingElement("directory");
            }
        }

        // load files
        {
            tinyxml2::XMLElement* filesXML = filesystemDebugInfo.FirstChildElement("files");
            tinyxml2::XMLElement* fileXML = filesXML->FirstChildElement("file");

            while (fileXML)
            {
                int fileId = fileXML->IntAttribute("ID");
                const char* nameAttribute = nullptr;
                fileXML->QueryStringAttribute("Name", &nameAttribute);
                gFilesystemDebugInfo.mFiles[fileId] = nameAttribute;

                fileXML = fileXML->NextSiblingElement("file");
            }
        }
    }
}

s32 cdromFnd = 0;

s32 getCdromFnd() {
    return cdromFnd;
}

void initCDAndFileSystem(std::vector<s_fileTableEntry>* fileTable, std::vector<s_directoryEntry>* directoryTable, int mode)
{
    cdromFnd = mode;
    if (mode == -1) {
        cdromFnd = 0;
    }

    // stuff

    {
        std::vector<u8> temp;
        readRawSectorsFromDisk(0x18, temp, 0x8000, 0, 0);
        waitReadCompletion(0);

        for (int i = 0; i < 0x8000 / 7; i++)
        {
            s_fileTableEntry newEntry;
            newEntry.m0_startSector = temp[i * 7 + 0];
            newEntry.m0_startSector += temp[i * 7 + 1] * 0x100;
            newEntry.m0_startSector += temp[i * 7 + 2] * 0x10000;

            newEntry.m3_size = temp[i * 7 + 3];
            newEntry.m3_size += temp[i * 7 + 4] * 0x100;
            newEntry.m3_size += temp[i * 7 + 5] * 0x10000;
            newEntry.m3_size += temp[i * 7 + 6] * 0x1000000;

            fileTable->push_back(newEntry);
        }
    }

    {
        std::vector<u8> temp;
        readRawSectorsFromDisk(0x28, temp, 0x7A, 0, 0);
        waitReadCompletion(0);

        for (int i = 0; i < 0x7A/2; i++)
        {
            s_directoryEntry newEntry;
            newEntry.m0_firstFileIndex = READ_LE_S16(&temp[i * 2]);
            directoryTable->push_back(newEntry);
        }
    }

    // find num files in each directory
    for (int i = 0; i < directoryTable->size(); i++)
    {
        if ((*directoryTable)[i].m0_firstFileIndex == -1)
        {
            (*directoryTable)[i].mNumFilesInDirectory = 0;
        }
        else
        {
            int firstFileForDirectory = (*directoryTable)[i].m0_firstFileIndex;
            int closestDirectoryFirstFile = fileTable->size();
            for (int j = 0; j < directoryTable->size(); j++)
            {
                if (((*directoryTable)[j].m0_firstFileIndex >= 0) && (i != j) && ((*directoryTable)[j].m0_firstFileIndex > firstFileForDirectory) && ((*directoryTable)[j].m0_firstFileIndex < closestDirectoryFirstFile))
                {
                    closestDirectoryFirstFile = (*directoryTable)[j].m0_firstFileIndex;
                }
            }

            (*directoryTable)[i].mNumFilesInDirectory = closestDirectoryFirstFile - firstFileForDirectory;
        }
    }

    // name the directories
    {
        (*directoryTable)[4].mName = "Fields";

        for (int i = 0; i < directoryTable->size(); i++)
        {
            if ((*directoryTable)[i].mName == "")
            {
                char buffer[256];
                sprintf(buffer, "Directory %d", i);
                (*directoryTable)[i].mName = buffer;
            }
        }
    }

    filessystemDebugInfo_load();
}

int startOfDirectoryFileIndex = 0;

int setCurrentDirectory(int directory, int offset)
{
    startOfDirectoryFileIndex = fatDirectoryTableBuffer[directory + offset].m0_firstFileIndex - 1;
    if (startOfDirectoryFileIndex < 0)
    {
        startOfDirectoryFileIndex = 0;
        return -1;
    }

    return startOfDirectoryFileIndex;
}

int getFileSize(int fileIndex)
{
    return fatFileTableBuffer[fileIndex + startOfDirectoryFileIndex - 1].m3_size;
}

int getFileSizeAligned(int fileIndex)
{
    int size = getFileSize(fileIndex);
    int size2 = size + 3;
    if (size2 < 0)
    {
        size2 = size + 6;
    }
    return size2 & ~3;
}

int getFileStartSector(int fileIndex)
{
    return fatFileTableBuffer[fileIndex + startOfDirectoryFileIndex - 1].m0_startSector;
}

void readFile(int fileIndex, std::vector<u8>& output, int, int)
{
    c_isoManager::getCurrentDisc()->readData(getFileStartSector(fileIndex), getFileSizeAligned(fileIndex), output);
}

std::vector<class c_fileHexView*> gFileHexView;

class c_fileHexView
{
public:
    c_fileHexView(int directory, int file, bool bDecompress)
    {
        char buffer[256];
        sprintf(buffer, "%d/%d", directory, file);
        mName = buffer;

        setCurrentDirectory(directory, 0);
        readFile(file, mBuffer, 0, 0x80);

        if (bDecompress) {
            std::vector<u8> decompressedBuffer;
            int totalSize = READ_LE_U32(mBuffer.begin());
            decompressedBuffer.resize(totalSize);
            decompress(mBuffer.begin(), decompressedBuffer);
            mBuffer = decompressedBuffer;
        }
    }

    bool frame()
    {
        mMemoryEditor.ReadOnly = true;
        mMemoryEditor.DrawWindow(mName.c_str(), &mBuffer[0], mBuffer.size());

        if (!mMemoryEditor.Open)
        {
            return false;
        }

        return true;
    }

private:
    std::string mName;
    MemoryEditor mMemoryEditor;
    std::vector<u8> mBuffer;
};

void createHexView(int directory, int file, bool bDecompress = false)
{
    gFileHexView.push_back(new c_fileHexView(directory, file, bDecompress));
}

void c_filesystemExplorer::frame()
{
    std::string popupToOpen = "";
    static int fileToRename = -1;
    static char filenameBuffer[512] = "";
    if (ImGui::Begin("FileSystemExplorer"))
    {
        for (int i = 0; i < fatDirectoryTableBuffer.size() - 1; i++)
        {
            setCurrentDirectory(i, 0);
            if(fatDirectoryTableBuffer[i].mNumFilesInDirectory)
            {
                std::string name = "Directory " + std::to_string(i);
                if (gFilesystemDebugInfo.mDirectories[i].length()) {
                    name = gFilesystemDebugInfo.mDirectories[i];
                }

                if (ImGui::TreeNode(name.c_str()))
                {
                    int numFiles = getNumFilesInDirectory(i);
                    for (int j = 1; j < numFiles; j++)
                    {
                        int realFileIndex = fatDirectoryTableBuffer[i].m0_firstFileIndex + j;
                        std::string name = "File " + std::to_string(realFileIndex);
                        if (getFileSize(j) <= 0)
                        {
                            name += "(deleted)";
                            continue;
                        }

                        if (gFilesystemDebugInfo.mFiles[realFileIndex].length()) {
                            name = gFilesystemDebugInfo.mFiles[realFileIndex];
                        }

                        ImGui::PushID(j);
                        if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf))
                        {
                            if (ImGui::BeginPopupContextItem(name.c_str()))
                            {
                                if (ImGui::MenuItem("Show Hex"))
                                {
                                    createHexView(i, j);
                                }
                                if (ImGui::MenuItem("Show Hex (decompress first)"))
                                {
                                    createHexView(i, j, true);
                                }
                                if (ImGui::MenuItem("Dump"))
                                {
                                    std::vector<u8> buffer;
                                    setCurrentDirectory(i, 0);
                                    readFile(j, buffer, 0, 0x80);
                                    FILE* fHandle = fopen("file.bin", "wb+");
                                    if(fHandle)
                                    {
                                        fwrite(&buffer[0], 1, getFileSize(j), fHandle);
                                        fclose(fHandle);
                                    }
                                }
                                if (ImGui::MenuItem("Rename")) {
                                    popupToOpen = "RenameFile";
                                    fileToRename = fatDirectoryTableBuffer[i].m0_firstFileIndex + j;
                                    strcpy(filenameBuffer, gFilesystemDebugInfo.mFiles[fileToRename].c_str());
                                }
                                ImGui::EndPopup();
                            }
                            ImGui::TreePop();
                        }
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
            }
        }
        
    }
    ImGui::End();

    if (popupToOpen.length())
    {
        ImGui::OpenPopup(popupToOpen.c_str());
        popupToOpen.clear();
    }

    //ImGui::SetNextWindowSize(ImVec2(500, 500));
    if (ImGui::BeginPopup("RenameFile"))
    {
        ImGui::PushID("##");

        if (ImGui::InputText("", filenameBuffer, sizeof(filenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("OK"))
        {
            gFilesystemDebugInfo.mFiles[fileToRename] = filenameBuffer;
            filessystemDebugInfo_save();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopID();
        ImGui::EndPopup();
    }

    for (int i = 0; i < gFileHexView.size(); i++)
    {
        if(gFileHexView[i])
        {
            if (!gFileHexView[i]->frame())
            {
                delete gFileHexView[i];
                gFileHexView[i] = nullptr;
            }
        }
    }
}

void batchStartLoadingFiles(sLoadingBatchCommands* pCommands, int param_2)
{
    // TODO: this is a quick and dirty implementation, not how the original code worked
    while (pCommands->m4_loadPtr)
    {
        readFile(pCommands->m0_fileIndex, *pCommands->m4_loadPtr, 0, 0);

        pCommands++;
    }
}

std::vector<std::vector<u8>> doPointerRelocationAndSplit(std::vector<u8>& inputData)
{
    std::vector<std::vector<u8>> splitBuffers;

    std::vector<u8>::iterator it = inputData.begin();
    u32 uVar1 = READ_LE_U32(it);
    if (uVar1)
    {
        it += 4;
        int uVar3 = 1;
        do
        {
            uVar3++;

            u32 startPos = READ_LE_U32(it);
            u32 endPos = READ_LE_U32(it + 4);
            u32 size = endPos - startPos;

            if (endPos < startPos) {
                size = 0;
            }

            std::vector<u8> outputBuffer;
            outputBuffer.reserve(size);

            for (int i = 0; i < size; i++) {
                outputBuffer.push_back(READ_LE_U8(inputData.begin() + startPos + i));
            }

            splitBuffers.push_back(outputBuffer);
            it += 4;
        } while (uVar3 <= uVar1);
    }

    return splitBuffers;
}

std::vector<std::vector<u8>::iterator> doPointerRelocation(std::vector<u8>& inputData)
{
    std::vector<std::vector<u8>::iterator> relocatedPointers;

    std::vector<u8>::iterator it = inputData.begin();
    u32 uVar1 = READ_LE_U32(it);
    if (uVar1)
    {
        it += 4;
        int uVar3 = 1;
        do
        {
            uVar3++;
            relocatedPointers.push_back(inputData.begin() + READ_LE_U32(it));
            it += 4;
        } while (uVar3 <= uVar1);
    }
    return relocatedPointers;
}