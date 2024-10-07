// TicTacToe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>

#include "TicTacToe.h"


int score_x = 0;
int score_o = 0;

struct Game g = { 0 };

enum GameState NextTurn(enum GameState turn) {
    if (turn != X_TURN && turn != O_TURN) {
        return START;
    }

    return turn == X_TURN ? O_TURN : X_TURN;
}

Vector2 CheckCollision(Vector2 mouse_pos, struct Window window) {
    Vector2 collision = { -1, -1 };
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            if (CheckCollisionPointRec(mouse_pos, window.layout[i][j])) {
                return (Vector2) { j, i };
            }
        }
    }

    return collision;
}

Vector2 RandomStrategy(struct Window* w) {
    Vector2 move;
    do {
        move = (Vector2){ GetRandomValue(0, NUM_TILES - 1), GetRandomValue(0, NUM_TILES - 1) };
    } while (w->game.board[(int)move.y][(int)move.x] != START);
    return move;
}

Vector2 BlockStrategy(struct Window* w) {
    // Check for us winning
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            if (w->game.board[i][j] != START) {
                continue;
            }

            w->game.board[i][j] = w->game.state;
            enum GameState win = CheckWin(w->game.board);
            w->game.board[i][j] = START;
            if (win == w->game.state + TURN_WIN_OFFSET) {
                return (Vector2) { j, i };
            }
        }
    }

    // Check for blocking opponent win
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            if (w->game.board[i][j] != START) {
                continue;
            }

            enum GameState next = NextTurn(w->game.state);
            w->game.board[i][j] = next;
            enum GameState win = CheckWin(w->game.board);
            w->game.board[i][j] = START;
            if (win == next + TURN_WIN_OFFSET) {
                return (Vector2) { j, i };
            }
        }
    }

    // If no blocks or wins, just pick a random spot
    return RandomStrategy(w);
}

Vector2 ManualStrategy(struct Window* w) {
    Vector2 collision = { -1, -1 };
    if (IsGestureDetected(GESTURE_TAP)) {
        Vector2 mouse_pos = { 0 };
        mouse_pos = GetMousePosition();
        collision = CheckCollision(mouse_pos, *w);
    }

    return collision;
}

void InitGame(struct Window* w) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic-Tac-Toe");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    for (int i = 0; i < NUM_TILES; i++) {
        int y_pos = (TILE_MARGIN * i) + (TILE_SIZE * i) + TILE_START_Y;

        for (int j = 0; j < NUM_TILES; j++) {
            int x_pos = (TILE_MARGIN * j) + (TILE_SIZE * j) + TILE_START_X;
            w->layout[i][j] = (Rectangle){ x_pos, y_pos, TILE_SIZE, TILE_SIZE };
        }
    }
    w->game.state = X_TURN;
    w->sounds = true;

    InitAudioDevice();
    w->click_sound = LoadSound("audio/select_005.ogg");
    w->x_strategy = &ManualStrategy;
    w->o_strategy = &BlockStrategy;
}

void DrawOutline(Vector2 start, Vector2 size, bool invert) {
    Color color1 = WHITE;
    Color color2 = DARK_OUTLINE;
    if (invert) {
        color1 = DARK_OUTLINE;
        color2 = WHITE;
    }

    // Left
    DrawRectangle(start.x, start.y, OUTLINE_WIDTH, size.y - OUTLINE_WIDTH, color1);
    DrawTriangle((Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH }, (Vector2){ start.x, start.y + size.y - OUTLINE_WIDTH}, (Vector2){ start.x, start.y + size.y }, color1);

    // Top
    DrawRectangle(start.x, start.y, size.x - OUTLINE_WIDTH, OUTLINE_WIDTH, color1);
    DrawTriangle((Vector2){ start.x + size.x, start.y }, (Vector2) { start.x + size.x - OUTLINE_WIDTH, start.y }, (Vector2){ start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH }, color1);

    // Bottom
    DrawRectangle(start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH, size.x - OUTLINE_WIDTH, OUTLINE_WIDTH, color2);
    DrawTriangle((Vector2){ start.x, start.y + size.y }, (Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y },  (Vector2){ start.x + OUTLINE_WIDTH, start.y + size.y - OUTLINE_WIDTH}, color2);

    // Right
    DrawRectangle(start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH, OUTLINE_WIDTH, size.y - OUTLINE_WIDTH, color2);
    DrawTriangle((Vector2){ start.x + size.x - OUTLINE_WIDTH, start.y + OUTLINE_WIDTH }, (Vector2){ start.x + size.x, start.y + OUTLINE_WIDTH}, (Vector2){ start.x + size.x, start.y }, color2);
}

