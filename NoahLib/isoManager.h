#pragma once

class c_isoFile
{
public:
    c_isoFile(FILE* fHandle)
    {
        m_fHandle = fHandle;
    }

    bool readData(int startSector, int size, std::vector<u8>& output);

private:
    FILE* m_fHandle;
};

class c_isoManager
{
public:
    static void init();
    static c_isoFile* getCurrentDisc();

private:
    static c_isoFile* c_isoManager::openIsoFile(std::string& fileName);

    static std::vector<c_isoFile*> g_isoFiles;
    static int m_currentDisc;
};
