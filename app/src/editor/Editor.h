
#pragma once
#include <cstdint>
#include "event/EventHandler.h"
class Editor
{
public:
	Editor(uint32_t width, uint32_t height);
	auto init() -> void;
	auto onImGui() -> void;
private:
	auto drawMenuBar() -> void;

	EventHandler handler;

};