/*
 * interaction.h - Header file for interaction with user
 */

#pragma once

#include <unistd.h>

// If prompt is open, print it to the console
#define PROMPT(is_open, prompt) \
    if (is_open) { \
        std::cout << prompt; \
    }

