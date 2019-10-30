#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "operations.h"
#define maxOperations 255

// float version 

struct operation {
	int  leftEdge, rightEdge, position, leftminus, rightminus;
	float leftNumber, rightNumber;
};

struct operation* operations;
char* arr;
int* results;
int length;
int numOfOperations;


int isNumber(int pos) {
	int i;
	if (pos < 0 || pos > length)
		return -1;
	char a = arr[pos];
	if (a == '(' || a == '*' || a == '/' || a == '-' || a == '+') return -2;
	for (i = 0; i < 10; i++)
		if (a - '0' == i)
			return 1;
	return 0;
}

int getEnd(int start) {   //function that skips the brackets and returns the idex of the necesary closing bracket
	int i = start + 1, temp = 1;
	while (temp > 0 && i < length) {
		i++;
		if (arr[i] == ')')
			temp--;
		else if (arr[i] == '(')
			temp++;
	}
	return i;
}

int setOrder(int start, int end, int numb) {   //a recursive function to set the order of operations, according to the algebraic precedence rules
	int i = start;
	int j = 0;
	int number = numb;        //number is the current index of operation, used for recursion
	for (i = start; i < end; i++)
		if (arr[i] == '(') {
			number = setOrder(i + 1, getEnd(i), number);
			i = getEnd(i);
		}

	for (i = start; i < end; i++) {
		if (arr[i] == '(') {
			i = getEnd(i); //skip brackets
		}
		if (arr[i] == '/' || arr[i] == '*') {  // check if operation is / or *
			operations[number].position = i;
			number++;
		}
	}

	for (i = start; i < end; i++) {
		if (arr[i] == '(') {
			i = getEnd(i); //skip brackets
		}
		if (arr[i] == '-' || arr[i] == '+') {  // check if operation is + or -
			int isOperation = 1;
			if (arr[i] == '-') {
				for (j = i-1; j > 0; j--) {
					if (isNumber(j) == 1) break;
					if (isNumber(j) == -2 || isNumber == -1) {
						isOperation = 0;
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

int calculate(int index, int num) { //this function calculates the result of the operation
	int i, j = 0, trig = 0;
	float temp = 0, left = 0, right = 0, leftminus = 1, rightminus = 1;
	for (i = index; i >= 0; i--) //find the left side number
		if (isNumber(i) == 1) break;
		else if (isNumber(i) == -1) {
			operations[num].leftNumber = 0;
			break;
		}
	j = 0;
	while (isNumber(i) == 1) { //save the left number as an int in the struct
		int c = arr[i] - '0';
		left += c * tenToPower(j);
		j++;
		i--;
	}
	if (isNumber(i) != -1) {
		if (arr[i] == '-' && checkIfNotOperation(i,num) == 1) //check if left number is negative
			leftminus = -1;
	}

	operations[num].leftEdge = i;
	operations[num].leftNumber = left*leftminus;
	operations[num].leftminus = leftminus;

	for (i = index; i < length; i++) { //find the right side number
		if (isNumber(i) != -1 && checkIfNotOperation(i,num) == 1 && isNumber(i+1) == 1) {
			if (arr[i] == '-' )
				rightminus = -1; //check if right number is negative
		}
		if (isNumber(i) == 1) break;
		else if (isNumber(i) == -1) { // if length has been exceded then set right to 0
			operations[num].rightNumber = 0;
		}
	}
	j = 0;
	while (isNumber(i) == 1) { // get the length of the number while tracking forward
		j++;
		i++;
	} 
	int t = i;
	for (i = j; i > 0; i--){
		int c = arr[t+i-j-1] - '0';
		int pow = tenToPower(j - i); // set the right number while tracking backwards
			right += c * pow;
	}

	operations[num].rightEdge = t - 1;
	operations[num].rightNumber = right * rightminus;
	operations[num].rightminus = rightminus;

	if (arr[index] == '+')
		temp = add(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '-')
		temp = sub(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '*')
		temp = mult(operations[num].leftNumber, operations[num].rightNumber);
	if (arr[index] == '/')
		temp = divide(operations[num].leftNumber, operations[num].rightNumber);

	j = 0;
	int sum = temp;
	for (i = operations[num].rightEdge; i > operations[num].leftEdge; i--) {
		if (temp < 0)
			arr[i] = '-';
		else arr[i] = ' ';
	}
	if (sum == 0) {
		arr[operations[num].rightEdge] = '0';
	}
	else
	for (i = operations[num].rightEdge; i > operations[num].leftEdge; i--) {
		arr[i] = abs(sum) % 10 + '0';
		sum /= 10;
		if (sum == 0)
			break;
	}

	return (int)temp;
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
	results = (int*)malloc(sizeof(int) * n);
	numOfOperations = setOrder(0, n, 0);   //set the order of operations from start to the end of the ecuation
	
	printarr(n,arr);
	for (i = 0; i < numOfOperations; i++)
		printf(" %d->%d ", operations[i].position, i);
	printf("\n");
	for (i = 0; i < numOfOperations; i++) {
		int temp = calculate(operations[i].position, i);

		printf("for operaition %d result is %d \n",i,temp);
		printarr(n,arr);
		if (i == numOfOperations - 1) {
			printf("result %d", temp);
		}
	}
	
	free(arr);
	free(operations);
	free(results);
	return 0;
}
