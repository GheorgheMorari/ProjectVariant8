#pragma once

#include <stdio.h>
#include <stdlib.h>


float add(float left, float right) {
	return left + right;
}
float sub(float left, float right) {
	return left - right;
}
float mult(float left, float right) {
	return left * right;
}
float divide(float left, float right) {
	return left / right;
}

int tenToPower(int n) {     //function that returns the 10^n
	int i, temp = 1;
	for (i = 0; i < n; i++) {
		temp *= 10;
	}
	return temp;
}


void printarr(int n, char* arr) {  // prints out the contents of the array for debugging purposes
	int j;
	for (j = 0; j <= n; j++)
		printf("%c", arr[j]);
	printf("\n");
}


