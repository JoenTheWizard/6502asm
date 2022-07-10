SDL_Window* window = NULL;

void* OpenGUI(void *vargp) {
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

    // Set render color to black (background will be rendered in this color)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear winow
    SDL_RenderClear(renderer);

    //Create a rect (pixel)
    //Draw streams of pixels
    for (int i = 0; i < 680; i+=15) {
        SDL_Rect rect = {
            .x = i,
            .y = 0,
            .w = 15,
            .h = 15
        };
        SDL_SetRenderDrawColor(renderer, i*5, 0, 255, 255);

        SDL_RenderFillRect(renderer, &rect);
    }

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
