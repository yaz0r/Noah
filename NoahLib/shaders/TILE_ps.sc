$input v_color0

#include "bgfx_shader.sh"

highp USAMPLER2D(s_PSXVram, 0);

int readU8FromPDXVram(int x, int y)
{
    uvec4 texel = texelFetch(s_PSXVram, ivec2(x,y), 0);
    return texel.r;
}

int readU4FromPDXVram(float x, float y)
{
    uvec4 texel = texelFetch(s_PSXVram, ivec2(x/2,y), 0);

    if((int)x & 1)
    {
        return (texel.r >> 4) & 0xF;
        
    }
    else
    {
        return texel.r & 0xF;
    }
}

int getTexturePageXBase(int4 config)
{
    return 64 * (config.x & 0xF);
}

int getTexturePageYBase(int4 config)
{
    return 256 * ((config.x>>4) & 0x1);
}

int getTexturePageColors(int4 config)
{
    return ((config.x>>7) & 0x3);
}

float4 getCLUTForColor(int2 clutConfig, int colorIndex)
{
//    return float4(colorIndex / (float)0xF, 0,0,1);

    int CLUT0 = readU8FromPDXVram((clutConfig.x * 16) * 2 + colorIndex * 2, clutConfig.y);
    int CLUT1 = readU8FromPDXVram((clutConfig.x * 16) * 2 + colorIndex * 2 + 1, clutConfig.y);

    int CLUT = CLUT0 | (CLUT1 << 8);

    int R = (CLUT >> 0) & 0x1F;
    int G = (CLUT >> 5) & 0x1F;
    int B = (CLUT >> 10) & 0x1F;
    int A = (CLUT >> 15) & 1;

    float4 result;
    result.x = R / (float)0x1F;
    result.y = G / (float)0x1F;
    result.z = B / (float)0x1F;
    result.w = 1-A;

    return result;
}

void main()
{
    gl_FragColor = v_color0;
}
