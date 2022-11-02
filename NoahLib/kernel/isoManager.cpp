#include "noahLib.h"
#include "isoManager.h"

std::vector<c_isoFile*> c_isoManager::g_isoFiles;
int c_isoManager::m_currentDisc = -1;

c_isoFile* c_isoManager::openIsoFile(const std::string& fileName)
{
    FILE* fHandle = fopen(fileName.c_str(), "rb");
    if (fHandle)
    {
        return new c_isoFile(fHandle);
    }
    return nullptr;
}

void c_isoManager::init()
{
    c_isoFile* pNewIso = openIsoFile(std::string("iso/1.bin"));
    if (pNewIso)
    {
        g_isoFiles.push_back(pNewIso);
    }

    if (g_isoFiles.size())
    {
        m_currentDisc = 0;
    }
}

c_isoFile* c_isoManager::getCurrentDisc()
{
    assert(m_currentDisc >= 0);
    return g_isoFiles[m_currentDisc];
}

bool c_isoFile::readData(int startSector, int size, std::vector<u8>& output)
{
    if (size < 0)
        return false;

    output.resize(size);

    fseek(m_fHandle, 0x930 * startSector, SEEK_SET);

    int outputPosition = 0;

    while (size)
    {
        fseek(m_fHandle, 24, SEEK_CUR); // skip sync + time mark

        int sizeToRead = size;
        if (sizeToRead > 0x800)
            sizeToRead = 0x800;

        fread(&output[outputPosition], 1, sizeToRead, m_fHandle);

        fseek(m_fHandle, 280, SEEK_CUR); // skip EDC/ECC

        size -= sizeToRead;
        outputPosition += sizeToRead;
    }

    return true;
}
