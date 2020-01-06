#include<stdio.h>
#include<stdlib.h>

typedef struct block {
	int x, y;
}block;

int search(int x, int y, int vec);
int up(int, int);
int down(int, int);
int right(int, int);
int left(int, int);

int flag;
int maze[300][300];
int chk[300][300];
int root_x[500];
int root_y[500];
int root_comp[500];
int index_root;
FILE *read, *write;
int n, m, k;
int num;

int main() {
	int i, j;
	block comp[5] = { 0, };
	read = fopen("hw2_input.txt", "r");
	write = fopen("hw2_output.txt", "w");
	fscanf(read, "%d %d", &n, &m);
	int res = 0;
	int cot = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			char temp;
			fscanf(read, "%c", &temp);
			if (temp == ' ' || temp == '\n') {
				j--;
				continue;
			}
			else if (temp == '*') {
				maze[i][j] = -1;
				chk[i][j] = -2;
			}
			else {
				maze[i][j] = temp - 48;
				if (temp - 48 != 0) {
					comp[k].x = j;
					comp[k].y = i;
					k++;
				}
			}
		}
	}
	root_x[0] = comp[0].y;
	root_y[0] = comp[0].x;
	res = search(comp[0].y, comp[0].x, 0);
	if (res == 0) {
		fprintf(write, "NULL\n");
	}
	else {
		for (i = 0;; i++) {
			if (root_comp[i] != 0) {
				cot++;
				if (cot == k) {
					break;
				}
			}
		}
		fprintf(write, "%d\n", i);
		cot = 0;
		int h1 = 0;
		int h2 = 0;
		for (i = 1; ; i++) {
			if (root_comp[i] != 0) {
				h2 = i;
				break;
			}
		}
		while (1) {
			for (;; h2++) {
				if (root_comp[h2] != 0) {
					cot++;
					fprintf(write, "%d, %d: ", root_comp[h2], root_comp[h1]);
					int h3 = 0;
					for (h3 = h2; h3 >= h1; h3--) {
						fprintf(write, "(%d,%d)", root_x[h3] + 1, root_y[h3] + 1);
						if (h3 != h1) {
							fprintf(write, ",");
						}
					}
					fprintf(write, "\n");
					h1 = h2;
				}
				if (cot == (k - 1)) {
					break;
				}
			}
			if (cot == (k - 1)) {
				break;
			}
		}
	}
}

int search(int x, int y, int vec) {
	if (flag == 1) {
		return 1;
	}
	else if (num == k) {
		flag = 1;
		return 1;
	}
	else if (chk[x][y] == -2 || chk[x][y] == -1) {
		root_x[index_root] = 0;
		root_y[index_root] = 0;
		index_root--;
		return 0;
		//벽에 부딫침
	}
	else {
		int count = 0;
		int temp = 0;
		chk[x][y] = -1;
		if (maze[x][y] != 0) {
			root_comp[index_root] = maze[x][y];
			num++;
			if (num == k) {
				return 1;
			}
		}
		switch (vec) {
		case 0:
			if (x - 1 >= 0 && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += up(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y - 1 >= 0 && flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += left(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y + 1 < m&& flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += right(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x + 1 < n && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += down(x, y);
				}
				if (count == 1)
					return 1;
			}
			break;
		case 1:
			if (x + 1 < n && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += down(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y - 1 >= 0 && flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += left(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y + 1 < m&& flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += right(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x - 1 >= 0 && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += up(x, y);
				}
				if (count == 1)
					return 1;
			}
			break;
		case 2:
			if (y - 1 >= 0 && flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += left(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x - 1 >= 0 && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += up(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x + 1 < n && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += down(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y + 1 < m&& flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += right(x, y);
				}
				if (count == 1)
					return 1;
			}
			break;
		case 3:
			if (y + 1 < m&& flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += right(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x - 1 >= 0 && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += up(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (x + 1 < n && flag != 1) {
				if (maze[x][y + 1] > 0) {
					count += right(x, y);
				}
				else if (y > 1 && maze[x][y - 1] > 0) {
					count += left(x, y);
				}
				else {
					count += down(x, y);
				}
				if (count == 1)
					return 1;
			}
			if (y - 1 >= 0 && flag != 1) {
				if (maze[x + 1][y] > 0) {
					count += down(x, y);
				}
				else if (x > 1 && maze[x + 1][y] > 0) {
					count += up(x, y);
				}
				else {
					count += left(x, y);
				}
				if (count == 1)
					return 1;
			}
			break;
		}
		if (count == 0) {
			if (root_comp[index_root] != 0) {
				root_comp[index_root] = 0;
				num--;
			}

			index_root--;
			chk[x][y] = 0;
			return 0;
		}
	}
}

int up(int x, int y) {
	int temp;
	index_root++;
	root_x[index_root] = x - 1;
	root_y[index_root] = y;
	temp = search(x - 1, y, 0);
	return temp;
}

int down(int x, int y) {
	int temp;
	index_root++;
	root_x[index_root] = x + 1;
	root_y[index_root] = y;
	temp = search(x + 1, y, 1);
	return temp;
}

int left(int x, int y) {
	int temp;
	index_root++;
	root_x[index_root] = x;
	root_y[index_root] = y - 1;
	temp = search(x, y - 1, 2);
	return temp;
}

int right(int x, int y) {
	int temp;
	index_root++;
	root_x[index_root] = x;
	root_y[index_root] = y + 1;
	temp = search(x, y + 1, 3);
	return temp;
}