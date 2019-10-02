#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

/* new_game: constructs a new game with an empty board */
game* new_game(unsigned int side, enum type type){
	game* res = (game*) malloc(sizeof(game));
	board* board = board_new(side, type);
	res->b = board;
	res->next = WHITE_NEXT;
	return res;
}

/* game_free: frees the memory allocated to the game */
void game_free(game* g){
	board_free(g->b);
	free(g);
}

/* place_marble: place a colored marble,
 * the color is decided by whose turn it is,
 * returns 1 if a marble is placed, returns 0 otherwise */
int place_marble(game* g, pos p){
	square current = board_get(g->b, p);
	if(current == EMPTY){
		switch(g->next){
			case WHITE_NEXT:
				board_set(g->b, p, WHITE);
				return 1;
			case BLACK_NEXT:
				board_set(g->b, p, BLACK);
				return 1;
			default:
				fprintf(stderr, "place_marble: invalid turn"
						"\n\n");
				exit(1);
		}
	}
	return 0;
}

/* adjust_for_quadrant: adjusts row and column headd on quadrant */
void adjust_for_quadrant(game* g, quadrant q, unsigned int* row, 
		unsigned int* col){
	unsigned int quad = g->b->side / 2;
	switch(q){
		case NW:	
			break;
		case NE:
			*col += quad;
			break;
		case SW:
			*row += quad;
			break;
		case SE:
			*row += quad;
			*col += quad;
			break;
		default:
			fprintf(stderr, "adjust_for_quadrant: "
					"invalid quadrant\n\n");
			exit(1);
	}
}

/* rotate_quadrant: rotates a quadrant of the board in the given direction */
void rotate_quadrant(game* g, direction d, unsigned int row, unsigned int col){
	unsigned int quad = g->b->side / 2;
	int row_adj = row - col,
	    col_adj = col - row;
	for(int i = row; i < row + (quad / 2); ++i){
		int i_inc = i - row,
		    i_max = row + quad - i_inc - 1;
		for(int j = col + i_inc; j < i_max + col_adj; ++j){
			int j_inc = j - col,
			    j_max = col + quad - j_inc - 1;
			unsigned int r2 = i_max,
				     c2 = j_max,
				     r1, c1, r3, c3;
			switch(d){
				case CW:
					r1 = j_max + row_adj;
				        c1 = i + col_adj;
				        r3 = j + row_adj;
				        c3 = i_max + col_adj;
					break;
				case CCW:
					r1 = j + row_adj;
					c1 = i_max + col_adj;
					r3 = j_max + row_adj;
					c3 = i + col_adj;
					break;
				default:
					fprintf(stderr, "rotate_quadrant: "
							"invalid dir\n\n");
					exit(1);
			}
			pos p0 = make_pos(i, j),
			    p1 = make_pos(r1, c1),
			    p2 = make_pos(r2, c2),
			    p3 = make_pos(r3, c3);
			square s0 = board_get(g->b, p0),
			       s1 = board_get(g->b, p1),
			       s2 = board_get(g->b, p2),
			       s3 = board_get(g->b, p3);
			board_set(g->b, p0, s1);
			board_set(g->b, p1, s2);
			board_set(g->b, p2, s3);
			board_set(g->b, p3, s0);
		}
	}
}

/* switch_turn: switches the turn of the game */
void switch_turn(game *g){
	switch(g->next){
		case WHITE_NEXT:
			g->next = BLACK_NEXT;
			break;
		case BLACK_NEXT:
			g->next = WHITE_NEXT;
			break;
		default:
			fprintf(stderr, "switch_turn: invalid turn\n\n");
			exit(1);
	}
}

/* twist_quadrant: twists the quadrant of the game board 
 * in the given direction, then switches turn afterwards */
void twist_quadrant(game* g, quadrant q, direction d){
	unsigned int row = 0,  /* index of the top-left corner of */
		     col = 0;  /* the quadrant being rotated */
	adjust_for_quadrant(g, q, &row, &col);
	rotate_quadrant(g, d, row, col); 
	switch_turn(g);
}

