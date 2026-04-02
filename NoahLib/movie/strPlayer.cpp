#include "noahLib.h"
#include "strPlayer.h"
#include "kernel/isoManager.h"
#include "kernel/audio/spu.h"
#include "kernel/audio/spuEmu/spu.h"

void sStrPlayer::init()
{
    m_decoder.init();
    m_isoFile = nullptr;
    m_fileStartSector = 0;
    m_fileSizeInSectors = 0;
    m_currentSector = 0;
    m_currentFrameNumber = -1;
    m_width = 0;
    m_height = 0;
    m_finished = true;
    m_audioEnabled = false;
    m_xaSectorCount = 0;
}

void sStrPlayer::shutdown()
{
    if (m_audioEnabled) {
        std::lock_guard<std::mutex> lock(spuMutex);
        emulatedSpuDevice.flush_xa();
    }
    if (m_isoFile) {
        fclose(m_isoFile);
        m_isoFile = nullptr;
    }
    m_frameBuffer.clear();
    m_currentFrameRGB.clear();
    m_finished = true;
    m_audioEnabled = false;
}

bool sStrPlayer::start(int fileStartSector, int fileSizeInSectors, bool enableAudio)
{
    m_isoFile = fopen("iso/1.bin", "rb");
    if (!m_isoFile) {
        Noah_Log("STR: failed to open iso/1.bin\n", 0);
        return false;
    }
    m_fileStartSector = fileStartSector;
    m_fileSizeInSectors = fileSizeInSectors;
    m_currentSector = 0;
    m_currentFrameNumber = -1;
    m_width = 0;
    m_height = 0;
    m_finished = false;
    m_audioEnabled = enableAudio;
    m_xaSectorCount = 0;
    m_frameBuffer.clear();
    m_currentFrameRGB.clear();

    if (m_audioEnabled) {
        // Set SPU CD volume registers so XA audio is audible
        // Ghidra: the game sets these via SPU register writes
        std::lock_guard<std::mutex> lock(spuMutex);
        emulatedSpuDevice.flush_xa();
        emulatedSpuDevice.write(0x1B0 / 2, 0x7FFF); // cdvol_l
        emulatedSpuDevice.write(0x1B2 / 2, 0x7FFF); // cdvol_r
    }

    return true;
}

bool sStrPlayer::readRawSector(int sectorIndex, u8* out2352)
{
    if (!m_isoFile) return false;

    int absoluteSector = m_fileStartSector + sectorIndex;
    fseek(m_isoFile, (long)absoluteSector * 0x930, SEEK_SET);
    size_t bytesRead = fread(out2352, 1, 0x930, m_isoFile);
    return bytesRead == 0x930;
}

bool sStrPlayer::decodeNextFrame()
{
    if (m_finished) return false;

    // Scan sectors looking for video sectors of the next frame
    int targetFrame = m_currentFrameNumber + 1;
    bool frameStarted = false;
    int expectedSectors = 0;
    int collectedSectors = 0;

    while (m_currentSector < m_fileSizeInSectors) {
        u8 rawSector[0x930];
        if (!readRawSector(m_currentSector, rawSector)) {
            m_finished = true;
            return false;
        }
        m_currentSector++;

        // Subheader is at offset 16 (after 12 sync + 4 header)
        // subheader[2] = submode byte: bit 2 = audio, bit 1 = video
        const u8 submode = rawSector[18];

        // Check for audio sector (submode bit 2)
        if (m_audioEnabled && (submode & 0x04)) {
            // Feed XA ADPCM audio sector to SPU
            // play_xa expects pointer to subheader (offset 16 in raw sector)
            std::lock_guard<std::mutex> lock(spuMutex);
            emulatedSpuDevice.play_xa(m_xaSectorCount, rawSector + 16);
            m_xaSectorCount++;
            continue;
        }

        // STR sector data starts at offset 24 (after 12 sync + 4 header + 8 subheader)
        const u8* sectorData = rawSector + 24; // skip sync + header + subheader
        const sStrSectorHeader* strHeader = (const sStrSectorHeader*)sectorData;

        // Check for video sector magic
        if (strHeader->magic != 0x0160) {
            continue;
        }
        if (strHeader->chunkType != 0x8001) {
            continue;
        }

        u32 frameNum = strHeader->frameNumber;
        if ((int)frameNum < targetFrame) {
            // Old frame, skip
            continue;
        }
        if ((int)frameNum > targetFrame) {
            // We missed the target frame somehow, adjust
            targetFrame = frameNum;
        }

        if (!frameStarted) {
            frameStarted = true;
            expectedSectors = strHeader->numSectors;
            m_width = strHeader->width;
            m_height = strHeader->height;
            // Allocate based on sector count × data per sector
            // Standard STR: 2016 bytes of MDEC data per sector (0x20..0x7FF in 2048-byte region)
            int bufferSize = expectedSectors * 2016;
            m_frameBuffer.resize(bufferSize);
            memset(m_frameBuffer.data(), 0, m_frameBuffer.size());
            collectedSectors = 0;
        }

        // Copy MDEC data from this sector into the frame buffer
        // MDEC data starts after the 32-byte STR sector header
        // Mode 2 Form 2 sectors have 2328 bytes of data, minus 32 byte STR header = 2296
        int sectorIdx = strHeader->sectorNumber;
        const u8* mdecData = sectorData + 32;
        int mdecDataSize = 2016; // 2048 - 32 (STR header), standard STR demux size

        int destOffset = sectorIdx * 2016;
        int copySize = mdecDataSize;
        if (destOffset + copySize > (int)m_frameBuffer.size()) {
            copySize = (int)m_frameBuffer.size() - destOffset;
        }
        if (copySize > 0 && destOffset >= 0) {
            memcpy(m_frameBuffer.data() + destOffset, mdecData, copySize);
        }

        collectedSectors++;
        if (collectedSectors >= expectedSectors) {
            // Full frame collected, decode it
            m_currentFrameNumber = targetFrame;

            bool result = m_decoder.decodeFrame(m_frameBuffer.data(), (int)m_frameBuffer.size(),
                                                m_width, m_height, m_currentFrameRGB);
            return result;
        }
    }

    // Reached end of file
    m_finished = true;
    return false;
}
