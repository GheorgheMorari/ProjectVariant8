#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "operations.h"
#define maxOperations 255
#define debug 1

// float version 

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
		if (arr[i] == ')'|| arr[i] == ']'|| arr[i] == '}')
			temp--;
		else if (arr[i] == '('|| arr[i] == '[' || arr[i] == '{')
			temp++;
	}
	return i;
}

int setOrder(int start, int end, int numb) {   //a recursive function to set the order of operations, according to the algebraic precedence rules
	int i = start;
	int j = 0;
	int number = numb;        //number is the current index of operation, used for recursion
	for (i = start; i < end; i++)
		if (arr[i] == '('|| arr[i] == '['|| arr[i] == '{') {
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
				for (j = i-1; j >= -1; j--) {
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
		if (isNumber(i) == 1 || validposition[i] == 1) { 
			if (validposition[i] == 1) {
				trig = 1;
			}
			break; 
		}
		else if (isNumber(i) == -1 || arr[i] == '(' || arr[i] == '[' || arr[i] == '{') {
			operations[num].leftNumber = 0;
			break;
		}
	j = 0;
	temp = i;
	if (trig != 1) {
		while (isNumber(temp) == 1) { //save the left number as an int in the struct
			j++;
			temp--;
		}
		if (isNumber(temp) == -3) {  // if there is a dot
			int m = i - temp;
			for (temp = i - j + 1; temp <= i; temp++) {
				int c = arr[(int)temp] - '0';
				left += c * tenToPower(i - temp - m);
			}
			i -= m + 1;
			j = 0;
			while (isNumber(i) == 1) { //save the left number as an int in the struct
				int c = arr[i] - '0';
				left += c * tenToPower(j);
				j++;
				i--;
			}
		}
		else {
			j = 0;
			while (isNumber(i) == 1) { //save the left number as an int in the struct
				int c = arr[i] - '0';
				left += c * tenToPower(j);
				j++;
				i--;
			}
		}

		if (isNumber(i) != -1) {
			if (arr[i] == '-' && checkIfNotOperation(i, num) == 1) //check if left number is negative
				leftminus = -1;
		}
	}
	else {
		left = mainarr[i];
	}

	operations[num].leftEdge = i-1;
	operations[num].leftNumber = left * leftminus;
	operations[num].leftminus = leftminus;

	trig = 0;
	for (i = index; i < length; i++) { //find the right side number
		if (isNumber(i) != -1 && checkIfNotOperation(i, num) == 1 && isNumber(i + 1) == 1) {
			if (arr[i] == '-')
				rightminus = -1; //check if right number is negative
		}
		if (isNumber(i) == 1 || validposition[i] == 1) {
			if(validposition[i] == 1)
				trig = 1;
			break;
		}
		else if (isNumber(i) == -1) { // if length has been exceded then set right to 0
			operations[num].rightNumber = 0;
		}
	}
	j = 0;
	if (trig != 1) {
		while (isNumber(i) == 1) { // get the length of the number while tracking forward
			j++;
			i++;
		}
		int t = i;
		for (i = j; i > 0; i--) {
			int c = arr[t + i - j - 1] - '0';
			int pow = tenToPower(j - i); // set the right number while tracking backwards
			right += c * pow;
		}
		i = t;
		if (isNumber(i) == -3) {
			i++;
			j = 0;
			while (isNumber(i) == 1) { //save the left number as an int in the struct
				int c = arr[i] - '0';
				right += c * tenToPower(-j - 1);
				j++;
				i++;
			}
		}
	}
	else {
		right = mainarr[i];
		rightminus = abs(rightminus);
	}
	operations[num].rightEdge = i;
	operations[num].rightNumber = right * rightminus;
	operations[num].rightminus = rightminus;

	if (arr[index] == '+')
		temp = add(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '-')
		temp = sub(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '*')
		temp = mult(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '/') {
		if (operations[num].rightNumber == 0) {
			printf("Undifined");
			exit(0);
		} else 
			temp = divide(operations[num].leftNumber, operations[num].rightNumber);
	}
	j = 0;
	int sum = temp;
	for (i = operations[num].rightEdge; i > operations[num].leftEdge; i--) {
		/*if (temp < 0)
			arr[i] = '-';
		else */
		arr[i] = ' ';
		validposition[i] = 0;
		mainarr[i] = 0;
	}	
		validposition[operations[num].position] = 1;
		mainarr[operations[num].position] = temp;
		if (debug) {
			printf("left %f, right %f \n", left, right);
		}

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


int main() {
	int i,j, n;
	arr = inputText(&n);
	length = n;
	operations = (struct operation*)malloc(sizeof(struct operation) * maxOperations);
	validposition = (int*)malloc(sizeof(int) * length);
	mainarr = (float*)malloc(sizeof(float) * length);

	for (j = 0; j <= n; j++)
		validposition[j] = 0;

	numOfOperations = setOrder(0, n, 0);   //set the order of operations from start to the end of the ecuation
	
	printarr(n,arr);
	if (debug) {
		for (i = 0; i < numOfOperations; i++)
			printf(" %d->%d ", operations[i].position, i);
		printf("\n");
	}
	for (i = 0; i < numOfOperations; i++) {
		float temp = calculate(operations[i].position, i);
		if (debug) {
			for (j = 0; j <= n; j++)
				printf("%d", validposition[j]);
			printf("for operaition %d result is %f \n", i, temp);
			printarr(n, arr);
		}
		if (i == numOfOperations - 1) {
			printf("result %f", temp);
		}
	}
	
	free(arr);
	free(operations);
	free(mainarr);
	free(validposition);
	return 0;
}
