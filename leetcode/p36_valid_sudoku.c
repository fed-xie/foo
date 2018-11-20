#include <stdlib.h>
#include <stdbool.h>

bool isValidSudoku(char** board, int boardRowSize, int boardColSize)
{
	char chkrow[9];
	char chkcol[9][9];
	char chkbox[3][9];
	memset(chkcol, 0, sizeof(chkcol));
	for (int row=0; row<boardRowSize; ++row) {
		memset(chkrow, 0, sizeof(chkrow));
		if (0 == row % 3)
			memset(chkbox, 0, sizeof(chkbox));
		for (int col=0; col<boardColSize; ++col) {
			if ('.' != board[row][col]) {
				int n = board[row][col] - '1';
				
				if (chkrow[n])
					return false;
				else
					chkrow[n] = 1;
				
				if (chkcol[col][n])
					return false;
				else
					chkcol[col][n] = 1;
				
				//int boxi = (row/3) * 3 + col/3;
				//int boxn = (row%3) * 3 + col%3;
				int boxi = col/3;
				if (chkbox[boxi][n])
					return false;
				else
					chkbox[boxi][n] = 1;
			}
		}
	}
	
	return true;
}

int main(int argc, char** argv)
{
	char* puzzle[] = {
		"519748632",
		"783652419",
		"426139875",
		"357986241",
		"264317598",
		"198524367",
		"975863124",
		"832491756",
		"64127598."
	};
	printf("%s\n", isValidSudoku(puzzle, 9, 9) ? "valid" : "invalid");
	return 0;
}