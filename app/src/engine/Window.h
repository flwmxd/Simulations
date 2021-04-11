//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "opengl/OGL.h"

class Window
{
public:
	auto init(int32_t width,int32_t height) -> void;
	auto update() -> void;
	auto setEventCallback() -> void;
	auto setWidthAndHeight(int32_t width, int32_t height) -> void;
	inline auto getWidth() const { return width; };
	inline auto getHeight() const { return height; }
private:
	int32_t width;
	int32_t height;

	GLFWwindow* window = nullptr;
};