//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////

#include "EventHandler.h"
#include "EventDispatcher.h"


EventHandler::~EventHandler()
{
	if (eventDispatcher)
		eventDispatcher->removeEventHandler(this);
}

auto EventHandler::remove() -> void
{
	if (eventDispatcher)
	{
		eventDispatcher->removeEventHandler(this);
		eventDispatcher = nullptr;
	}
}

