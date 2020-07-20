#include "noahLib.h"
#include "filesystem.h"
#include "isoManager.h"
#include "imgui.h"

#include "../imgui_club/imgui_memory_editor/imgui_memory_editor.h"

std::vector<s_fileTableEntry> fatFileTableBuffer;
std::vector<s_directoryEntry> fatDirectoryTableBuffer;

void readRawSectorsFromDisk(int startSector, std::vector<u8>& output, int size, int, int)
{
    c_isoManager::getCurrentDisc()->readData(startSector, size, output);
}

void waitReadCompletion(int)
{
    MissingCode();
}

int getNumFilesInDirectory(int directory)
{
    if (fatDirectoryTableBuffer[directory].m0_firstFileIndex < 0)
        return 0;

    return fatDirectoryTableBuffer[directory].mNumFilesInDirectory;
}

void initCDAndFileSystem(std::vector<s_fileTableEntry>* fileTable, std::vector<s_directoryEntry>* directoryTable, int mode)
{
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
        size2 + size + 6;
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
    c_fileHexView(int directory, int file)
    {
        char buffer[256];
        sprintf(buffer, "%d/%d", directory, file);
        mName = buffer;

        setCurrentDirectory(directory, 0);
        readFile(file, mBuffer, 0, 0x80);
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

void createHexView(int directory, int file)
{
    gFileHexView.push_back(new c_fileHexView(directory, file));
}

void c_filesystemExplorer::frame()
{
    if (ImGui::Begin("FileSystemExplorer"))
    {
        for (int i = 0; i < fatDirectoryTableBuffer.size() - 1; i++)
        {
            setCurrentDirectory(i, 0);
            if(fatDirectoryTableBuffer[i].mNumFilesInDirectory)
            {
                if (ImGui::TreeNode(fatDirectoryTableBuffer[i].mName.c_str()))
                {
                    int numFiles = getNumFilesInDirectory(i);
                    for (int j = 0; j < numFiles; j++)
                    {
                        char buffer[256];
                        if (getFileSize(j) <= 0)
                        {
                            sprintf(buffer, "file %d (deleted)", j);
                        }
                        else
                        {
                            sprintf(buffer, "file %d", j);
                        }

                        std::string name = buffer;

                        if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf))
                        {
                            if (ImGui::BeginPopupContextItem(name.c_str()))
                            {
                                if (ImGui::MenuItem("Show Hex"))
                                {
                                    createHexView(i, j);
                                }
                                if (ImGui::MenuItem("Dump"))
                                {
                                    std::vector<u8> buffer;
                                    setCurrentDirectory(i, 0);
                                    readFile(j, buffer, 0, 0x80);
                                    FILE* fHandle = fopen("file.bin", "wb+");
                                    fwrite(&buffer[0], 1, getFileSize(j), fHandle);
                                    fclose(fHandle);
                                }
                                ImGui::EndPopup();
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
            }
        }
        
    }
    ImGui::End();

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
