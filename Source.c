#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "operations.h"
#define maxOperations 255
#define debug 1
// float version branch from scratch

struct operation {
	int  leftEdge, rightEdge, position, leftminus, rightminus;
	float leftNumber, rightNumber;
};

struct operation* operations;
char* arr;
int length;
int numOfOperations;
float* mainarr;
int* validposition;


int isNumber(int pos) {
	int i;
	if (pos < 0 || pos > length)
		return -1;
	char a = arr[pos];
	if (a == '(' || a == '[' || a == '{' || a == '*' || a == '/' || a == '-' || a == '+') return -2;
	if (a == ',' || a == '.') return -3;
	for (i = 0; i < 10; i++)
		if (a - '0' == i)
			return 1;
	return 0;
}

int getEnd(int start) {   //function that skips the brackets and returns the idex of the necesary closing bracket
	int i = start + 1, temp = 1;
	while (temp > 0 && i < length) {
		i++;
		if (arr[i] == ')' || arr[i] == ']' || arr[i] == '}')
			temp--;
		else if (arr[i] == '(' || arr[i] == '[' || arr[i] == '{')
			temp++;
	}
	return i;
}

int setOrder(int start, int end, int numb) {   //a recursive function to set the order of operations, according to the algebraic precedence rules
	int i = start;
	int j = 0;
	int number = numb;        //number is the current index of operation, used for recursion
	for (i = start; i < end; i++)
		if (arr[i] == '(' || arr[i] == '[' || arr[i] == '{') {
			number = setOrder(i + 1, getEnd(i), number);
			i = getEnd(i);
		}

	for (i = start; i < end; i++) {
		if (arr[i] == '(' || arr[i] == '[' || arr[i] == '{') {
			i = getEnd(i); //skip brackets
		}
		if (arr[i] == '/' || arr[i] == '*') {  // check if operation is / or *
			operations[number].position = i;
			number++;
		}
	}

	for (i = start; i < end; i++) {
		if (arr[i] == '(' || arr[i] == '[' || arr[i] == '{') {
			i = getEnd(i); //skip brackets
		}
		if (arr[i] == '-' || arr[i] == '+') {  // check if operation is + or -
			int isOperation = 1;
			if (arr[i] == '-') {
				for (j = i - 1; j >= -1; j--) {
					if (isNumber(j) == 1) break;
					if (isNumber(j) == -2 || isNumber == -1) {
						//isOperation = 0;
						break;
					}
				}
			}
			if (isOperation == 1) {
				operations[number].position = i;
				number++;
			}
		}
	}
	return number;
}

float calculate(int index, int num) { //this function calculates the result of the operation
	int i, j = 0, trig = 0;
	float temp = 0, left = 0, right = 0, leftminus = 1, rightminus = 1;
	for (i = index; i >= 0; i--) //find the left side number
		if (validposition[i] == 1 && isNumber != -1) {
			operations[num].leftNumber = mainarr[i];
			operations[num].leftEdge = i;
			trig = 1;
			break;
		}
	if (trig == 0) {
		operations[num].leftEdge = 0;
		operations[num].leftNumber = 0;
	}
	trig = 0;
	for (i = index; i < length; i++) { //find the right side number
		if (validposition[i] == 1 && isNumber != -1) {
			operations[num].rightNumber = mainarr[i];
			operations[num].rightEdge = i;
			trig = 1;
			break;
		}
	}
	if (trig == 0) {
		operations[num].rightEdge = 0;
		operations[num].rightNumber = 0;
	}


	if (arr[index] == '+')
		temp = (operations[num].leftNumber + operations[num].rightNumber);
	if (arr[index] == '-')
		temp = (operations[num].leftNumber - operations[num].rightNumber);
	if (arr[index] == '*')
		temp = (operations[num].leftNumber * operations[num].rightNumber);
	if (arr[index] == '/') {
		if (operations[num].rightNumber == 0) {
			printf("Undefined");
			exit(0);
		}
		else
			temp = (operations[num].leftNumber / operations[num].rightNumber);
	}
	validposition[index] = 1;
	mainarr[index] = temp;

	validposition[operations[num].leftEdge] = 0;
	validposition[operations[num].rightEdge] = 0;

	return temp;
}

int checkIfNotOperation(int pos, int num) {
	int i,t;
	for (i = num; i < numOfOperations; i++){
		t = operations[i].position;
		if (pos == t) {
			return 0;
		}
	}
	return 1;
}

void setNumbers() {
	int i, j = 0, k = 0;
	int minus = 1;
	for (i = 0; i < length; i++) { //find the next number to set
		if (isNumber(i) != -1 && checkIfNotOperation(i, 0) == 1 && isNumber(i + 1) == 1) {
			if (arr[i] == '-')
				minus = -1; //check if number is negative
		}
		if (isNumber(i) == 1) {
			k = i;
			validposition[k] = 1;
			j = 0;
			while (isNumber(i) == 1) { // get the length of the number while tracking forward
				j++;
				i++;
			}
			int t;
			for (t = j; t > 0; t--) {
				int c = arr[t + i - j - 1] - '0';
				float pow = tenToPower(j - t); // set the right number while tracking backwards
				mainarr[k] += c * pow;
			}	
			if (isNumber(i) == -3) {
				i++;
				j = 0;
				while (isNumber(i) == 1) { //save the left number as an int in the struct
					int c = arr[i] - '0';
					mainarr[k] += c * tenToPower(-j - 1);
					j++;
					i++;
				}
			}
			mainarr[k] *= minus;
		}	
	}

}

int main() {
	int i,j, n;
	arr = inputText(&n);
	length = n;
	operations = (struct operation*)malloc(sizeof(struct operation) * length);
	validposition = (int*)malloc(sizeof(int) * length);
	mainarr = (float*)malloc(sizeof(float) * length);
	numOfOperations = setOrder(0, n, 0);   //set the order of operations from start to the end of the ecuation

	for (j = 0; j <= length; j++) {
		mainarr[j] = 0;
		validposition[j] = 0;
	}
	setNumbers();

	printarr(n,arr);
	
	if (debug) {
		for (j = 0; j <= length; j++) {
			printf("%d", validposition[j]);
		}
		printf("\n");
		for (i = 0; i < numOfOperations; i++)
			printf("pos%d->%d ", operations[i].position, i);
		printf("\n");
	}
	for (i = 0; i < numOfOperations; i++) {
		int temp = calculate(operations[i].position, i);
		if (debug) {
			printf("for operation %d result is %d \n", i, temp);
		}
		if (i == numOfOperations - 1) {
			printf("result %d", temp);
		}
	}
	
	free(validposition);
	free(mainarr);
	free(arr);
	free(operations);
	return 0;
}
