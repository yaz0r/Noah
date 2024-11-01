$input v_texcoord0, v_texcoord1, v_texcoord2, v_textureWindow, v_color0

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

int readU16FromPSXVram(int x, int y)
{
    int CLUT0 = readU8FromPDXVram(x * 2, y);
    int CLUT1 = readU8FromPDXVram(x * 2 + 1, y);

    int CLUT = CLUT0 | (CLUT1 << 8);
    return CLUT;
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
    int tpage = (config.y << 8) | config.x;
    return ((tpage>>7) & 0x3);
}

float4 unpackU16Color(int colorInput) {
    int R = (colorInput >> 0) & 0x1F;
    int G = (colorInput >> 5) & 0x1F;
    int B = (colorInput >> 10) & 0x1F;
    int A = (colorInput >> 15) & 1;

    float4 result;
    result.x = R / (float)0x1F;
    result.y = G / (float)0x1F;
    result.z = B / (float)0x1F;
    result.w = 1-A;

    return result;
}

float4 getCLUTForColor(int2 clutConfig, int colorIndex)
{
    int CLUT = readU16FromPSXVram(clutConfig.x * 16 + colorIndex, clutConfig.y);

    if(CLUT == 0)
        discard;

    return unpackU16Color(CLUT);
}

void main()
{
    int code = v_color0.w;
    bool textureBlendingDisabled = code & 0x1;
    bool transparent = code & 0x2;
    bool textured = code & 0x4;
    bool gouraud = code & 0x10;

    float4 color = float4(0,0,0,1);

    if(textured) {
        int texturePageColor = getTexturePageColors(v_texcoord2);
        int2 texturePageBase = getTexturePageBase(v_texcoord2);
        int4 textureWindow = getTextureWindow(v_textureWindow.x | (v_textureWindow.y << 8) | (v_textureWindow.z << 16) | (v_textureWindow.w << 24));

        int2 texcoordInPage = (int2)v_texcoord0.xy;
        texcoordInPage.x = (texcoordInPage.x % textureWindow.w) + textureWindow.x;
        texcoordInPage.y = (texcoordInPage.y % textureWindow.z) + textureWindow.y;

        if(!gl_FrontFacing)
        {
            //if(dFdx(v_texcoord0.x) < 0){ texcoordInPage.x++;}
            if(dFdy(v_texcoord0.y) > 0){ texcoordInPage.y++;}
            texcoordInPage.x++;
            //texcoordInPage.y++;
        }
        else {
            if(dFdx(v_texcoord0.x) < 0){ texcoordInPage.x++;}
            if(dFdy(v_texcoord0.y) > 0){ texcoordInPage.y++;}
        }



        if(texturePageColor == 0) // 4bit
        {
            int texcoordX = texturePageBase.x * 4 + texcoordInPage.x;
            int texcoordY = texturePageBase.y + texcoordInPage.y;
            int colorIndex = readU4FromPDXVram(texcoordX, texcoordY);
            color = getCLUTForColor(v_texcoord1, colorIndex);        
        }
        else if(texturePageColor == 1) // 8bit
        {
            int texcoordX = texturePageBase.x * 2 + texcoordInPage.x;
            int texcoordY = texturePageBase.y + texcoordInPage.y;
            int colorIndex = readU8FromPDXVram(texcoordX, texcoordY);
            color = getCLUTForColor(v_texcoord1, colorIndex);
        }
        else
        {
            color = unpackU16Color(readU16FromPSXVram(texturePageBase.x + texcoordInPage.x, texturePageBase.y + texcoordInPage.y));
        }

        if(!textureBlendingDisabled) {
            //color.xyz *= v_color0.xyz / (float)0x80;
            //color.xyz = (((color.xyz * 255.f) * v_color0.xyz) / 128.f) / 255.f;
        }
    }
    else {
        color.xyz = v_color0.xyz / (float)0xFF;
    }


    //colorFromTexture.xyz *= v_color0/(float)0x80;

    gl_FragColor = color;
}
