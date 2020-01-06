#include <stdio.h>

// function to get root of number
float sqrt(int input) {
	float x = 1;
	int i = 0;
	for (i = 0; i < 10; i++) {
		x = (x + (input / x)) / 2;
	}
	return x;
}
// function to get length of cities
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



int main()
{
	float arr[7][7];
	int dot[7][2] = {0, 0, 2, 6, 8, 4, 7, 2, 1, 6, 4, 9, 3, 2};
	double dist_min=99999, dist=0;
	int course[6];

	getPathLengthData(arr, dot);

	//depth1
	for (int d1 = 1; d1 < 7; d1++)
	{
		dist = arr[0][d1];
		if (dist > dist_min)
			continue;	
		double dist1 = dist;

		//depth2
		for (int d2 = 1; d2 < 7; d2++)
		{
			if (d2 == d1)
				continue;
			dist = dist1 + arr[d1][d2];
			if (dist > dist_min)
				continue;
			double dist2 = dist;

			//depth3
			for (int d3 = 1; d3 < 7; d3++)
			{
				if (d3 == d2 || d3 == d1)
					continue;
				dist = dist2 + arr[d2][d3];
				if (dist > dist_min)
					continue;
				double dist3 = dist;

				//depth4
				for (int d4 = 1; d4 < 7; d4++)
				{
					if (d4 == d3 || d4 == d2 || d4 == d1)
						continue;
					dist = dist3 + arr[d3][d4];
					if (dist > dist_min)
						continue;
					double dist4 = dist;

					//depth5
					for (int d5 = 1; d5 < 7; d5++)
					{
						if (d5 == d4 || d5 == d3 || d5 == d2 || d5 == d1)
							continue;
						dist = dist4 + arr[d4][d5];
						if (dist > dist_min)
							continue;
						double dist5 = dist;

						//depth6
						for (int d6 = 1; d6 < 7; d6++)
						{
							if (d6 == d5 || d6 == d4 || d6 == d3 || d6 == d2 || d6 == d1)
								continue;
							dist = dist5 + arr[d5][d6] + arr[d6][0];
							if (dist < dist_min)
							{
								course[0] = d1;
								course[1] = d2;
								course[2] = d3;
								course[3] = d4;
								course[4] = d5;
								course[5] = d6;
								dist_min = dist;
							}
						}
					}
				}
			}
		}
	}
	printf("1 %d %d %d %d %d %d 1\n%lf\n", course[0]+1, course[1]+1, course[2]+1, course[3]+1, course[4]+1, course[5]+1, dist_min);
}
