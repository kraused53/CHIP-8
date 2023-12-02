#include "chip8.h"

// Initialize an instance of chip8_t
void init_chip8( chip8_t *c ) {
    int index;

    // Zero out registers and key states
    for( index = 0; index < 16; index++ ) {
        c->V[ index ]    = 0x00;
        c->KEYS[ index ] = 0x00;
    }

    // Zero out program memory
    for( index = 0; index < MEM_SIZE; index++ ) {
        c->MEMORY[ index ] = 0x00;
    }

    // Zero out stack

    // Zero out graphics memory
    for( index = 0; index < GFX_SIZE; index++ ) {
        c->GFX[ index ] = 0x00;
    }
    c->GFX[ 0 ] = 0xFF;
    c->GFX[ 63 ] = 0xFF;
    c->GFX[ 64*31 ] = 0xFF;
    c->GFX[ 64*31 + 63 ] = 0xFF;

    // Trigger a screen update
    c->SU = 0x01;

}

// Free any dynamically allocated memory
void exit_chip8( chip8_t *c ) {
    // No dynamic memory as of now
}

// Attempt to load a rom into memory
// Return 0 on success
// Return 1 on fail
uint8_t load_rom( chip8_t *c, FILE *rom ) {


    return 0;
}