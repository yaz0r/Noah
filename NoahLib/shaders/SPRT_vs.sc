$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_texcoord0, v_texcoord1, v_texcoord2

#include "bgfx_shader.sh"

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position.xyz, 1));
    v_texcoord0 = a_texcoord0;
    v_texcoord1 = a_texcoord1;
    v_texcoord2 = a_texcoord2;
}
