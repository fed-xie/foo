#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define bit(x) (0x1 << x)
#define isbit(n, b) (n & bit(b))
#define setbit(n, b) ((n) |= bit(b))
#define clrbit(n, b) ((n) &= ~bit(b))

#define IPSBL_MASK 0x3FE

//static const uint8_t nsum = 36; //0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8;
struct cell_t {
	uint8_t val; //cell value, 0 for unset
	uint8_t psbl_num; //number of possible values
	uint16_t ipsbl_flag; //bitmask of impossible values
};

struct board_t {
	int filled_num;
	struct cell_t cells[9][9];
};

//index of boxes
static inline int pos2boxi(int row, int col) { return (row/3) * 3 + col/3; }
//index inside box
static inline int pos2boxn(int row, int col) { return (row%3) * 3 + col%3; }
static inline int boxn2row(int boxn, int boxi) { return boxn/3 + (boxi/3) * 3; }
static inline int boxn2col(int boxn, int boxi) { return boxn%3 + (boxi%3) * 3; }

static bool set_cell(struct board_t* board, int row, int col, int val);
static bool propagate_constraint(struct board_t* board, int row, int col, int excluded_val);

static bool propagate_constraint(struct board_t* board, int row, int col, int excluded_val)
{
	struct cell_t* cell = &(board->cells[row][col]);
	if (isbit(cell->ipsbl_flag, excluded_val))
		return true;
	if (cell->val == excluded_val)
		return false;
	
	setbit(cell->ipsbl_flag, excluded_val);
	--cell->psbl_num;
	
	if (1 == cell->psbl_num) {
		for (int n=1; n<=9; ++n) {
			if (!isbit(cell->ipsbl_flag, n))
				return set_cell(board, row, col, n);
		}
		assert(false);
	}
	return true;
}

static bool set_cell(struct board_t* board, int row, int col, int val)
{
	struct cell_t* cell = &(board->cells[row][col]);
	if (cell->val == val)
		return true;
	if (isbit(cell->ipsbl_flag, val))
		return false;
	
	cell->ipsbl_flag = IPSBL_MASK & ~bit(val);
	cell->psbl_num = 1;
	cell->val = val;
	++board->filled_num;
	
	//propagate constraints
	int boxi = pos2boxi(row, col);
	for (int n=0; n<9; ++n) {
		if (n != row &&	!propagate_constraint(board, n, col, val))
			return false;
		if (n != col && !propagate_constraint(board, row, n, val))
			return false;
		int br = boxn2row(n, boxi);
		int bc = boxn2col(n, boxi);
		if (br != row && bc != col && !propagate_constraint(board, br, bc, val))
			return false;
	}
	return true;
}

struct point_t {
	uint8_t row;
	uint8_t col;
};

#if _MSC_VER && !restrict
#define restrict __restrict
#endif
static int pointcmp(
	struct point_t* restrict pa, 
	struct point_t* restrict pb, 
	const struct board_t* board)
{
	return board->cells[pa->row][pa->col].psbl_num - 
		board->cells[pb->row][pb->col].psbl_num;
}

static void quicksort(struct point_t* points, int pointnum, const struct board_t* board)
{
	int l = 0, r = pointnum - 1;
	if (r <= 0)
		return;
	struct point_t point = points[l];
	while (l < r) {
		while (l < r && pointcmp(&point, points+r, board) <= 0)
			--r;
		points[l] = points[r];
		while (l < r && pointcmp(points+l, &point, board) <= 0)
			++l;
		points[r] = points[l];
	}
	points[l] = point;
	quicksort(points, l, board);
	quicksort(points+l+1, pointnum-l-1, board);
}

static bool try_fill(struct board_t* board, struct point_t* points)
{
	uint8_t row, col;
	while (0xFF != points->row) {
		row = points->row;
		col = points->col;
		if (0 == board->cells[row][col].val) //find empty cell to fill
			break;
		else
			++points;
	}
	if (0xFF == points->row)
		return false;
	
	struct board_t snapshot = *board;
	for (int i=1; i<=9; ++i) {
		if (!isbit(board->cells[row][col].ipsbl_flag, i)) {
			if (set_cell(board, row, col, i)) {
				if (81 == board->filled_num  || try_fill(board, points+1))
					return true;
			}
			*board = snapshot; //restore snapshot
		}
	}
	return false;
}

static bool fill_empty_cells(struct board_t* board)
{
	if (81 == board->filled_num)
		return true;
	
	int empty_num = 81 - board->filled_num;
	struct point_t* points = malloc(sizeof(struct point_t) * (empty_num + 1));
	if (NULL == points)
		return false;
	
	int count = 0;
	for (int row=0; row<9; ++row) {
		for (int col=0; col<9; ++col) {
			if (0 == board->cells[row][col].val) {
				points[count].row = row;
				points[count].col = col;
				++count;
			}
		}
	}
	assert(count == empty_num);
	quicksort(points, empty_num, board);
	points[count].row = 0xFF;
	points[count].col = 0xFF;
	
	bool res = try_fill(board, points);
	free(points);
	return res;
}

void solveSudoku(char** board, int boardRowSize, int boardColSize)
{
	struct board_t numboard;
	for (int row=0; row<9; ++row) {
		for (int col=0; col<9; ++col) {
			numboard.cells[row][col].val = 0;
			numboard.cells[row][col].psbl_num = 9;
			numboard.cells[row][col].ipsbl_flag = 0;
		}
	}
	numboard.filled_num = 0;
	
	for (int row=0; row<9; ++row) {
		for (int col=0; col<9; ++col) {
			if ('.' != board[row][col] &&
				!set_cell(&numboard, row, col, board[row][col] - '0'))
				return;
		}
	}
	
	for (int row=0; row<9; ++row) {
		for (int col=0; col<9; ++col) {
			if (numboard.cells[row][col].val)
				board[row][col] = numboard.cells[row][col].val + '0';
		}
	}
	
	if (!fill_empty_cells(&numboard))
		return;
	
	for (int row=0; row<9; ++row)
		for (int col=0; col<9; ++col)
			board[row][col] = numboard.cells[row][col].val + '0';
}

#include <stdio.h>
int main(int argc, char** argv)
{
	char* puzzle[] = {
		//"..9748...",
		//"7........",
		//".2.1.9...",
		//"..7...24.",
		//".64.1.59.",
		//".98...3..",
		//"...8.3.2.",
		//"........6",
		//"...2759.."
		
		"1...7..3.",
		"83.6.....",
		"..29..6.8",
		"6....49.7",
		".9.....5.",
		"3.75....4",
		"2.3..91..",
		".....2.43",
		".4..8...9"
	};
	solveSudoku(puzzle, 9, 9);
	
	for (int row=0; row<9; ++row) {
		for (int col=0; col<9; ++col)
			printf("%c", puzzle[row][col]);
		printf("\n");
	}
	return 0;
}