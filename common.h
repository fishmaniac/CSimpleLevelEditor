#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define PROJECT_NAME "CSimpleLevelEditor"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRAME_TIME 16

#define GRID_HEIGHT 32
#define GRID_WIDTH 32

#define MAP_WIDTH 300
#define MAP_HEIGHT 30
#define MAP_WIDTH_MAX 3000
#define MAP_HEIGHT_MAX 100
#define TILE_SIZE 32

#define FONT_SCALAR 10
#define FONT_SIZE 16
#define MAX_TEXT 100

#define BUTTON_AMT 6
#define BUTTON_AMT_APP 3

#define RECT_W_MAX 900
#define RECT_H_MAX 300
#define RECT_W_MIN 300
#define RECT_H_MIN 100

#define COLOR_WHITE (SDL_Color){255, 255, 255, 255}
#define COLOR_BLACK (SDL_Color){0, 0, 0, 255}
#define COLOR_BLUE (SDL_Color){101, 189, 255, 255}
#define COLOR_GREEN (SDL_Color){24, 175, 0, 255}
#define COLOR_RED (SDL_Color){255, 0, 0, 255}

