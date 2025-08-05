$input a_position, a_texcoord0, a_texcoord1, a_texcoord2, a_texcoord3, a_color0
$output v_texcoord0, v_texcoord1, v_texcoord2, v_textureWindow, v_color0

#include "bgfx_shader.sh"

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position.xyz, 1));
    v_color0 = vec4(a_color0);
    v_texcoord0 = a_texcoord0;
    v_texcoord1 = a_texcoord1;
    v_texcoord2 = a_texcoord2;
    v_textureWindow = a_texcoord3;
}