void DrawScore(enum GameState turn) {
    DrawOutline((Vector2) { OUTSIDE_MARGIN, OUTSIDE_MARGIN }, (Vector2) { SCREEN_WIDTH - (OUTSIDE_MARGIN * 2), SCORE_SIZE_Y }, true);

    int circle_margin = OUTSIDE_MARGIN + TILE_MARGIN * 2;
    DrawCircle(circle_margin, circle_margin, SCORE_CIRCLE_DIAMETER, DARK_OUTLINE);
    DrawCircle(SCREEN_WIDTH - circle_margin, circle_margin, SCORE_CIRCLE_DIAMETER, DARK_OUTLINE);
    if (turn == X_TURN) {
        DrawCircle(circle_margin, circle_margin, SCORE_LIGHT_DIAMETER, GREEN);
    }
    else if (turn == O_TURN) {
        DrawCircle(SCREEN_WIDTH - circle_margin, circle_margin, SCORE_LIGHT_DIAMETER, GREEN);
    }

    char* separator = "-";
    int separator_text_size = MeasureText(separator, SCORE_FONT_SIZE);
    int separator_start = (SCREEN_WIDTH / 2) - (separator_text_size / 2);
    DrawText(separator, separator_start, SCORE_O_SCORE_Y, SCORE_FONT_SIZE, BLACK);

    char score_x_str[SCORE_DIGITS + 1];
    snprintf(score_x_str, SCORE_DIGITS + 1, "%d", score_x);
    int x_text_size = MeasureText(score_x_str, SCORE_FONT_SIZE);
    int x_text_start = separator_start - 20 - x_text_size;
    DrawText(score_x_str, x_text_start, SCORE_X_SCORE_Y, SCORE_FONT_SIZE, RED);

    char score_o_str[SCORE_DIGITS + 1];
    snprintf(score_o_str, SCORE_DIGITS + 1, "%d", score_o);
    int o_text_size = MeasureText(score_o_str, SCORE_FONT_SIZE);
    int o_text_start = separator_start + separator_text_size + 20;
    DrawText(score_o_str, o_text_start, SCORE_O_SCORE_Y, SCORE_FONT_SIZE, BLUE);

}

void DrawBoard(struct Window window) {
    int board_size = TILE_MARGIN * (NUM_TILES + 1) + (TILE_SIZE * NUM_TILES);
    DrawOutline((Vector2) { OUTSIDE_MARGIN, BOARD_START_Y}, (Vector2) { SCREEN_WIDTH - (OUTSIDE_MARGIN * 2), board_size }, true);
    for (int i = 0; i < NUM_TILES; i++) {
        for (int j = 0; j < NUM_TILES; j++) {
            int x_pos = window.layout[i][j].x;
            int y_pos = window.layout[i][j].y;
            Color color;
            switch (window.game.board[i][j]) {
            case X_TURN:
                DrawLineEx((Vector2) { x_pos + 15, y_pos + 15 }, (Vector2) { x_pos + TILE_SIZE - 15, y_pos + TILE_SIZE - 15 }, 10, RED);
                DrawLineEx((Vector2) { x_pos + 15, y_pos + TILE_SIZE - 15 }, (Vector2) { x_pos + TILE_SIZE - 15, y_pos + 15}, 10, RED);
                break;
            case O_TURN:
                DrawCircle(x_pos + (TILE_SIZE / 2), y_pos + (TILE_SIZE / 2), (TILE_SIZE /2 ) - 10, BLUE);
                DrawCircle(x_pos + (TILE_SIZE / 2), y_pos + (TILE_SIZE / 2), (TILE_SIZE / 2) - 20, BACKGROUND);
                break;
            }
            DrawOutline((Vector2) { x_pos, y_pos }, (Vector2) { TILE_SIZE, TILE_SIZE }, false);
        }
    }
}

