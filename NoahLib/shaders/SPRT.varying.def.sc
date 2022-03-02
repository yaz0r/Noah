vec3 a_position  : POSITION;
vec4 a_normal    : NORMAL;
vec4 a_color0    : COLOR0;
vec2 a_texcoord0 : TEXCOORD0;
uint2 a_texcoord1 : TEXCOORD1;
uint4 a_texcoord2 : TEXCOORD2;
uint4 a_texcoord3 : TEXCOORD3;

vec4 v_color0    : COLOR0    = vec4(1.0, 0.0, 0.0, 1.0);
vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 1.0);
vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
uint2 v_texcoord1 : TEXCOORD1 = uint2(0,0);
uint4 v_texcoord2 : TEXCOORD2 = uint4(0,0,0,0);
uint4 v_textureWindow : TEXCOORD3 = uint4(0,0,0,0);
float v_depth : TEXCOORD1 = float(0);

