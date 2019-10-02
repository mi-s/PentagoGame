#ifndef _POS_H
#define _POS_H

/* pos: represents a location on the board */
struct pos {
	unsigned int r, c;
};

typedef struct pos pos;

/* make_pos: creates a pos at the given row/column index,
 * row/column indexing is zero-based (top-left corner is (0,0)) */
pos make_pos(unsigned int r, unsigned int c);

#endif /* _POS_H */
