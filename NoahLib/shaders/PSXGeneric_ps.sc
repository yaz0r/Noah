$input v_texcoord0, v_texcoord1, v_texcoord2, v_textureWindow, v_color0

#include "bgfx_shader.sh"

USAMPLER2D(s_PSXVram, 0);

int readU8FromPDXVram(int x, int y)
{
    uvec4 texel = texelFetch(s_PSXVram, ivec2(x,y), 0);
    return int(texel.r);
}

int readU4FromPDXVram(int x, int y)
{
    uvec4 texel = texelFetch(s_PSXVram, ivec2(int(uint(x)/uint(2)),y), 0);

    if(bool(x & 1))
    {
        return int(texel.r >> 4) & int(0xF);
        
    }
    else
    {
        return int(texel.r) & int(0xF);
    }
}

int readU16FromPSXVram(int x, int y)
{
    int CLUT0 = readU8FromPDXVram(x * 2, y);
    int CLUT1 = readU8FromPDXVram(x * 2 + 1, y);

    int CLUT = CLUT0 | (CLUT1 << 8);
    return CLUT;
}

int getTexturePageXBase(ivec4 config)
{
    return int(64) * (config.x & int(0xF));
}

int getTexturePageYBase(ivec4 config)
{
    return int(256) * ((config.x>>4) & int(0x1));
}

ivec2 getTexturePageBase(ivec4 config)
{
    ivec2 tPageBase;
    tPageBase.x = getTexturePageXBase(config);
    tPageBase.y = getTexturePageYBase(config);
    return tPageBase;
}

ivec4 getTextureWindow(int config)
{
    ivec4 textureWindow;

    textureWindow.x = ((config >> 0xA) << 3) & 0xFF;
    textureWindow.y = ((config >> 0xF) << 3) & 0xFF;
    textureWindow.z = -((config >> 0x5) << 3) & 0xFF;
    textureWindow.w = -((config) << 3) & 0xFF;

    if (textureWindow.z == 0)
    {
        textureWindow.z = 256;
    }
    if (textureWindow.w == 0)
    {
        textureWindow.w = 256;
    }
    return textureWindow;
}

int getTexturePageColors(ivec4 config)
{
    int tpage = (config.y << 8) | config.x;
    return ((tpage>>7) & int(0x3));
}

vec4 unpackU16Color(int colorInput) {
    int R = (colorInput >> 0) & int(0x1F);
    int G = (colorInput >> 5) & int(0x1F);
    int B = (colorInput >> 10) & int(0x1F);
    int A = (colorInput >> 15) & int(0x1);

    vec4 result;
    result.x = float(R) / float(0x1F);
    result.y = float(G) / float(0x1F);
    result.z = float(B) / float(0x1F);
    result.w = 1.f-float(A);

    return result;
}

vec4 getCLUTForColor(ivec2 clutConfig, int colorIndex)
{
    int CLUT = readU16FromPSXVram(clutConfig.x * 16 + colorIndex, clutConfig.y);

    if(CLUT == int(0))
        discard;

    return unpackU16Color(CLUT);
}

void main()
{
    int code = int(v_color0.w);
    bool textureBlendingDisabled = bool(code & 0x1);
    bool transparent = bool(code & 0x2);
    bool textured = bool(code & 0x4);
    bool gouraud = bool(code & 0x10);

    vec4 color = vec4(0,0,0,1);

    if(textured) {
        int texturePageColor = getTexturePageColors(v_texcoord2);
        ivec2 texturePageBase = getTexturePageBase(v_texcoord2);
        ivec4 textureWindow = getTextureWindow(v_textureWindow.x | (v_textureWindow.y << 8) | (v_textureWindow.z << 16) | (v_textureWindow.w << 24));

        ivec2 texcoordInPage = ivec2(v_texcoord0.xy);
        texcoordInPage.x = int((uint(texcoordInPage.x) % uint(textureWindow.w)) + uint(textureWindow.x));
        texcoordInPage.y = int((uint(texcoordInPage.y) % uint(textureWindow.z)) + uint(textureWindow.y));

        if(!gl_FrontFacing)
        {
            //if(dFdx(v_texcoord0.x) < 0){ texcoordInPage.x++;}
            if(dFdy(v_texcoord0.y) > 0.f){ texcoordInPage.y++;}
            texcoordInPage.x++;
            //texcoordInPage.y++;
        }
        else {
            if(dFdx(v_texcoord0.x) < 0.f){ texcoordInPage.x++;}
            if(dFdy(v_texcoord0.y) > 0.f){ texcoordInPage.y++;}
        }



        if(texturePageColor == int(0)) // 4bit
        {
            int texcoordX = texturePageBase.x * 4 + texcoordInPage.x;
            int texcoordY = texturePageBase.y + texcoordInPage.y;
            int colorIndex = readU4FromPDXVram(texcoordX, texcoordY);
            color = getCLUTForColor(v_texcoord1, colorIndex);        
        }
        else if(texturePageColor == int(1)) // 8bit
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
        color.xyz = v_color0.xyz / float(0xFF);
    }


    //colorFromTexture.xyz *= v_color0/(float)0x80;

    gl_FragColor = color;
}