void DrawGameOver(enum GameState ending) {
    DrawRectangle(GAME_OVER_START_X, GAME_OVER_START_Y, GAME_OVER_SIZE_X, GAME_OVER_SIZE_Y, BACKGROUND);
    DrawOutline((Vector2) { GAME_OVER_START_X, GAME_OVER_START_Y }, (Vector2) { GAME_OVER_SIZE_X, GAME_OVER_SIZE_Y }, false);
    const char* ending_text;
    if (ending == X_WIN) {
        ending_text = "X wins!";
    }
    else if (ending == O_WIN) {
        ending_text = "O wins!";
    }
    else {
        ending_text = "Stalemate!";

    }
    int text_size = MeasureText(ending_text, GAME_OVER_FONT_SIZE);
    int text_offset_x = (GAME_OVER_SIZE_X / 2) - (text_size / 2);
    int text_offset_y = (GAME_OVER_SIZE_Y / 2) - (GAME_OVER_FONT_SIZE / 2);
    DrawText(ending_text, GAME_OVER_START_X + text_offset_x, GAME_OVER_START_Y + text_offset_y, GAME_OVER_FONT_SIZE, BLACK);
}

bool IsGameOver(enum GameState state) {
    if (state == X_WIN || state == O_WIN || state == STALEMATE) {
        return true;
    }
    else {
        return false;
    }
}

void HandleUpdate(struct Window* w) {
    if (w->game.state == X_TURN) {
        Vector2 move = w->x_strategy(w);
        if (move.x != -1 && move.y != -1 && w->game.board[(int)move.y][(int)move.x] == START) {
            w->game.board[(int)move.y][(int)move.x] = w->game.state;
            w->game.state = NextTurn(w->game.state);

            if (w->sounds) {
                PlaySound(w->click_sound);
            }
        }
    } else if (w->game.state == O_TURN) {
        Vector2 move = w->o_strategy(w);
        if (move.x != -1 && move.y != -1 && w->game.board[(int)move.y][(int)move.x] == START) {
            w->game.board[(int)move.y][(int)move.x] = w->game.state;
            w->game.state = NextTurn(w->game.state);

            if (w->sounds) {
                PlaySound(w->click_sound);
            }
        }
    }
    else if (w->game.state == X_WIN || w->game.state == O_WIN || w->game.state == STALEMATE) {
        if (IsGestureDetected(GESTURE_TAP)) {
            ResetBoard(w->game.board);
            w->game.state = X_TURN;
        }
    }

    if (!IsGameOver(w->game.state)) {
        enum GameState win = CheckWin(w->game.board);
        if (win == X_WIN) {
            score_x += 1;
            w->game.state = X_WIN;
        } else if (win == O_WIN) {
            score_o += 1;
            w->game.state = O_WIN;
        }
        else if (CheckTie(w->game.board)) {
            w->game.state = STALEMATE;
        }
    }
}

void DrawEverything(struct Window* w) {
    BeginDrawing();

    ClearBackground(BACKGROUND);

    DrawOutline((Vector2) { 0, 0 }, (Vector2) { SCREEN_WIDTH, SCREEN_HEIGHT }, false);
    DrawScore(w->game.state);
    DrawBoard(*w);

    if (IsGameOver(w->game.state)) {
        DrawGameOver(w->game.state);
    }

    EndDrawing();
}

void CloseGame(struct Window* w) {
    UnloadSound(w->click_sound);     // Unload sound data

    CloseAudioDevice();     // Close audio device

    CloseWindow();        // Close window and OpenGL context
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    struct Window w = { 0 };
    InitGame(&w);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        HandleUpdate(&w);

        // Draw
        //----------------------------------------------------------------------------------
        DrawEverything(&w);
        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseGame(&w);
    //--------------------------------------------------------------------------------------

    return 0;
}
