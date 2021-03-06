// CoolNumberSquareFromHackerRank.cpp : A cool problem from HackerRank (HackerRant)
//

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
int NumberRowsAwayFromClosestOfTopOrBottomRow(int n, int row)
{
	if (row <= n) return row; // 2, 1 should return 1
	return 2 * n - 2 - row;
}

int main()
{
	int n;
	std::cin >> n;
	// Complete the code to print the pattern.
	int i, j, val, incr;
	for (i = 0; i < 2 * n - 1; ++i) {
		val = n; incr = -1;
		for (j = 0; j < 2 * n - 1; ++j) {
			printf("%d ", val);
			if (j == NumberRowsAwayFromClosestOfTopOrBottomRow(n, i))  ++incr;
			if (2 * n - 2 - NumberRowsAwayFromClosestOfTopOrBottomRow(n, i) == j) ++incr;
			val += incr;
		}
		printf("\n");
	}
	return 0;
}
