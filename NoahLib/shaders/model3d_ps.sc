$input v_color0, v_texcoord0

#include "bgfx_shader.sh"

SAMPLER2D(s_texture, 0);

void main()
{
    vec4 txcol = texture2D(s_texture, v_texcoord0.xy);
    gl_FragColor = txcol * v_texcoord0.z + v_color0 * (1-v_texcoord0.z);
    gl_FragColor.w = 1;

}
