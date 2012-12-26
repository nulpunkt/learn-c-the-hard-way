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

int length_of_int_array (int *array) {
	return array[-1];
}

int bubble_sort(int *numbers, compare_ints cmp) {
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

	return length_of_int_array(numbers);
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

void print_sorter(compare_ints cmp) {
	unsigned char *data = (unsigned char *)cmp;
	int i = 0;
	for (i = 0; i < 25; i++) {
		printf("%02x:", data[i]);
	}
	puts("\n");
}

int main (int argc, char *argv[]) {
	int *numbers = random_int_array(10);
	
	print_int_array(numbers);

	printf("=====================\n");

	bubble_sort(numbers, int_natural_order);
	print_int_array(numbers);

	printf("=====================\n");

	bubble_sort(numbers, int_reverse_order);
	print_int_array(numbers);

	print_sorter(int_natural_order);

	return 0;
}