/* horiz_check: checks rows for win */
void horiz_check(board* b, pos head, int* color_win){
	for(int k = 1; k < (b->side - 1); ++k){
		pos tail = make_pos(head.r, head.c + k);
		if(board_get(b, head) == board_get(b, tail)){
			continue;
		}
		return;
	}
	*color_win = 1;
}

/* vert_check: checks columns for win */
void vert_check(board* b, pos head, int* color_win){
	for(int k = 1; k < (b->side - 1); ++k){
		pos tail = make_pos(head.r + k, head.c);
		if(board_get(b, head) == board_get(b, tail)){
			continue;
		}
		return;
	}
	*color_win = 1;
}

/* asc_diag_check: checks ascending diagonals for win */
void asc_diag_check(board* b, pos head, int* color_win){
	for(int k = 1; k < (b->side - 1); ++k){
		pos tail = make_pos(head.r - k, head.c + k);
		if(board_get(b, head) == board_get(b, tail)){
			continue;
		}
		return;
	}
	*color_win = 1;
}

/* desc_diag_check: checks descending diagonals for win */ 
void desc_diag_check(board* b, pos head, int* color_win){
	for(int k = 1; k < (b->side - 1); ++k){
		pos tail = make_pos(head.r + k, head.c + k);
		if(board_get(b, head) == board_get(b, tail)){
			continue;
		}
		return;
	}
	*color_win = 1;
}

/* color_check: if the current square matches a color, and the color
 * has not won yet, this function applies the check function 
 * to the current square */
void color_check(void (*check)(board*, pos, int*), 
		board* b, int row, int col, int* wh_win, int* blk_win){
	pos head = make_pos(row, col);
	square head_s = board_get(b, head);
	if(head_s == WHITE && *wh_win == 0){
		(*check)(b, head, wh_win);
	} else if(head_s == BLACK && *blk_win == 0){
		(*check)(b, head, blk_win);
	}
}

/* full_check: checks board to see if it is full */
int full_check(board* b){
	for(int i = 0; i < b->side; ++i){
		for(int j = 0; j < b->side; ++j){
			pos current = make_pos(i, j);
			if(board_get(b, current) == EMPTY){
				return 0;
			}
		}
	}
	return 1;
}

/* win_check: check the game board to see if the game has
 * ended in a white win, black win, or a draw 
 * and returns result as out-parameters */
void win_check(game* g, int* wh_win, int* blk_win, int* draw){
	*wh_win = 0;
	*blk_win = 0;
	*draw = 0;
	for(int i = 0; i < g->b->side; ++i){
		for(int j = 0; j < 2; ++j){
			color_check(&horiz_check, g->b, i, j, wh_win, blk_win);
		}
	}
	for(int i = 0; i < 2; ++i){
		for(int j = 0; j < g->b->side; ++j){
			color_check(&vert_check, g->b, i, j, wh_win, blk_win);
		}
	}
	for(int i = (g->b->side - 2); i < g->b->side; ++i){
		for(int j = 0; j < 2; ++j){
			color_check(&asc_diag_check, g->b, i, j, 
					wh_win, blk_win);
		}
	}	
	for(int i = 0; i < 2; ++i){
		for(int j = 0; j < 2; ++j){
			color_check(&desc_diag_check, g->b, i, j, 
					wh_win, blk_win);
		}
	}
	if(*wh_win == 1 && *blk_win == 1){ 
		*wh_win = 0;
		*blk_win = 0;
		*draw = 1; 
	} else if(*wh_win == 0 && *blk_win == 0){
		*draw = full_check(g->b);
	}
}

/* game_over: checks game to see if a player has won or if there is a draw */
int game_over(game* g){
	int wh_win, blk_win, draw;
	win_check(g, &wh_win, &blk_win, &draw);
	if(wh_win == 1 || blk_win == 1 || draw == 1){
		return 1;
	}
	return 0;
}

/* game_outcome: returns the outcome of a game that has been completed */
outcome game_outcome(game* g){
	int wh_win, blk_win, draw;
	win_check(g, &wh_win, &blk_win, &draw);
	if(wh_win == 1){
		return WHITE_WIN;
	} else if(blk_win == 1){
		return BLACK_WIN;
	} else if(draw == 1){
		return DRAW;
	} else{
		return -1; /* meaningless, should never be reached */
	}
}
