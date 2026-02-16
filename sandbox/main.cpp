#define SDL_MAIN_USE_CALLBACKS  // This is necessary for the new callbacks API. To use the legacy API, don't define this. 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>

#include <memory>

#include <app_context.h>
#include <particles.h>
#include <ballistics.h>
#include <springs.h>
#include <buoyancy.h>
#include <collision.h>

constexpr uint32_t windowStartWidth = 1600;
constexpr uint32_t windowStartHeight = 900;
const char* windowTitle = "Brise Sandbox";

std::unique_ptr<BriseSandbox::Demo> currentDemo;

SDL_AppResult SDL_Fail() {
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // init the library, here we make a window so we only need the Video capabilities.
    if (not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        return SDL_Fail();
    }

    // create a window
    SDL_Window* window = SDL_CreateWindow(windowTitle, windowStartWidth, windowStartHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (not window) {
        return SDL_Fail();
    }

    // create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (not renderer) {
        return SDL_Fail();
    }

    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth) {
            SDL_Log("This is a highdpi environment.");
        }
    }

    // set up the application data
    *appstate = new AppContext{
       .window = window,
       .renderer = renderer
    };

    // set up first demo played
    currentDemo = std::make_unique<BriseSandbox::ParticlesDemo>();

    SDL_SetRenderVSync(renderer, 1);   // enable vysnc

    SDL_Log("Application started successfully!");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    auto* app = (AppContext*)appstate;

    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_1) {
            currentDemo = std::make_unique<BriseSandbox::ParticlesDemo>();
        }
        else if (event->key.scancode == SDL_SCANCODE_2) {
            currentDemo = std::make_unique<BriseSandbox::BallisticsDemo>();
        }
        else if (event->key.scancode == SDL_SCANCODE_3) {
            currentDemo = std::make_unique<BriseSandbox::SpringsDemo>();
        }
        else if (event->key.scancode == SDL_SCANCODE_4) {
            currentDemo = std::make_unique<BriseSandbox::BuoyancyDemo>();
        }
        else if (event->key.scancode == SDL_SCANCODE_5) {
            currentDemo = std::make_unique<BriseSandbox::CollisionDemo>();
        }
    }

    // Poll app events
    currentDemo->PollEvent(app, event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* app = (AppContext*)appstate;

    // High-resolution delta time calculation (SDL3)
    static uint64_t lastCounter = SDL_GetPerformanceCounter();
    const uint64_t currentCounter = SDL_GetPerformanceCounter();

    const uint64_t frequency = SDL_GetPerformanceFrequency();
    const double deltaTime =
        static_cast<double>(currentCounter - lastCounter) /
        static_cast<double>(frequency);

    lastCounter = currentCounter;

    //FPS Counter

    static double fpsTimer = 0.0;
    static int frameCount = 0;
    static double fps = 0.0;

    fpsTimer += deltaTime;
    frameCount++;

    if (fpsTimer >= 1.0)
    {
        fps = frameCount / fpsTimer;
        fpsTimer = 0.0;
        frameCount = 0;
    }

    // Update current demo
    currentDemo->Update(deltaTime);

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(app->renderer);

    Utils::DrawDebugAxis(appstate);

    // Render debug text
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    char text[256];
    SDL_snprintf(
        text,
        sizeof(text),
        "FPS: %.1f\n"
        "Delta: %.2f ms\n",
        fps,
        deltaTime * 1000
    );

    SDL_RenderDebugText(app->renderer, 10, 10, text);

    // Render demo name
    SDL_SetRenderScale(app->renderer, 4, 4);
    SDL_RenderDebugText(app->renderer, 5, 10, currentDemo->GetName());
    SDL_SetRenderScale(app->renderer, 1.5, 1.5);
    SDL_RenderDebugText(app->renderer, 13, 60, "Press 1, 2, 3, 4, 5... to change demo");
    SDL_SetRenderScale(app->renderer, 1, 1);

    // Render current demo
    currentDemo->Render(app);

    SDL_RenderPresent(app->renderer);

    return app->app_quit;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    auto* app = (AppContext*)appstate;
    if (app) {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);

        delete app;
    }

    SDL_Log("Application quit successfully!");
    SDL_Quit();
}