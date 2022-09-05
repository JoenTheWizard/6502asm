#include "defines.h"

SDL_Window* window = NULL;

void OpenGUI(uint8_t* memory) {
    window = SDL_CreateWindow
    (
        "6502 ASM", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        480,
        SDL_WINDOW_SHOWN
    );

    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear winow
    SDL_RenderClear(renderer);

    //printf("%x===\n",colorPalette[1]);
    //Create a rect (pixel)
    //Draw streams of pixels
    int x = 0;
    int y = 0;
    for (int i = 0x0200; i <= 0x05FF; i++) {
        if (memory[i] > 0) {
            SDL_Rect rect = {
                .x = x,
                .y = y,
                .w = 15,
                .h = 15
            };
            //printf("LOL: %x\n", memory[i] & 0x0F);
            switch (memory[i] & 0x0F) {
                case 0x00:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
                case 0x01:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
                case 0x02:
                    SDL_SetRenderDrawColor(renderer, 136, 0, 0, 255);
                    break;
                case 0x03:
                    SDL_SetRenderDrawColor(renderer, 170, 255, 238, 255);
                    break;
                case 0x04:
                    SDL_SetRenderDrawColor(renderer, 204, 68, 204, 255);
                    break;
                case 0x05:
                    SDL_SetRenderDrawColor(renderer, 0, 204, 85, 255);
                    break;
                case 0x06:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 170, 255);
                    break;
                case 0x07:
                    SDL_SetRenderDrawColor(renderer, 238, 238, 119, 255);
                    break;
                case 0x08:
                    SDL_SetRenderDrawColor(renderer, 221, 136, 85, 255);
                    break;
                case 0x09:
                    SDL_SetRenderDrawColor(renderer, 102, 68, 0, 255);
                    break;
                case 0x0a:
                    SDL_SetRenderDrawColor(renderer, 255, 119, 119, 255);
                    break;
                case 0x0b:
                    SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
                    break;
                case 0x0c:
                    SDL_SetRenderDrawColor(renderer, 119, 119, 119, 255);
                    break;
                case 0x0d:
                    SDL_SetRenderDrawColor(renderer, 170, 255, 102, 255);
                    break;
                case 0x0e:
                    SDL_SetRenderDrawColor(renderer, 0, 136, 255, 255);
                    break;
                case 0x0f:
                    SDL_SetRenderDrawColor(renderer, 187, 187, 187, 255);
                    break;
            }
            //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

            SDL_RenderFillRect(renderer, &rect);
        }
        x+=15;
        if (x % 0x020 == 0) {
            x = 0;
            y+=15;
        }
    }

    // for (int i = 0; i < 680; i+=15) {
    //     SDL_Rect rect = {
    //         .x = i,
    //         .y = 0,
    //         .w = 15,
    //         .h = 15
    //     };
    //     SDL_SetRenderDrawColor(renderer, i*5, 0, 255, 255);

    //     SDL_RenderFillRect(renderer, &rect);
    // }

    //While program is running
    SDL_Event event;
    while(event.type != SDL_QUIT) {
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                SDL_DestroyWindow(window);
                SDL_Quit();
            }
             // Render the rect to the screen
             SDL_RenderPresent(renderer);
        }
    }
}
