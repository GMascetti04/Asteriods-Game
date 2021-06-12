#pragma once
#include "Engine/Core/Event.h"
#include "Engine/Debug/Profiler.h"

namespace GuiLayer {
	void Init();
	void Destroy();

	void Begin();
	void End();

	bool handledEvent(Event& event);
}