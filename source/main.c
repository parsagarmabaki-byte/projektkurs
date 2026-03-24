#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SPEED        200
#define PLAYER_W     64
#define PLAYER_H     128
#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 1000
#define BACKGROUND_W  3200   // större än fönstret
#define BACKGROUND_H  2000

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

    SDL_Surface *pPlayer = IMG_Load("resources/green-hood.png"); // LOAD PLAYER
     //FELHANTERING
    if (!pPlayer) {
        printf("IMG_Load Error (player): %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Surface *pBackground = IMG_Load("resources/background-test.png"); // LOAD BACKGROUND
     //FELHANTERING
    if (!pBackground) {
        printf("IMG_Load Error (background): %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *pPlayerTexture = SDL_CreateTextureFromSurface(pRenderer, pPlayer);
    SDL_FreeSurface(pPlayer); //tar bort den från minnet
    //FELHANTERING
    if (!pPlayerTexture) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *pBackgroundTexture = SDL_CreateTextureFromSurface(pRenderer, pBackground);
    SDL_FreeSurface(pBackground); //tar bort den från minnet
    //FELHANTERING
    if (!pBackgroundTexture) {
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
        if (playerX > BACKGROUND_W  - PLAYER_W) playerX = BACKGROUND_W  - PLAYER_W;
        if (playerY > BACKGROUND_H - PLAYER_H) playerY = BACKGROUND_H - PLAYER_H;

        playerRect.x = (int)playerX;
        playerRect.y = (int)playerY;

       /* --- DRAW --- */
        int cameraX = (int)playerX - (WINDOW_WIDTH  / 2) + (PLAYER_W / 2);
        int cameraY = (int)playerY - (WINDOW_HEIGHT / 2) + (PLAYER_H / 2);

        SDL_Rect bgRect = { -cameraX, -cameraY, BACKGROUND_W, BACKGROUND_H };
        SDL_Rect screenPlayerRect = 
        {
            (WINDOW_WIDTH  - PLAYER_W) / 2,
            (WINDOW_HEIGHT - PLAYER_H) / 2,
            PLAYER_W,
            PLAYER_H
        };

SDL_RenderClear(pRenderer);
SDL_RenderCopy(pRenderer, pBackgroundTexture, NULL, &bgRect);
SDL_RenderCopy(pRenderer, pPlayerTexture,     NULL, &screenPlayerRect);
SDL_RenderPresent(pRenderer);
    }

    /* --- CLEANUP --- */
    SDL_DestroyTexture(pPlayerTexture);
    SDL_DestroyTexture(pBackgroundTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}