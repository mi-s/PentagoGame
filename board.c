#include <stdio.h>
#include <stdlib.h>
#include "board.h"

/* cells_board_new: board_new function when board is of type CELLS */
board* cells_board_new(unsigned int side){
	board* res = (board*) malloc(sizeof(board));
	res->side = side;
	res->type = CELLS;
	square** cells = (square**) malloc(side * sizeof(square*));
	for(int i = 0; i < side; ++i){
		cells[i] = (square*) malloc(side * sizeof(square));
	}
	for(int i = 0; i < side; ++i){
		for(int j = 0; j < side; ++j){
			cells[i][j] = EMPTY;
		}
	}
	res->u.cells = cells;
	return res;
}

/* bits_len: returns length of the storage array for board of type BITS */
unsigned int bits_len(unsigned int side){
	unsigned int n = side * side,
		     elements = n / 16;
	if(n % 16 > 0){
		return elements + 1;
	} else{
		return elements;
	}
}

/* bits_board_new: board_new function when board is of type BITS */
board* bits_board_new(unsigned int side){
	board* res = (board*) malloc(sizeof(board));
	res->side = side;
	res->type = BITS;
	unsigned int len = bits_len(side);
	unsigned int* bits = malloc(len * sizeof(unsigned int));
	for(int i = 0; i < len; ++i){
		bits[i] = 0;
	}
	res->u.bits = bits;
	return res;
}

/* board_new: construct a new, empty board */
board* board_new(unsigned int side, enum type type){
	if((side < 4) || (side % 2 != 0)){
		fprintf(stderr, "board_new: invalid side length\n\n");
		exit(1);
	}
	switch(type){
		case CELLS:
			return cells_board_new(side);
		case BITS:
			return bits_board_new(side);
		default:
			fprintf(stderr, "board_new: invalid board type\n\n");
			exit(1);
	}	
}

/* cells_board_free: board_free function when board is of type CELLS */
void cells_board_free(board* b){
	for(int i = 0; i < b->side; ++i){
		free(b->u.cells[i]);
	}
	free(b->u.cells);
	free(b);
}

/* bits_board_free: board_free function when board is of type BITS */
void bits_board_free(board* b){
	free(b->u.bits);
	free(b);
}

/* board_free: free the memory allocated to a board */
void board_free(board* b){
	switch(b->type){
		case CELLS:
			cells_board_free(b);
			break;
		case BITS:
			bits_board_free(b);
			break;
		default:
			fprintf(stderr, "board_free: invalid board type\n\n");
			exit(1);
	}
}

/* print_header_char: prints a char and then increments the char value by
 * the appropriate amount, skipping unused ranges of char ASCII values */
 void print_header_char(char* char_val){
	if(*char_val == 123){
		printf("?");
	} else if(*char_val == 90){
		printf("%c", *char_val);
		*char_val += 7;
	} else if(*char_val == 57){
		printf("%c", *char_val);
		*char_val += 8;
	} else{
		printf("%c", *char_val);
		*char_val += 1;
	}
}

/* print_col_header: prints out the row of column headers */
void print_col_header(board* b, char* col_header){
	unsigned int board_dim = b->side + 3,
		     blank = (board_dim + 1) / 2;
	for(int i = 0; i < board_dim; ++i){
		if(i < 2 || i == blank){
			printf(" ");
		} else{
			print_header_char(col_header);
		}
	}
}

/* print_empty_row: prints an empty row of a board */
void print_empty_row(board* b){
	unsigned int board_dim = b->side + 3;
	for(int i = 0; i < board_dim; ++i){
		printf(" ");
	}
}

/* cells_print_row: prints out a row of a board using the board's cell data */
void cells_print_row(board* b, char* row_header, unsigned int* row){
	square* cells_row = b->u.cells[*row];
	unsigned int board_dim = b->side + 3,
		     blank = (board_dim + 1) / 2,
		     col = 0;
	for(int i = 0; i < board_dim; ++i){
		if(i == 0){
			print_header_char(row_header);
			continue;
		} else if(i == 1 || i == blank){
			printf(" ");
			continue;
		} 
		switch(cells_row[col]){
			case WHITE:
				printf("o");
				break;
			case BLACK:
				printf("*");
				break;
			case EMPTY:
				printf(".");
				break;
			default:
				fprintf(stderr, "cells_print_row: "
						"invalid square\n\n");
				exit(1);
		}
		col += 1;
	}
	*row += 1;
}

