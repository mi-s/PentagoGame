#ifndef _LOGIC_H
#define _LOGIC_H

#include "board.h"


enum turn {
    	BLACK_NEXT,
	WHITE_NEXT
};

typedef enum turn turn;

enum outcome {
	BLACK_WIN,
	WHITE_WIN,
	DRAW
};

typedef enum outcome outcome;

enum quadrant {
	NW, NE, SW, SE
};

typedef enum quadrant quadrant;

enum direction {
	CW, CCW
};

typedef enum direction direction;

/* game: a game consists of a board and a turn corresponding to a color */
struct game {
	board* b;
	turn next;
};

typedef struct game game;

/* new_game: constructs a new game with an empty board */
game* new_game(unsigned int side, enum type type);

/* game_free: frees the memory allocated to the game */
void game_free(game* g);

/* place_marble: place a colored marble,
 * the color is decided by whose turn it is,
 * returns 1 if a marble is placed, returns 0 otherwise */
int place_marble(game* g, pos p);

/* twist_quadrant: twists the quadrant of the game board 
 * in the given direction, then switches turn */
void twist_quadrant(game* g, quadrant q, direction d);

/* game_over: checks game to see if a player has won if or there is a draw */
int game_over(game* g);

/* game_outcome: returns the outcome of a game that has been completed */
outcome game_outcome(game* g);

#endif /* _LOGIC_H */
