#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>

/* Function Prototypes */
uint8_t init_sdl( SDL_Window *w, SDL_Renderer *r, uint8_t s );
void    exit_sdl( SDL_Window *w, SDL_Renderer *r );

int main( int argc, char **argv ) {
    
    /* Chip 8 Setup */

    /* Load ROM */

    /* Set up SDL */
    SDL_Window   *window      = NULL;
    SDL_Renderer *renderer    = NULL;
    SDL_Event     event;
    uint8_t       pixel_scale = 8;

    // Try to initialize SDL
    if( init_sdl( window, renderer, pixel_scale ) != 0 ) {
        // Free memory and exit subsystems
        exit_sdl( window, renderer );
        return EXIT_FAILURE;
    }

    /* System Loop */
    uint8_t quit = 0;
    while( !quit ) {
        // Poll SDL System Events
        while( SDL_PollEvent( &event ) ) {
            // If the user closes the window
            if( event.type == SDL_QUIT ) {
                quit = 1;
            }else if( event.type == SDL_KEYDOWN ) {

                // Parse key presses
                switch( event.key.keysym.sym ) {

                    // F1 will exit program
                    case SDLK_F1:
                        quit = 1;
                        break;
                    
                    // Un-handled key press
                    default:
                        break;
                }

            }else if( event.type == SDL_KEYUP ) {

                // Parse key presses
                switch( event.key.keysym.sym ) {
                    
                    // Un-handled key press
                    default:
                        break;
                }

            }
        }
    }

    exit_sdl( window, renderer );
    return EXIT_SUCCESS;
}

// Attempt to initialize SDL subsystems, window, and renderer
// Return 0 on success
// Return 1 on fail
uint8_t init_sdl( SDL_Window *w, SDL_Renderer *r, uint8_t s ) {

    // Attempt to initialize SDL subsystems
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        SDL_Log( "Could Not Initialize SDL Subsystems: %s\r\n", SDL_GetError() );
        return 1;
    }

    // Try to create a window
    w = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        ( 64 * s ),
        ( 32 * s ),
        SDL_WINDOW_SHOWN
    );
    if( w == NULL ) {
        SDL_Log( "Could Not Create SDL Window: %s\r\n", SDL_GetError() );
        return 1;
    }

    // Try to create a renderer
    r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if( r == NULL ) {
        SDL_Log( "Could Not Create SDL Renderer: %s\r\n", SDL_GetError() );
        return 1;
    }

    return 0;
}

// Free memory used by SDL and exit subsystems
void exit_sdl( SDL_Window *w, SDL_Renderer *r ) {
    // Free renderer memory
    SDL_DestroyRenderer( r );
    // Free window memory
    SDL_DestroyWindow( w );
    // Exit SDL Subsystems
    SDL_Quit();
}