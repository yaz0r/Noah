$input v_texcoord0, v_texcoord1, v_texcoord2, v_textureWindow

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

int2 getTexturePageBase(int4 config)
{
    int2 output;
    output.x = getTexturePageXBase(config);
    output.y = getTexturePageYBase(config);
    return output;
}

int4 getTextureWindow(int config)
{
    int4 output;

    output.x = ((config >> 0xA) << 3) & 0xFF;
    output.y = ((config >> 0xF) << 3) & 0xFF;
    output.z = -((config >> 0x5) << 3) & 0xFF;
    output.w = -((config) << 3) & 0xFF;

    if (output.z == 0)
    {
        output.z = 256;
    }
    if (output.w == 0)
    {
        output.w = 256;
    }
    return output;
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

    if(CLUT == 0)
        discard;

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
    int texturePageColor = getTexturePageColors(v_texcoord2);
    int2 texturePageBase = getTexturePageBase(v_texcoord2);
    int4 textureWindow = getTextureWindow(v_textureWindow.x | (v_textureWindow.y << 8) | (v_textureWindow.z << 16) | (v_textureWindow.w << 24));

    int2 texcoordInPage = (int2)v_texcoord0.xy;
    texcoordInPage.x = (texcoordInPage.x % textureWindow.w) + textureWindow.x;
    texcoordInPage.y = (texcoordInPage.y % textureWindow.z) + textureWindow.y;

    if(texturePageColor == 0) // 4bit
    {
        int colorIndex = readU4FromPDXVram(texturePageBase.x * 4 + texcoordInPage.x, texturePageBase.y + texcoordInPage.y);
        float4 color = getCLUTForColor(v_texcoord1, colorIndex);
        gl_FragColor = color;
    }
    else
    {
        int colorIndex = readU8FromPDXVram(texturePageBase.x * 2 + texcoordInPage.x, texturePageBase.y + texcoordInPage.y);
        float4 color = getCLUTForColor(v_texcoord1, colorIndex);
        gl_FragColor = color;
    }
}
