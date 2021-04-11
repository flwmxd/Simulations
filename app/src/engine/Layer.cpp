
#include "Layer.h"
#include "opengl/Shader.h"


auto Layer::init() -> void
{
	flatShader = std::make_unique<opengl::Shader>("shader/vertFlatColor.glsl", "shader/fragFlatColor.glsl");
	flatShader->compile();
}

auto Layer::onImGui() -> void
{

}

auto Layer::onImGuiEnd() -> void
{

}

