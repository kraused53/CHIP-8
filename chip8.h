#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/* Memory Addresses */
#define MEM_SIZE    0x1000
#define ROM_START   0x0200
#define FONT_START  0x0050
#define CHIP8_W     64
#define CHIP8_H     32
#define GFX_SIZE    ( CHIP8_H * CHIP8_W )
#define STACK_DEPTH 12

/* Chip 8 Data Structure */
typedef struct {
    /* System "Hardware" */
    // 16x 8-Bit registers
    uint8_t V[ 16 ];
    // Program memory ( Default 4K-Byte )
    uint8_t MEMORY[ MEM_SIZE ];
    // User Key States
    uint8_t KEYS[ 16 ];
    // Stack: 16-Bits wide, 12 deep
    uint8_t STACK[ STACK_DEPTH ];
    // Video memory ( As of now, 1 byte per screen pixel )
    uint8_t GFX[ GFX_SIZE ];

    /* 16-Bit Variables */
    // Index pointer
    uint16_t IP;
    // Program counter
    uint16_t PC;
    // Current opcode
    uint16_t OP;

    /* 8-Bit Variables */
    // Stack pointer
    uint8_t SP;
    // Sount timer
    uint8_t ST;
    //Delay timer
    uint8_t DT;

    /* Flags */
    // Use to trigger a graphics window update
    uint8_t SU;
    // Used to halt chip 8 programs
    // Will be set if an infinite loop is detected
    // Will no halt SDL framework
    uint8_t halted;

}chip8_t;

#include "chip8_diss.h"

/* Function prototypes */
void    init_chip8( chip8_t *c );
void    exit_chip8( chip8_t *c );
uint8_t load_rom( chip8_t *c, FILE *rom );
void    step_chip8( chip8_t *c );
void    clear_gfx( chip8_t *c );
void draw_object( chip8_t *c, uint8_t x, uint8_t y, uint8_t n );