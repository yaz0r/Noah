#include "noahLib.h"
#include "decompress.h"

void decompress(std::vector<u8>::const_iterator inputStream, std::vector<u8>& output)
{
    int totalSize = READ_LE_U32(inputStream);
    if (totalSize == 0)
    {
        output.resize(0);
        return;
    }

    if (output.size() <= 16)
    {
        output.resize(0);
        return;
    }

    inputStream += 4;

    std::vector<u8>::iterator outputStream = output.begin();
    do
    {
        int controlByte = READ_LE_U8(inputStream);
        inputStream++;
        u8 controlBit = controlByte & 1;
        u8 bitCounter = 8;

        do
        {
            controlByte >>= 1;
            bitCounter--;
            if (controlBit == 0)
            {
                *outputStream++ = *inputStream++;
            }
            else
            {
                u8 controlByte1 = *inputStream++;
                u8 controlByte0 = *inputStream++;

                int offset = ((controlByte0 & 0xF) << 8) | controlByte1;
                int length = (controlByte0 >> 4) + 3;

                std::vector<u8>::iterator readBackIterator = outputStream - offset;

                for (int i = 0; i < length; i++)
                {
                    *outputStream++ = *readBackIterator++;
                }
            }
            controlBit = controlByte & 1;
        } while (bitCounter != 0);
    } while (outputStream - output.begin() < totalSize);

    output.resize(totalSize);
}

void fieldDecompress(int size, std::vector<u8>::const_iterator inputStream, std::vector<u8>& output)
{
    decompress(inputStream, output);
}

std::vector<u8> mallocAndDecompress(std::vector<u8>::const_iterator input)
{
    int totalSize = READ_LE_U32(input);

    std::vector<u8> output;
    output.resize(totalSize);
    std::vector<u8>::const_iterator buffer = input;
    decompress(buffer, output);
    return output;
}
