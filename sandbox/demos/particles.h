#pragma once

#include<SDL3/SDL.h>

#include <demo.h>
#include <particle.h>

namespace BriseSandbox {
	
	class ParticlesDemo : public Demo {

	// Attributes
	private:
		Brise::Particle p;

	public:
		ParticlesDemo() { Init(); }
		~ParticlesDemo() { Shutdown(); }

		void PollEvent(AppContext* appstate, SDL_Event* event) override {

		}

		void Update() override {

		}

		void Render(AppContext* appstate) override {
			SDL_SetRenderDrawColor(appstate->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			SDL_RenderRect(appstate->renderer, new SDL_FRect(0, 0, 100, 100));
		}
		
	private:
		void Init() {
			p = Brise::Particle();
		}

		void Shutdown() {

		}
	};
}