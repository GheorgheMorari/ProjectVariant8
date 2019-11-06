#pragma once

#include <stdio.h>
#include <stdlib.h>

float tenToPower(int n) {     //function that returns the 10^n
	float i, temp = 1;
	if (n < 0) {
		for (i = 0; i < abs(n); i++)
			temp /= 10;
	}
	else
		for (i = 0; i < n; i++) {
			temp *= 10;
		}
	return temp;
}

void printarr(int n, char* arr) {  // prints out the contents of the array for debugging purposes
	int j;
	for (j = 0; j < n; j++)
		printf("%c", arr[j]);
	printf("\n");
}


