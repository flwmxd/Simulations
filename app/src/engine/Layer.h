

#pragma once
#include <memory>
#include "opengl/Shader.h"
#include "engine/Timer.h"
class Layer 
{
public:

	virtual auto init() -> void;
	virtual auto draw(const Timestep& timestep) -> void = 0;
	virtual auto onImGui() -> void;
	virtual auto onImGuiEnd() -> void;

	inline auto isActive() const { return active; }
	inline auto& getFlatShader() { return flatShader; }


protected:
	bool active = false;
	std::unique_ptr<opengl::Shader> flatShader;
};