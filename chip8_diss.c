#include "chip8_diss.h"

void diss_step_chip8( chip8_t *c ) {

    switch( c->OP & 0xF000 ) {
        // System Call
        case 0x0000:
            if( ( c->OP & 0x0FFF ) == 0x00E0 ) {
                printf( "%04X: 0x%04X - Clear the screen.\r\n", c->PC, c->OP );
            }else if( ( c->OP & 0x0FFF ) == 0x00EE ) {
                printf( "%04X: 0x%04X - Return from subroutine.\r\n", c->PC, c->OP );
            }else {
                printf( "%04X: 0x%04X - Unimplemented OpCode!\r\n", c->PC, c->OP );
            }
            break;
        
        // Jump
        case 0x1000:
            printf( "%04X: 0x%04X - Jump to address 0x%04X.\r\n", c->PC, c->OP, ( c->OP & 0x0FFF ) );
            break;

        // Go to Subroutine 
        case 0x2000:
            printf( "%04X: 0x%04X - Go to subroutine at 0x%04X.\r\n", c->PC, c->OP, ( c->OP & 0x0FFF ) );
            break;

        // Set Register to data
        case 0x6000:
            printf( "%04X: 0x%04X - Set V[ %01X ] to the value %02X\r\n", c->PC, c->OP, ( ( c->OP & 0x0F00 ) >> 8 ), ( c->OP & 0x00FF ) );
            break;

        // Add data to register
        case 0x7000:
            printf( "%04X: 0x%04X - Add the value %02X to the contents of V[ %01X ] \r\n", c->PC, c->OP, ( c->OP & 0x00FF ), ( ( c->OP & 0x0F00 ) >> 8 ) );
            break;

        // Set index register
        case 0xA000:
            printf( "%04X: 0x%04X - Set the index pointer to the address 0x%04X \r\n", c->PC, c->OP, ( c->OP & 0x0FFF ) );
            break;

        // Draw
        case 0xD000:
            printf( "%04X: 0x%04X - Draw an object that is %d bytes wide. Address:\r\n", c->PC, c->OP, ( ( c->OP & 0x000F ) >> 0 ) );
            printf( "\tx = V[ %01X ] = ( %2d )\r\n", ( ( c->OP & 0x0F00 ) >> 8 ), c->V[ ( ( c->OP & 0x0F00 ) >> 8 ) ] );
            printf( "\ty = V[ %01X ] = ( %2d )\r\n", ( ( c->OP & 0x00F0 ) >> 4 ), c->V[ ( ( c->OP & 0x00F0 ) >> 4 ) ] );
            break;

        default:
            printf( "%04X: 0x%04X - Unimplemented OpCode!\r\n", c->PC, c->OP );
            break;
    }

}

void diss_chip8( chip8_t *c, uint8_t len ) {

    int cnt;

    for( cnt = 0; cnt < len; cnt++ ) {

        c->OP = c->MEMORY[ c->PC + 1 ] | ( c->MEMORY[ c->PC + 0 ] << 8 );
    
        diss_step_chip8( c );

        c->PC += 2;

    }

}