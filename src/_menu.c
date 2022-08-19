#include <SDL2/SDL.h>

// You must include the command line parameters for your main function to be
// recognized by SDL
int main(int argc, char** args) {
    // Pointers to our window and surface
    SDL_Surface* winSurface = NULL;
    SDL_Window* window = NULL;

    // Initialize SDL. SDL_Init will return -1 if it fails.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	printf("error initializing SDL: %s\n", SDL_GetError());
	// End the program
	return 1;
    }

    // Create our window
    window =
	SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED,
			 SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

    // Make sure creating the window succeeded
    if (!window) {
	printf("error initializing SDL: %s\n", SDL_GetError());
	// End the program
	return 1;
    }

    // Get the surface from the window
    winSurface = SDL_GetWindowSurface(window);

    // Make sure getting the surface succeeded
    if (!winSurface) {
	printf("error initializing SDL: %s\n", SDL_GetError());
	// End the program
	return 1;
    }

    // Fill the window with a white rectangle
    SDL_FillRect(winSurface, NULL,
		 SDL_MapRGB(winSurface->format, 255, 255, 255));

    // Update the window display
    SDL_UpdateWindowSurface(window);

    // Wait
    SDL_Delay(5000);

    // Destroy the window. This will also destroy the surface
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    // End the program
    return 0;
}
