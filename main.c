#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

void drawCircle(SDL_Renderer *renderer , int centerX, int centerY, int radius)
{
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x*x + y*y <= radius*radius)
			{
				SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
			}
		}
	}
}

int main()
{
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Create Window
    SDL_Window *window = SDL_CreateWindow(
        "Blank Canvas",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    if (!window)
    {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. Create Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer)
    {
        printf("Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    // 4. Main Loop
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // 5. Clear Screen (Black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int mouseX , mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_SetRenderDrawColor(renderer, 20, 240, 200, 255);

        drawCircle(renderer, mouseX, mouseY, 25);

        // 6. Present Frame
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
