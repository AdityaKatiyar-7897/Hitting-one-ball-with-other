#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float radius;
} Ball;

void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
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
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Mouse Physics Experiment",
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

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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

    Ball npc;
    npc.x = 400;
    npc.y = 300;
    npc.vx = 0;
    npc.vy = 0;
    npc.radius = 45;

    int prevMouseX = 0;
    int prevMouseY = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float mouseVX = mouseX - prevMouseX;
        float mouseVY = mouseY - prevMouseY;

        // Update NPC position
        npc.x += npc.vx;
        npc.y += npc.vy;

        // Friction
        npc.vx *= 0.98f;
        npc.vy *= 0.98f;

        // Wall collisions
        if (npc.x - npc.radius < 0)
        {
            npc.x = npc.radius;
            npc.vx = -npc.vx;
        }

        if (npc.x + npc.radius > 800)
        {
            npc.x = 800 - npc.radius;
            npc.vx = -npc.vx;
        }

        if (npc.y - npc.radius < 0)
        {
            npc.y = npc.radius;
            npc.vy = -npc.vy;
        }

        if (npc.y + npc.radius > 600)
        {
            npc.y = 600 - npc.radius;
            npc.vy = -npc.vy;
        }

        // Collision with mouse ball
        float dx = npc.x - mouseX;
        float dy = npc.y - mouseY;

        float distanceSquared = dx*dx + dy*dy;
        float mouseRadius = 40;
        float radiusSum = npc.radius + mouseRadius;

        if (distanceSquared <= radiusSum * radiusSum)
        {
            float distance = sqrt(distanceSquared);

            if (distance != 0)
            {
                // Normalized direction
                float nx = dx / distance;
                float ny = dy / distance;

                // Fix overlap
                float overlap = radiusSum - distance;
                npc.x += nx * overlap;
                npc.y += ny * overlap;

                // Transfer velocity (basic version)
                npc.vx = mouseVX;
                npc.vy = mouseVY;
            }
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // NPC ball (lavender)
        SDL_SetRenderDrawColor(renderer, 180, 90, 255, 255);
        drawCircle(renderer, (int)npc.x, (int)npc.y, npc.radius);

        // Mouse ball (electric cyan)
        SDL_SetRenderDrawColor(renderer, 20, 240, 200, 255);
        drawCircle(renderer, mouseX, mouseY, 40);

        SDL_RenderPresent(renderer);

        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
