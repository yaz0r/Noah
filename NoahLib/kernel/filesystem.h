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
    virtual void init(const std::vector<u8>& data) = 0;
    virtual void clear() {}
    virtual ~sLoadableData() {}
};

struct sLoadableDataRaw : public sLoadableData  {
    virtual void init(const std::vector<u8>& inputData) override {
        if (m_rawData.size() == inputData.size()) { // TODO: to prevent reallocation that would break iterators
            for (int i = 0; i < inputData.size(); i++) {
                m_rawData[i] = inputData[i];
            }
            return;
        }
        m_rawData.resize(inputData.size());
        memcpy(m_rawData.data(), inputData.data(), inputData.size());
    }
    virtual void clear() override {
        m_rawData.clear();
    }
    void resize(size_t size) {
        m_rawData.resize(size);
    }
    sLoadableDataRaw() {
    }
    sLoadableDataRaw(size_t size) {
        m_rawData.resize(size);
    }
    std::span<u8>::iterator beginSpan() {
        std::span<u8> temp(m_rawData.begin(), m_rawData.end());
        return temp.begin();
    }

    const std::vector<u8>& getRawData() {
        return m_rawData;
    }
private:
    std::vector<u8> m_rawData;
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
//void readFile(int fileIndex, std::vector<u8>& output, int, int);
void readFile(int fileIndex, sLoadableData& output, int, int);
int setCurrentDirectory(int directory, int offset);
int isCDBusy();
int waitReadCompletion(int);
std::vector<std::vector<u8>::const_iterator> doPointerRelocation(const std::vector<u8>& inputData);
std::vector<std::vector<u8>> doPointerRelocationAndSplit(std::vector<u8>::const_iterator inputData);

void filessystemDebugInfo_load();
void filessystemDebugInfo_save();