#ifndef _BOARD_H
#define _BOARD_H

#include "pos.h"

/* square: represents the contents of a location on the board */
enum square{
	EMPTY,
	BLACK,
	WHITE
};

typedef enum square square;

/* board_rep: cells is a two-dimensional array of squares on the board,
 * bits is a much more compact method of storing the board's information
 * (cells is used for project part 1, bits used for project part 2) */
union board_rep{
	enum square** cells;
	unsigned int* bits;
};

typedef union board_rep board_rep;

enum type{
	CELLS, BITS
};

/* board: representation of a game board,
 * side is the length of the board's side,
 * type and u are used to represent the board's storage method */
struct board{
	unsigned int side;
	enum type type;
	board_rep u;
};

typedef struct board board;

/* board_new: construct a new, empty board */
board* board_new(unsigned int side, enum type type);

/* board_free: free the memory allocated to a board */
void board_free(board* b);

/* board_show: prints the board */
void board_show(board* b);

/* board_get: returns the square at the board position */
square board_get(board* b, pos p);

/* board_set: modifies the square at the board position */
void board_set(board* b, pos p, square s);

#endif /* _BOARD_H */
