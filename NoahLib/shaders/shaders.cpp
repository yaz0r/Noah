#include "noahLib.h"
#include "embeddedShadersMacro.h"

#if BGFX_PLATFORM_SUPPORTS_SPIRV
#include "shaders/generated/spirv/model3d_ps.sc.bin.h"
#include "shaders/generated/spirv/model3d_vs.sc.bin.h"
#include "shaders/generated/spirv/PSXGeneric_ps.sc.bin.h"
#include "shaders/generated/spirv/PSXGeneric_vs.sc.bin.h"
#include "shaders/generated/spirv/vertexColor_ps.sc.bin.h"
#include "shaders/generated/spirv/vertexColor_vs.sc.bin.h"
#endif

#if BGFX_PLATFORM_SUPPORTS_METAL
#include "shaders/generated/metal/model3d_ps.sc.bin.h"
#include "shaders/generated/metal/model3d_vs.sc.bin.h"
#include "shaders/generated/metal/PSXGeneric_ps.sc.bin.h"
#include "shaders/generated/metal/PSXGeneric_vs.sc.bin.h"
#include "shaders/generated/metal/vertexColor_ps.sc.bin.h"
#include "shaders/generated/metal/vertexColor_vs.sc.bin.h"
#endif

#if BGFX_PLATFORM_SUPPORTS_GLSL
#include "shaders/generated/glsl/model3d_ps.sc.bin.h"
#include "shaders/generated/glsl/model3d_vs.sc.bin.h"
#include "shaders/generated/glsl/PSXGeneric_ps.sc.bin.h"
#include "shaders/generated/glsl/PSXGeneric_vs.sc.bin.h"
#include "shaders/generated/glsl/vertexColor_ps.sc.bin.h"
#include "shaders/generated/glsl/vertexColor_vs.sc.bin.h"
#endif

#if BGFX_PLATFORM_SUPPORTS_DXBC
#include "shaders/generated/dx11/model3d_ps.sc.bin.h"
#include "shaders/generated/dx11/model3d_vs.sc.bin.h"
#include "shaders/generated/dx11/PSXGeneric_ps.sc.bin.h"
#include "shaders/generated/dx11/PSXGeneric_vs.sc.bin.h"
#include "shaders/generated/dx11/vertexColor_ps.sc.bin.h"
#include "shaders/generated/dx11/vertexColor_vs.sc.bin.h"
#endif

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
    BGFX_EMBEDDED_SHADER(model3d_ps),
    BGFX_EMBEDDED_SHADER(model3d_vs),
    BGFX_EMBEDDED_SHADER(PSXGeneric_ps),
    BGFX_EMBEDDED_SHADER(PSXGeneric_vs),
    BGFX_EMBEDDED_SHADER(vertexColor_ps),
    BGFX_EMBEDDED_SHADER(vertexColor_vs),

    BGFX_EMBEDDED_SHADER_END()
};

bgfx::ProgramHandle loadBgfxProgram(const std::string& VSFile, const std::string& PSFile)
{
    bgfx::RendererType::Enum type = bgfx::getRendererType();

    bgfx::ProgramHandle ProgramHandle = bgfx::createProgram(
        bgfx::createEmbeddedShader(s_embeddedShaders, type, VSFile.c_str())
        , bgfx::createEmbeddedShader(s_embeddedShaders, type, PSFile.c_str())
        , true
    );
    assert(bgfx::isValid(ProgramHandle));
    return ProgramHandle;
}
