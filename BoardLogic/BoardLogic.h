#include <stdbool.h>

#define NUM_TILES 3

enum GameState {
    START = 0,
    X_TURN,
    O_TURN,
    X_WIN,
    O_WIN,
    STALEMATE,
};

#define TURN_WIN_OFFSET 2
static_assert(X_TURN + TURN_WIN_OFFSET == X_WIN, "X turn to win offset is wrong!");
static_assert(O_TURN + TURN_WIN_OFFSET == O_WIN, "O turn to win offset is wrong!");

struct Game {
    enum GameState board[NUM_TILES][NUM_TILES];
    enum GameState state;
};

#ifdef __cplusplus
extern "C" {
#endif

enum GameState CheckWin(enum GameState board[NUM_TILES][NUM_TILES]);

bool CheckTie(enum GameState board[NUM_TILES][NUM_TILES]);

void ResetBoard(enum GameState board[NUM_TILES][NUM_TILES]);
#ifdef __cplusplus
}
#endif