#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SPEED        200
#define PLAYER_W     64
#define PLAYER_H     128
#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 1000

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *pWindow = SDL_CreateWindow(
        "Shrouded",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );
    if (!pWindow) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *pRenderer = SDL_CreateRenderer(
        pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!pRenderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *pSurface = IMG_Load("resources/green-hood.png");
    if (!pSurface) {
        printf("IMG_Load Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pTexture) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Rect playerRect = {
        (WINDOW_WIDTH  - PLAYER_W) / 2,
        (WINDOW_HEIGHT - PLAYER_H) / 2,
        PLAYER_W,
        PLAYER_H
    };
    float playerX = playerRect.x;
    float playerY = playerRect.y;

    bool up = false, down = false, left = false, right = false;
    bool running = true;

    while (running) {
        /* --- INPUT --- */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                bool pressed = (event.type == SDL_KEYDOWN);
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W: case SDL_SCANCODE_UP:    up    = pressed; break;
                    case SDL_SCANCODE_S: case SDL_SCANCODE_DOWN:  down  = pressed; break;
                    case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT:  left  = pressed; break;
                    case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT: right = pressed; break;
                    default: break;
                }
            }
        }

        /* --- UPDATE --- */
        float velX = 0, velY = 0;
        if (up   && !down)  velY = -SPEED;
        if (down && !up)    velY =  SPEED;
        if (left && !right) velX = -SPEED;
        if (right && !left) velX =  SPEED;

        playerX += velX / 60.0f;
        playerY += velY / 60.0f;

        /* Håll spelaren inom fönstret */
        if (playerX < 0)                        playerX = 0;
        if (playerY < 0)                        playerY = 0;
        if (playerX > WINDOW_WIDTH  - PLAYER_W) playerX = WINDOW_WIDTH  - PLAYER_W;
        if (playerY > WINDOW_HEIGHT - PLAYER_H) playerY = WINDOW_HEIGHT - PLAYER_H;

        playerRect.x = (int)playerX;
        playerRect.y = (int)playerY;

        /* --- DRAW --- */
        SDL_RenderClear(pRenderer);
        SDL_RenderCopy(pRenderer, pTexture, NULL, &playerRect);
        SDL_RenderPresent(pRenderer);
    }

    /* --- CLEANUP --- */
    SDL_DestroyTexture(pTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}