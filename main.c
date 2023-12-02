#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>

/* Local Files */
#include "chip8.h"
#include "chip8_diss.h"

/* SDL Variables */
SDL_Window   *window      = NULL;
SDL_Renderer *renderer    = NULL;
SDL_Event     event;
uint8_t       pixel_scale = 8;

/* Function Prototypes */
uint8_t init_sdl( uint8_t s );
void    exit_sdl( void );
void    update_graphics( chip8_t c, uint8_t s );

int main( int argc, char **argv ) {
    
    /* Chip 8 Setup */
    chip8_t cpu;
    init_chip8( &cpu );

    /* Load ROM */

    /* Set up SDL */
    // Try to initialize SDL
    if( init_sdl( pixel_scale ) != 0 ) {
        // Free memory and exit subsystems
        exit_sdl();
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
        // If cpu flagged graphics update
        if( cpu.SU ) {
            // Clear flag
            cpu.SU = 0x00;
            // Update graphics
            update_graphics( cpu, pixel_scale );
        }
    }

    exit_chip8( &cpu );
    exit_sdl();
    return EXIT_SUCCESS;
}

// Attempt to initialize SDL subsystems, window, and renderer
// Return 0 on success
// Return 1 on fail
uint8_t init_sdl( uint8_t s ) {

    // Attempt to initialize SDL subsystems
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        SDL_Log( "Could Not Initialize SDL Subsystems: %s\r\n", SDL_GetError() );
        return 1;
    }

    // Try to create a window
    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        ( CHIP8_W * s ),
        ( CHIP8_H * s ),
        SDL_WINDOW_SHOWN
    );
    if( window == NULL ) {
        SDL_Log( "Could Not Create SDL Window: %s\r\n", SDL_GetError() );
        return 1;
    }

    // Try to create a renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
    );
    if( renderer == NULL ) {
        SDL_Log( "Could Not Create SDL Renderer: %s\r\n", SDL_GetError() );
        return 1;
    }

    return 0;
}

// Free memory used by SDL and exit subsystems
void exit_sdl( void ) {
    // Free renderer memory
    SDL_DestroyRenderer( renderer );
    // Free window memory
    SDL_DestroyWindow( window );
    // Exit SDL Subsystems
    SDL_Quit();
}

// Use the chip 8 instance to update the screen
void update_graphics( chip8_t c, uint8_t s ) {
    int row;
    int col;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = s;
    rect.w = s;

    // For each row
    for( row = 0; row < CHIP8_H; row++ ) {
        rect.y = row * s;
        // For each collumn
        for( col = 0; col < CHIP8_W; col++ ) {
            rect.x = col * s;
            
            if( c.GFX[ ( CHIP8_W * row ) + col ] ) {
                // White
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }else {
                // Black
                SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
            }

            // Draw Pixel
            SDL_RenderFillRect( renderer, &rect );
        }
    }
    SDL_RenderPresent( renderer );
}