#include "chip8_diss.h"

void chip8_diss( uint16_t PC, uint16_t OP ) {

    // OPCode Selector
    uint16_t A = OP >> 12;
    /*
    // Address
    uint16_t NNN = OP & 0x0FFF;
    // 8-Bit Constant
    uint16_t  NN = OP & 0x00FF;
    // 4-Bit Constant
    uint16_t   N = OP & 0x000F;
    // X Register
    uint16_t   X = ( OP & 0x0F00 ) >> 8;
    // Y Register
    uint16_t   Y = ( OP & 0x00F0 ) >> 4;
*/
    switch( A ) {
        // System Call
        
        
        default:
            printf( "%04X: 0x%04X - Unimplemented OpCode!\r\n", PC, OP );
            break;
    }
}