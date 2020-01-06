#include <stdio.h>
float sqrt(int input) {
	float x = 1;
	int i = 0;
	for (i = 0; i < 10; i++) {
		x = (x + (input / x)) / 2;
	}
	return x;
}//to get route of the number

void getPathLengthData(float arr[7][7], int dot[7][2]) {

	int i, j;
	int x1, y1;
	int x2, y2;
	int x_length, y_length, temp;
	float res;

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {


			x1 = dot[i][0];
			y1 = dot[i][1];

			x2 = dot[j][0];
			y2 = dot[j][1];

			x_length = x2 - x1;
			y_length = y2 - y1;

			temp = (x_length * x_length) + (y_length * y_length);
			res = sqrt(temp);

			arr[i][j] = res;

		}
	}
}

int main() {
	int dot[7][2] = {0, 0, 2, 6, 8, 4, 7, 2, 1, 6, 4, 9, 3, 2 };
	float arr[7][7] = { 0, };
	getPathLengthData(arr, dot);
	float dist = 0;
	float min = 99999;
	int r[7] = { 1, 0, 0, 0, 0, 0, 7 }; //route
	int save_route[7] = { 0, };
	//depth 1
	for (int d1 = 1; d1 <= 5; d1++) {
		dist = 0;
		dist = arr[0][d1];
		//depth 2;
		for (int d2 = 1; d2 <= 5; d2++) {
			if (d1 == d2) {
				continue;
			}
			dist = arr[0][d1] + arr[d1][d2];
			if (dist > min) continue;
			//depth 3
			for (int d3 = 1; d3 <= 5; d3++) {
				if (d1 == d3 || d2 == d3) {
					continue;
				}
				dist = arr[0][d1] + arr[d1][d2] + arr[d2][d3];
				if (dist > min) continue;
				//depth 4
				for (int d4 = 1; d4 <= 5; d4++) {
					if (d1 == d4 || d2 == d4 || d3 == d4) {
						continue;
					}
					dist = arr[0][d1] + arr[d1][d2] + arr[d2][d3] + arr[d3][d4];
					if (dist > min) continue;
					//depth 5
					for (int d5 = 1; d5 <= 5; d5++) {
						if (d1 == d5 || d2 == d5 || d3 == d5 || d4 == d5) {
							continue;
						}
						dist = arr[0][d1] + arr[d1][d2] + arr[d2][d3] + arr[d3][d4] + arr[d4][d5] + arr[d5][6];
						r[1] = d1 + 1;
						r[2] = d2 + 1;
						r[3] = d3 + 1;
						r[4] = d4 + 1;
						r[5] = d5 + 1;
						if (dist < min) {
							save_route[0] = r[0];
							save_route[1] = r[1];
							save_route[2] = r[2];
							save_route[3] = r[3];
							save_route[4] = r[4];
							save_route[5] = r[5];
							save_route[6] = r[6];
							min = dist;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < 7; i++) {
		printf("%d ", save_route[i]);
	}
	printf("\n%f", min);
	// print the result

}
