#pragma once

#include "BoardLogic.h"
#include "raylib.h"

#define OUTSIDE_MARGIN 10
#define OUTLINE_WIDTH 3

#define BOARD_START_X OUTSIDE_MARGIN
#define BOARD_START_Y (OUTSIDE_MARGIN * 2 + SCORE_SIZE_Y)
#define TILE_SIZE 100
#define TILE_MARGIN 10
#define TILE_START_X OUTSIDE_MARGIN + TILE_MARGIN
#define TILE_START_Y (BOARD_START_Y + TILE_MARGIN)
#define BOARD_SIZE_X (TILE_SIZE * NUM_TILES) + (TILE_MARGIN * (NUM_TILES + 1))
#define BOARD_SIZE_Y BOARD_SIZE_X

#define SCORE_START_X OUTSIDE_MARGIN
#define SCORE_START_Y OUTSIDE_MARGIN
#define SCORE_SIZE_Y TILE_SIZE

const int SCREEN_WIDTH = (OUTSIDE_MARGIN + BOARD_SIZE_X + OUTSIDE_MARGIN);
const int SCREEN_HEIGHT = (OUTSIDE_MARGIN + SCORE_SIZE_Y + OUTSIDE_MARGIN + BOARD_SIZE_Y + OUTSIDE_MARGIN);

#define SCORE_CIRCLE_DIAMETER 10
#define SCORE_LIGHT_DIAMETER 5
#define SCORE_DIGITS 2
#define SCORE_FONT_SIZE 80
#define SCORE_X_SCORE_X OUTSIDE_MARGIN * 2 + TILE_MARGIN * 2
#define SCORE_X_SCORE_Y OUTSIDE_MARGIN + TILE_MARGIN * 2
#define SCORE_O_SCORE_X (SCREEN_WIDTH - 40) - (OUTSIDE_MARGIN * 3) - TILE_MARGIN
#define SCORE_O_SCORE_Y OUTSIDE_MARGIN + TILE_MARGIN * 2

#define GAME_OVER_FONT_SIZE 40
#define GAME_OVER_SIZE_X 250
#define GAME_OVER_SIZE_Y 100
#define GAME_OVER_START_X (SCREEN_WIDTH / 2) - (GAME_OVER_SIZE_X / 2)
#define GAME_OVER_START_Y (SCREEN_HEIGHT / 2) - (GAME_OVER_SIZE_Y / 2)

#define BACKGROUND   (Color) { 192, 192, 192, 255 }
#define DARK_OUTLINE (Color) { 128, 128, 128, 255 }

struct Window {
    struct Game game;
    struct Rectangle layout[NUM_TILES][NUM_TILES];
    Sound click_sound;
    bool sounds;
    Vector2 (*x_strategy)(struct Window* w);
    Vector2 (*o_strategy)(struct Window* w);
};

Vector2 RandomStrategy(struct Window* w);
Vector2 ManualStrategy(struct Window* w);

Vector2 CheckCollision(Vector2 mouse_pos, struct Window window);
bool IsGameOver(enum GameState state);

void DrawOutline(Vector2 start, Vector2 size, bool invert);
void DrawScore(enum GameState turn);
void DrawBoard(struct Window window);
void DrawGameOver(enum GameState ending);
void DrawEverything(struct Window* w);

void InitGame(struct Window* w);
void HandleUpdate(struct Window* w);
void CloseGame(struct Window* w);