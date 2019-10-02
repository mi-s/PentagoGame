#include <stdio.h>
#include <stdlib.h>
#include "pos.h"

/* make_pos: creates a pos at the given row/column index,
 * row/column indexing is zero-based (top-left corner is (0,0)) */
pos make_pos(unsigned int r, unsigned int c){
	pos res;
	res.r = r;
	res.c = c;
	return res;
}
