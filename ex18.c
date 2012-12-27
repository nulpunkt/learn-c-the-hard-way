#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

void die(const char *msg) {
	if(errno) {
		perror(msg);
	} else {
		printf("ERROR: %s\n", msg);
	}
	exit(1);
}

typedef int (*compare_ints)(int a, int b);
typedef void (*sort_ints)(int *numbers, compare_ints cmp);

int length_of_int_array (int *array) {
	return array[-1];
}

void gsort(int *numbers, sort_ints sorter, compare_ints cmp) {
	sorter(numbers, cmp);
}

void bubble_sort(int *numbers, compare_ints cmp) {
	int temp = 0, i = 0, j = 0, n = length_of_int_array(numbers);

	for (i = 0; i < n-1; i++) {
		for (j = i+1; j < n; j++) {
			if (cmp(numbers[i], numbers[j]) > 0) {
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}
}

void print_int_array(int *numbers) {
	int i = 0, n = length_of_int_array(numbers);

	for (i = 0; i < n; i++) {
		printf("%d\n", numbers[i]);
	}
}

int int_reverse_order(int a, int b) {
	return b-a;
}

int int_natural_order(int a, int b) {
	return a-b;
}

int *random_int_array(int size) {
	int i = 0;
	int *numbers = malloc(size*sizeof(int)+1);

	numbers[0] = size;
	numbers++;

	for(i = 0; i < size; i++) {
		numbers[i] = rand();
	}

	return numbers;
}

int main (int argc, char *argv[]) {
	int *numbers = random_int_array(10);
	
	print_int_array(numbers);

	printf("=====================\n");

	gsort(numbers, bubble_sort, int_natural_order);
	print_int_array(numbers);

	printf("=====================\n");

	gsort(numbers, bubble_sort, int_reverse_order);
	print_int_array(numbers);

	return 0;
}
