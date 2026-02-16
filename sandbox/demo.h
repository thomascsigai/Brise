#pragma once

#include <app_context.h>

namespace BriseSandbox {
	
	class Demo {
	
	public:
		Demo() = default;

		virtual void PollEvent(AppContext* appState, SDL_Event* e) = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void Render(AppContext* appState) = 0;

		virtual const char* GetName() const = 0;
	};

}