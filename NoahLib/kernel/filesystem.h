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

s32 getCdromFnd();

struct sLoadableData {
    virtual void init(std::vector<u8>& data) = 0;
};

struct sLoadableDataRaw : public sLoadableData {
    virtual void init(std::vector<u8>& data) override {
        if (mData.size() == data.size()) { // TODO: to prevent reallocation that would break iterators
            for (int i = 0; i < data.size(); i++) {
                mData[i] = data[i];
            }
            return;
        }
        mData = data;
    }
    sLoadableDataRaw() {

    }
    sLoadableDataRaw(size_t size) {
        resize(size);
    }
    void resize(size_t newSize) {
        mData.resize(newSize);
    }
    void clear() {
        mData.clear();
    }
    std::vector<u8>::iterator begin() {
        return mData.begin();
    }
    size_t size() {
        return mData.size();
    }
    std::vector<u8> mData;
};

struct sLoadingBatchCommands
{
    u16 m0_fileIndex;
    sLoadableData* m4_loadPtr;
};
void batchStartLoadingFiles(sLoadingBatchCommands* pCommands, int param_2);

int getCurrentDirectory(int* param_1, int* param_2);

void initCDAndFileSystem(std::vector<s_fileTableEntry>* fileTable, std::vector<s_directoryEntry>* directoryTable, int);

int getNegativeFileSize(int param_1);
int getFileSize(int fileIndex);
int getFileSizeAligned(int fileIndex);
int getFileStartSector(int fileIndex);
void readFile(int fileIndex, std::vector<u8>& output, int, int);
int setCurrentDirectory(int directory, int offset);
int isCDBusy();
int waitReadCompletion(int);
std::vector<std::vector<u8>::iterator> doPointerRelocation(std::vector<u8>& inputData);
std::vector<std::vector<u8>> doPointerRelocationAndSplit(std::vector<u8>& inputData);

void filessystemDebugInfo_load();
void filessystemDebugInfo_save();