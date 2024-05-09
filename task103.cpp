#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define CIRCLE_RADIUS 20
#define CIRCLE_SPEED 10

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Circle Collision", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

bool checkCollision(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distance = sqrt(dx * dx + dy * dy);
    return distance < (r1 + r2);
    
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initializeSDL(&window, &renderer)) {
        return 1;
    }

    int movingCircleX = 0;
    int movingCircleY = SCREEN_HEIGHT / 2;
    int secondCircleX = SCREEN_WIDTH / 2;
    int secondCircleY = 0;

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        secondCircleY -= CIRCLE_SPEED;
                        break;
                    case SDLK_DOWN:
                        secondCircleY += CIRCLE_SPEED;
                        break;
                    case SDLK_LEFT:
                        secondCircleX -= CIRCLE_SPEED;
                        break;
                    case SDLK_RIGHT:
                        secondCircleX += CIRCLE_SPEED;
                        break;
                    default:
                        break;
                }
            }
        }

        // Move the moving circle from left to right continuously
        movingCircleX += CIRCLE_SPEED;
        if (movingCircleX >= SCREEN_WIDTH)
            movingCircleX = 0 - CIRCLE_RADIUS * 2; // Reset position if it goes off-screen

        // Check collision between circles
        bool collision = checkCollision(movingCircleX + CIRCLE_RADIUS, movingCircleY + CIRCLE_RADIUS, CIRCLE_RADIUS, secondCircleX, secondCircleY, CIRCLE_RADIUS);

        SDL_SetRenderDrawColor(renderer, 100, 200, 55, 35);
        SDL_RenderClear(renderer);

        // Draw moving circle
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        drawCircle(renderer, movingCircleX, movingCircleY, CIRCLE_RADIUS);

        // Draw second circle
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        drawCircle(renderer, secondCircleX, secondCircleY, CIRCLE_RADIUS);

        // If collision, change color
        if (collision) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawCircle(renderer, movingCircleX, movingCircleY, CIRCLE_RADIUS);
            drawCircle(renderer, secondCircleX, secondCircleY, CIRCLE_RADIUS);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
