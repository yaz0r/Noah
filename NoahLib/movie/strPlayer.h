#pragma once

#include "noahLib.h"
#include "mdecDecoder.h"

// PS1 STR sector header (at offset 0 of the 2016-byte Mode 2 Form 2 data payload)
struct sStrSectorHeader {
    u16 magic;        // 0x0160 for video
    u16 chunkType;    // 0x8001 for MDEC video
    u16 sectorNumber; // sector index within this frame
    u16 numSectors;   // total sectors in this frame
    u32 frameNumber;  // frame index
    u32 demuxedSize;  // total MDEC data size for this frame
    u16 width;
    u16 height;
    // ... rest is MDEC data
};

struct sStrPlayer {
    void init();
    void shutdown();

    // Start playing a movie from the ISO
    // fileStartSector: absolute sector on disc where the STR file starts
    // fileSize: total size in bytes
    bool start(int fileStartSector, int fileSizeInSectors);

    // Decode next frame. Returns false when movie is done.
    bool decodeNextFrame();

    // Get current decoded frame as RGB
    const std::vector<u8>& getCurrentFrameRGB() const { return m_currentFrameRGB; }
    int getFrameWidth() const { return m_width; }
    int getFrameHeight() const { return m_height; }
    int getCurrentFrameNumber() const { return m_currentFrameNumber; }
    bool isFinished() const { return m_finished; }

private:
    // Read a raw 2352-byte sector from the ISO
    bool readRawSector(int sectorIndex, u8* out2352);

    sMdecDecoder m_decoder;

    FILE* m_isoFile;
    int m_fileStartSector;
    int m_fileSizeInSectors;
    int m_currentSector;
    int m_currentFrameNumber;
    int m_width;
    int m_height;
    bool m_finished;

    // Reassembly buffer for one complete frame's MDEC data
    std::vector<u8> m_frameBuffer;
    std::vector<u8> m_currentFrameRGB;
};
