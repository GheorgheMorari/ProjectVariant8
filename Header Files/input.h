#pragma once

#include <stdio.h>
#include <stdlib.h>


char* inputText(int* len) {   //Inputting the array.
	int i;
#pragma warning(suppress : 4996)
	FILE* tes = fopen("input.txt", "r");
	fseek(tes, 0L, SEEK_END);
	int n = ftell(tes);
	fclose(tes);
#pragma warning(suppress : 4996)
	FILE* Input = fopen("input.txt", "r+");
	char* arr = (char*)malloc(sizeof(char) *(n+1));
	for (i = 0; i <= n; i++)
		fscanf_s(Input, "%c", &arr[i]);
	len[0] = n;
	fclose(Input);
	return(arr);
	free(arr);
	
}