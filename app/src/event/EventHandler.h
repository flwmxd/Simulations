//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cstdint>
#include <queue>
#include <functional>
#include <set>
#include "Event.h"


	class EventDispatcher;

	class EventHandler final
	{
	public:
		friend EventDispatcher;
		static constexpr int32_t PRIORITY_MAX = 0x1000;

		explicit EventHandler(int32_t initPriority = 0) : priority(initPriority)
		{
		}

		~EventHandler();

		auto remove() -> void;

		std::function<bool(MouseMoveEvent*)> mouseMoveHandler;
		std::function<bool(MouseClickEvent*)> mouseClickHandler;
		std::function<bool(MouseReleaseEvent*)> mouseRelaseHandler;
		std::function<bool(MouseScrolledEvent*)> mouseScrollHandler;

		std::function<bool(KeyPressedEvent*)> keyPressedHandler;
		std::function<bool(KeyReleasedEvent*)> keyReleasedHandler;

	private:
		int32_t priority;
		EventDispatcher *eventDispatcher = nullptr;
	};

