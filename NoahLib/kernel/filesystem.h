#pragma once

class c_filesystemExplorer
{
public:
    static void frame();
};

struct s_fileTableEntry
{
    int m0_startSector; // is actually 3 bytes in original, but little point to keep that optimization
    int m3_size;
};

struct s_directoryEntry
{
    s16 m0_firstFileIndex;

    std::string mName;
    int mNumFilesInDirectory;
};

extern std::vector<s_fileTableEntry> fatFileTableBuffer;
extern std::vector<s_directoryEntry> fatDirectoryTableBuffer;

void initCDAndFileSystem(std::vector<s_fileTableEntry>* fileTable, std::vector<s_directoryEntry>* directoryTable, int);

int getFileSize(int fileIndex);
int getFileSizeAligned(int fileIndex);
int getFileStartSector(int fileIndex);
void readFile(int fileIndex, std::vector<u8>& output, int, int);
int setCurrentDirectory(int directory, int offset);
void waitReadCompletion(int);
