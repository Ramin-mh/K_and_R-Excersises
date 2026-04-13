#include <stdio.h>
#include <stdlib.h>

void quick_sort(int v[], int left, int right);
void swap(int v[], int i, int j);
void print_array(int v[], int len);

int main(void){
	int nums[] = {10, 2, 5, 8, 1};
	int len = sizeof(nums) / sizeof(int);

	printf("%d\n\n", len);

	printf("Before sorting: ");
	print_array(nums, len);

	quick_sort(nums, 0, 4);

	printf("After sorting: ");
	print_array(nums, len);

	return EXIT_SUCCESS;
}

/* qsort: sort v[left]...v[right] into increasing order */
void quick_sort(int v[], int left, int right){
	int i, last;

	if (left >= right){
		return;
	}
	swap(v, left, (left + right) / 2);
	last = left;

	for (i = left + 1; i <= right; i++){
		if (v[i] < v[left]){
			swap(v, ++last, i);
		}
	}
	swap(v, left, last);
	quick_sort(v, left, last-1);
	quick_sort(v, last+1, right);
}

/* Interchage v[i] and v[j] */
void swap(int v[], int i, int j){
	int temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

void print_array(int v[], int len){
	int i;
	for (i = 0; i < len; i++){
		printf("%d ", v[i]);
	}
	putchar('\n');
}
