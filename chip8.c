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
    for( index = 0; index < STACK_DEPTH; index++ ) {
        c->GFX[ index ] = 0x00;
    }

    // Zero out graphics memory
    for( index = 0; index < GFX_SIZE; index++ ) {
        c->GFX[ index ] = 0x00;
    }


    /* 16-Bit Variables */
    // Index pointer
    c->IP = 0x0000;
    // Program counter
    c->PC = ROM_START;
    // Current opcode
    c->OP = 0x0000;

    /* 8-Bit Variables */
    // Stack pointer
    c->SP = 0x00;
    // Sount timer
    c->ST = 0x00;
    //Delay timer
    c->DT = 0x00;

    // Trigger a screen update
    c->SU = 0x01;

    // Clear halted flag
    c->halted = 0x00;

}

// Free any dynamically allocated memory
void exit_chip8( chip8_t *c ) {
    // No dynamic memory as of now
}

// Attempt to load a rom into memory
// Return 0 on success
// Return 1 on fail
uint8_t load_rom( chip8_t *c, FILE *rom ) {

    uint16_t  rom_len;
    uint8_t  *rom_buffer;

    // Get the length of the rom file
    fseek( rom, 0, SEEK_END );
    rom_len = ftell( rom );
    rewind( rom );

    // Make sure that the rom will fit into program memory
    // program memory is cpu->MEMORY[ from ROM_START to MEM_SIZE ]
    if( rom_len >= ( MEM_SIZE - ROM_START ) ) {
        printf( "Rom is too large for chip 8 memory.\r\n" );
		printf( "\tRom is     %4u bytes\r\n", rom_len );
		printf( "\tChip 8 has %4u bytes\r\n", ( MEM_SIZE - ROM_START ) );
		return 1;
    }

    // Allocate rom_buffer
    rom_buffer = ( uint8_t * )calloc( rom_len, sizeof( uint8_t ) );
    if( rom_buffer == NULL ) {
        printf( "Could not allocate %u bytes for rom buffer\r\n", rom_len );
        return 1;
    }

    // Read rom into rom_buffer
    fread( rom_buffer, sizeof( uint8_t ), rom_len, rom );

    // Load rom into cpu->MEM starting at ROM_START
    int index;
    for( index = 0; index < rom_len; index++ ) {
        c->MEMORY[ ROM_START + index ] = rom_buffer[ index ]; 
    }

    // Free rom_buffer
    free( rom_buffer );

    return 0;
}

// Execute one chip8 opcode
void step_chip8( chip8_t *c ) {

        // If the system is halted, do nothing
        if( c->halted ) {
            return;
        }

        c->OP = c->MEMORY[ c->PC + 1 ] | ( c->MEMORY[ c->PC + 0 ] << 8 );

        // Dissasemble opcode
        diss_step_chip8( c );

        switch( c->OP & 0xF000 ) {
            // System Call
            case 0x0000:
                // 0x00E0 - Clear the screen
                if( ( c->OP & 0x0FFF ) == 0x00E0 ) {
                    // Clear screen
                    clear_gfx( c );
                    // Trigger screen refresh
                    c->SU = 0x01;
                    // Advance PC to next opcode
                    c->PC += 2;
                }else {
                    // Advance PC to next opcode
                    c->PC += 2;
                }
                break;
            
            // Jump
            case 0x1000:
                // Check for infinite loop
                // If new address is the same as the old address, halt cpu
                if( c->PC == ( c->OP & 0x0FFF ) ) {
                    c->halted = 0x01;
                }else {
                    // Set the PC to the the address NNN
                    c->PC = c->OP & 0x0FFF;
                }
                break;

            // Set Register to data
            case 0x6000:
                c->V[ ( c->OP & 0x0F00 ) >> 8 ] = c->OP & 0x00FF;
                // Advance PC to next opcode
                c->PC += 2;
                break;

            // Add data to register ( do not change flag register )
            case 0x7000:
                c->V[ ( c->OP & 0x0F00 ) >> 8 ] = c->V[ ( c->OP & 0x0F00 ) >> 8 ] + ( c->OP & 0x00FF );
                // Advance PC to next opcode
                c->PC += 2;
                break;

            // Set index register
            case 0xA000:
                c->IP = c->OP & 0x0FFF;
                // Advance PC to next opcode
                c->PC += 2;
                break;

            // Draw
            case 0xD000:
                draw_object( c, c->V[ ( c->OP & 0x0F00 ) >> 8 ], c->V[ ( c->OP & 0x00F0 ) >> 4 ], ( c->OP & 0x000F ) >> 0 );
                // Trigger screen refresh
                c->SU = 0x01;
                // Advance PC to next opcode
                c->PC += 2;
                break;

            default:
                // Advance PC to next opcode
                c->PC += 2;
                break;
        }
    

}

void clear_gfx( chip8_t *c ) {
    int index;
    for( index = 0; index < GFX_SIZE; index++ ) {
        c->GFX[ index ] = 0x00;
    }
}

void draw_object( chip8_t *c, uint8_t x, uint8_t y, uint8_t n ) {
    x = x % CHIP8_W;
    y = y % CHIP8_H;


    c->V[ 0xF ] = 0x00;

    for( int yline = 0; yline < n; yline++ ) {
        int data = c->MEMORY[ c->IP + yline ];
        for( int xline = 0; xline < 8; xline++ ) {
            if( ( data & ( 0x80 >> xline ) ) != 0 ) {
                if( c->GFX[ x + xline + ( ( y + yline ) * CHIP8_W ) ] != 0 ) {
                    c->V[ 15 ] = 1;
                }
                c->GFX[ x + xline + ( ( y + yline ) * CHIP8_W ) ] ^= 1;
            }
        }
    }
}