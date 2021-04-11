//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <queue>
#include <functional>
#include <string>
#include "engine/KeyCodes.h"
#include "glm/glm.hpp"

#define GENERATE_EVENT_CLASS_TYPE(type) static EventType getEventType() { return EventType::type; }\
								virtual auto getType() const  -> EventType  override { return getEventType(); } \
								virtual auto getName() const -> const char * override { return #type; }
enum class EventType
{
	None,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	
	KeyPressed,
	KeyReleased,
	MouseClicked,
	MouseReleased,
	MouseMove,
	MouseScrolled,
};

class Event
{
public:
	virtual auto getType() const->EventType = 0;
	virtual auto getName() const -> const char* = 0;
};


class MouseMoveEvent : public Event
{
public:
	glm::vec2 position;
	GENERATE_EVENT_CLASS_TYPE(MouseMove);
};


class MouseClickEvent : public Event
{
public:
	int32_t buttonId = -1;
	glm::vec2 position;
	GENERATE_EVENT_CLASS_TYPE(MouseClicked);
};

class MouseReleaseEvent : public Event
{
public:
	int32_t buttonId = -1;
	glm::vec2 position;
	GENERATE_EVENT_CLASS_TYPE(MouseReleased);
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset)
		: xOffset(xOffset), yOffset(yOffset) {}

	inline auto getXOffset() const { return xOffset; }
	inline auto getYOffset() const { return yOffset; }

	GENERATE_EVENT_CLASS_TYPE(MouseScrolled);
private:
	float xOffset;
	float yOffset;
};

class KeyEvent : public Event
{
public:
	inline auto getKeyCode() const { return keyCode; }

protected:
	KeyEvent(KeyCode keycode)
		: keyCode(keycode) {}

	KeyCode keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyCode keycode, int repeatCount)
		: KeyEvent(keycode), repeatCount(repeatCount) {}

	inline auto getRepeatCount() const { return repeatCount; }


	GENERATE_EVENT_CLASS_TYPE(KeyPressed);
private:
	int32_t repeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(KeyCode keycode)
		: KeyEvent(keycode) {}

	GENERATE_EVENT_CLASS_TYPE(KeyReleased);
};

