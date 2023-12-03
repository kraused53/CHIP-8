#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chip8.h"

/* Function Prototypes */
void diss_step_chip8( chip8_t *c );
void diss_chip8( chip8_t *c, uint8_t len );