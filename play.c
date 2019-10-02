#include <stdio.h>
#include <stdlib.h>
#include "logic.h"
#include <string.h>

/* convert_index: converts row/col index into row/col integer
 * returns 999 if the char index is not a valid input */
unsigned int convert_index(char index){
	if(index >= 48 && index <= 57){
		return index - 48;
	} else if(index >= 65 && index <= 90){
		return index - 55;
	} else if(index >= 97 && index <= 122){
		return index - 61;
	} else{
		return 999;  /* indicates an invalid character input */
	}	          
}

/* place: handles the placement of a marble during a turn */
void place(game* g){
	int valid_place = 0;
	while(valid_place == 0){
		switch(g->next){
			case WHITE_NEXT:
				printf("White: ");
				break;
			case BLACK_NEXT:
				printf("Black: ");
				break;
			default:
				fprintf(stderr, "place: invalid turn\n\n");
				exit(1);
		}
		char row_index, col_index;
		scanf(" %c %c", &row_index, &col_index);
		printf("\n");
		unsigned int row = convert_index(row_index),
			     col = convert_index(col_index);
		if(row == 999 || col == 999){
			printf("Invalid character input, "
			       "please reinput ...\n");
		} else if(row >= g->b->side || col >= g->b->side){
			printf("Position not within board, "
			       "please reinput ...\n");
		} else{
			pos position = make_pos(row, col);
			valid_place = place_marble(g, position);	
			if(valid_place == 0){
				printf("Position already filled, "
			               "please reinput ...\n");
			}
		}
	}
}

/* twist: handles the twisting of a quadrant during a turn */
void twist(game* g){
	quadrant quad;
	int valid_quad = 0;
	while(valid_quad == 0){
		printf("Enter integer to select quadrant to twist "
	       	       "(0 for NW, 1 for NE, 2 for SW, 3 for SE): ");
		scanf("%i", &quad);
		if(quad >= 0 && quad <= 3){
			valid_quad = 1;
		} else{
			printf("Invalid quadrant selected, "
			       "please reinput ...\n");
		}
	}
	direction dir;
	int valid_dir = 0;
	while(valid_dir == 0){
		printf("Enter integer to select direction of twist "
	       	       "(0 for CW, 1 for CCW): ");
		scanf("%i", &dir);
		if(dir == 0 || dir == 1){
			valid_dir = 1;
		} else{
			printf("Invalid direction selected, "
			       "please reinput ...\n");
		}
	}
	twist_quadrant(g, quad, dir);
}

/* turn_loop: handles looping of turns for the main function, 
 * when an outcome has been reached, the function ends and the outcome
 * is returned as an out-parameter */
void turn_loop(game* g, outcome* res){
	int g_over = 0,
	    turn = 0;
	while(g_over == 0){
		turn += 1;
		printf("\n*****TURN %i*****\n", turn);
		board_show(g->b);
		place(g);
		g_over = game_over(g);
		if(g_over == 1){
			*res = game_outcome(g);
			switch(*res){
				case WHITE_WIN:
					if(g->next == WHITE_NEXT){
						return;
					}
					break;
				case BLACK_WIN:
					if(g->next == BLACK_NEXT){
						return;
					}
					break;
				case DRAW:
					return;
				default:
					fprintf(stderr, "turn_loop: invalid "
							"outcome\n\n");
					exit(1);
			}
		}
		board_show(g->b);
		twist(g);
		g_over = game_over(g);
	}
	*res = game_outcome(g);
}

/* main: creates a new game of Pentago from command line inputs, 
 * then plays game to completion */
int main(int argc, char* argv[]){
	/* read command line inputs */
	if(argc != 4){
		fprintf(stderr, "main: invalid number of inputs\n");
		exit(1);
	}
	unsigned int side;
	enum type game_type;
	for(int i = 1; i < argc; ++i){
		char* cmd = argv[i];
		if(strcmp(cmd, "-s") == 0){
			char* side_len = argv[i + 1];
			side = atoi(side_len);
			i += 1;
		} else if(strcmp(cmd, "-c") == 0){
			game_type = CELLS;
		} else if(strcmp(cmd, "-b") == 0){
			game_type = BITS;
		} else{	
			fprintf(stderr, "main: invalid command\n");
			exit(1);
		}
	}
	/* create new game and play to completion */
	printf("**********PENTAGO**********\n");
	printf("Creating new game ...\n");
	game* g = new_game(side, game_type);
	outcome res;
	turn_loop(g, &res);
	printf("*****FINAL GAME BOARD*****\n");
	board_show(g->b);
	switch(res){
		case WHITE_WIN:
			printf("\nWhite Wins!\n");
			break;
		case BLACK_WIN:
			printf("\nBlack Wins!\n");
			break;
		case DRAW:
			printf("\nGame has ended in a draw!\n");
			break;
		default:
			fprintf(stderr, "main: invalid outcome\n\n");
			exit(1);
	}
	return 0;
}
