#include "pch.h"
#include "BoardLogic.h"

struct BoardTestCase {
	enum GameState board[NUM_TILES][NUM_TILES];
	enum GameState expected_state;
};

TEST(CheckWinDiagonal, XPositive) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ X_TURN, START,  START  },
				{ START,  X_TURN, START  },
				{ START,  START,  X_TURN }
			},
			X_WIN
		},
		{
			{
				{ START,  START,  X_TURN },
				{ START,  X_TURN, START  },
				{ X_TURN, START,  START  }
			},
			X_WIN
		},
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}

TEST(CheckWinDiagonal, XNegative) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ X_TURN, START,  START },
				{ START,  X_TURN, START },
				{ START,  START,  START }
			},
			START,
		},
		{
			{
				{ X_TURN, START,  START },
				{ START,  X_TURN, START },
				{ START,  START,  O_TURN }
			},
			START,
		},
		{
			{
				{ START, START,  START  },
				{ START, X_TURN, START  },
				{ START, START,  X_TURN }
			},
			START
		},
		{
			{
				{ O_TURN, START,  START  },
				{ START,  X_TURN, START  },
				{ START,  START,  X_TURN }
			},
			START
		}
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}

TEST(CheckWinDiagonal, OPositive) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ O_TURN, START,  START  },
				{ START,  O_TURN, START  },
				{ START,  START,  O_TURN }
			},
			O_WIN
		},
		{
			{
				{ START,  START,  O_TURN },
				{ START,  O_TURN, START  },
				{ O_TURN, START,  START  }
			},
			O_WIN
		}
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}

TEST(CheckWinDiagonal, ONegative) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ O_TURN, START,  START },
				{ START,  O_TURN, START },
				{ START,  START,  START }
			},
			START,
		},
		{
			{
				{ O_TURN, START,  START },
				{ START,  O_TURN, START },
				{ START,  START,  X_TURN }
			},
			START,
		},
		{
			{
				{ START, START,  START  },
				{ START, O_TURN, START  },
				{ START, START,  O_TURN }
			},
			START
		},
		{
			{
				{ X_TURN, START,  START  },
				{ START,  O_TURN, START  },
				{ START,  START,  O_TURN }
			},
			START
		}
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}

TEST(CheckWinHorizontal, XPositive) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ X_TURN, X_TURN, X_TURN },
				{ START,  START,  START  },
				{ START,  START,  START  }
			},
			X_WIN
		},
		{
			{
				{ START,  START,  START  },
				{ X_TURN, X_TURN, X_TURN },
				{ START,  START,  START  }
			},
			X_WIN
		},
		{
			{
				{ START,  START,  START  },
				{ START,  START,  START  },
				{ X_TURN, X_TURN, X_TURN }
			},
			X_WIN
		}
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}

TEST(CheckWinHorizontal, OPositive) {
	struct BoardTestCase test_cases[] = {
		{
			{
				{ O_TURN, O_TURN, O_TURN },
				{ START,  START,  START  },
				{ START,  START,  START  }
			},
			O_WIN
		},
		{
			{
				{ START,  START,  START  },
				{ O_TURN, O_TURN, O_TURN },
				{ START,  START,  START  }
			},
			O_WIN
		},
		{
			{
				{ START,  START,  START  },
				{ START,  START,  START  },
				{ O_TURN, O_TURN, O_TURN }
			},
			O_WIN
		}
	};

	for (int i = 0; i < sizeof(test_cases) / sizeof(struct BoardTestCase); i++) {
		EXPECT_EQ(CheckWin(test_cases[i].board), test_cases[i].expected_state);
	}
}
