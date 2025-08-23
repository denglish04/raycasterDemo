#include <SDL.h>
#include <iostream>

int main() {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "My SDL2 Window",             // Window title
        SDL_WINDOWPOS_CENTERED,      // X position
        SDL_WINDOWPOS_CENTERED,      // Y position
        1024,                         // Width
        512,                         // Height
        SDL_WINDOW_SHOWN             // Flags
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Keep the window open until the user closes it
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16); // ~60 FPS
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