/* bit_extract: given a BITS array and integer n,
 * return the square at the n'th position of the array (zero-based) */
square bits_extract(unsigned int* bits, unsigned int n){
	unsigned int bits_index = n / 16,
		     binary_index = (n % 16) * 2;
	return 3 & (bits[bits_index] >> binary_index);
}

/* bits_print_row: prints out a row using a board's BITS data */
void bits_print_row(board* b, char* row_header, unsigned int* n){
	unsigned int board_dim = b->side + 3,
		     blank = (board_dim + 1) / 2;
	for(int i = 0; i < board_dim; ++i){
		if(i == 0){
			print_header_char(row_header);
			continue;
		} else if(i == 1 || i == blank){
			printf(" ");
			continue;
		}
		square current = bits_extract(b->u.bits, *n);
		switch(current){
			case EMPTY:
				printf(".");
				break;
			case WHITE:
				printf("o");
				break;
			case BLACK:
				printf("*");
				break;
			default:
				fprintf(stderr, "bits_print_row: "
						"invalid square\n\n");
				exit(1);
		}
		*n += 1;
	}
}

/* board_show: prints the board */
void board_show(board* b){
	unsigned int side = b->side,
		     board_dim = side + 3,
		     blank = (board_dim + 1) / 2,
		     n = 0;
	char row_header = 48,
	     col_header = 48;
	for(int i = 0; i < board_dim; ++i){
		if(i == 0){
			print_col_header(b, &col_header);
		} else if(i == 1 || i == blank){
			print_empty_row(b);
		} else{
			switch(b->type){
				case CELLS:
					cells_print_row(b, &row_header, &n);
					break;
				case BITS:
					bits_print_row(b, &row_header, &n);
					break;
				default:
					fprintf(stderr, "board_show: invalid "
						        "board type\n\n");
					exit(1);
			}
		}
		printf("\n");
	}
}

/* within_board: checks if p is within b
 * (helper for board_get and board_set) */
int within_board(board* b, pos p){
	if(p.r < b->side && p.c < b->side){
		return 1;
	}
	return 0;
}

/* bits_board_get: board_get for boards of type BITS */
square bits_board_get(board* b, pos p){
	unsigned int n = (p.r * b->side) + p.c;
	return bits_extract(b->u.bits, n);
}

/* board_get: returns the square at the given board position */
square board_get(board* b, pos p){
	if(within_board(b, p) == 0){
		fprintf(stderr, "board_get: position outside of board\n\n");
		exit(1);
	}
	switch(b->type){
		case CELLS:
			return b->u.cells[p.r][p.c];
		case BITS:
			return bits_board_get(b, p);
		default:
			fprintf(stderr, "board_get: invalid board type\n\n");
			exit(1);
	}
}

/* bits_board_set: board_set for boards of type BITS */
void bits_board_set(board* b, pos p, square s){
	unsigned int n = (p.r * b->side) + p.c,
		     bits_index = n / 16,
		     num_index = n % 16,
		     b1 = 0, /* binary values */ 
		     b2 = 0;
	if(s == WHITE){
		b1 = 1;
	} else if(s == BLACK){
		b2 = 1;
	}
	unsigned int b1_index = (num_index * 2) + 1,
		     b2_index = b1_index - 1,
		     mask1 = 1 << b1_index,
		     mask2 = 1 << b2_index;
	unsigned int* num = &(b->u.bits[bits_index]);
	*num = (*num & ~mask1) | ((b1 << b1_index) & mask1);
	*num = (*num & ~mask2) | ((b2 << b2_index) & mask2);
}		

/* board_set: modifies the square at the given board position */
void board_set(board* b, pos p, square s){
	if(within_board(b, p) == 0){
		fprintf(stderr, "board_set: position outside of board\n\n");
		exit(1);
	}	
	switch(b->type){
		case CELLS:
			b->u.cells[p.r][p.c] = s;
			break;
		case BITS:
			bits_board_set(b, p, s);
			break;
		default:
			fprintf(stderr, "board_set: invalid board type\n\n");
			exit(1);
	}
}
