#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17 */
void die(const char *message)
{
	if (errno)
		perror(message);
	else
		printf("ERROR: %s\n", message);

	exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

typedef int *(*sort_method)(int *, int count, compare_cb);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int i = 0;
	int j = 0;
	int *target = malloc(count * sizeof(int));

	if (!target)
		die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	for (i = 0; i < count; i++) {
		for (j = 0; j < count - 1; j++) {
			if (cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}

	return target;
}

int *bubble_sort2(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int i = 0;
	int j = 0;
	int *target = malloc(count * sizeof(int));

	if(!target) die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	for(i = count - 1; i ; i--) {
		for(j = 0; j < i; j++) {
			if(cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}

	return target;
}


// we could do this inline, but it's easier to read this way
int *merge_sorted(int * numbers1, int count1, int *numbers2, int count2, compare_cb cmp)
{
	int * rvalue;
	int * tmp;

	rvalue = malloc((count1 + count2) * sizeof(int));
	if (!rvalue) die("Memory error");
	for(tmp = rvalue; count1 && count2; tmp++) {
		if (cmp(*numbers1, *numbers2) > 0) {
			*tmp = *(numbers2++);
			if (! --count2) {
				memcpy(tmp + 1, numbers1, count1 * sizeof(int));
			}
		} else {
			*tmp = *(numbers1++);
			if (! --count1) {
				memcpy(tmp + 1, numbers2, count2 * sizeof(int));
			}
		}
	}
	return rvalue;
}

/* merge sort :: classing recursive algorythm */
int *merge_sort(int *numbers, int count, compare_cb cmp)
{
	int *rvalue;
	if (count == 1) {
		// end of recursion
		rvalue = malloc(sizeof(int));
		if (!rvalue) die("Memory error.");
		*rvalue = *numbers;
	} else {
		int i = count / 2;
		int j = i + count % 2;
		int * left = merge_sort(numbers, i, cmp);
		int * right = merge_sort(numbers + i, j, cmp);
		// this delays the allocation for rvalue until the parts have been sorted
		rvalue = merge_sorted(left, i, right, j, cmp);
		free(left);
		free(right);
	}
	return rvalue;
}

int *quick_sort_merge(int * numbers1, int count1, int *numbers2, int count2, int pivot)
{
	int *rvalue = malloc((count1 + count2 + 1) * sizeof(int));
	if (!rvalue)
		die("Memory error.");
	memcpy(rvalue, numbers1, count1 * sizeof(int));
	rvalue[count1] = pivot;
	memcpy(rvalue + count1 + 1, numbers2, count2 * sizeof(int));
	return rvalue;
}

int *quick_sort(int *numbers, int count, compare_cb cmp)
{
	int *rvalue;
	int pivot = numbers[--count];
	if (! count) {
		// end of recursion
		rvalue = malloc(sizeof(int));
		if (!rvalue)
			die("Memory error.");
		*rvalue = pivot;
	} else {
		int lc = 0;
		int rc = 0;
		int *left = malloc(count * sizeof(int));
		int *right = malloc(count * sizeof(int));
		while(count--) {
			if (cmp(pivot,*numbers) > 0) {
				left[lc++] = *numbers;
			} else {
				right[rc++] = *numbers;
			}
			numbers++;
		}
		if (lc) {
			int * tmp = quick_sort(left, lc, cmp);
			free(left);
			left = tmp;
		}
		if (rc) {
			int * tmp = quick_sort(right, rc, cmp);
			free(right);
			right = tmp;
		}
		rvalue = quick_sort_merge(left, lc, right, rc, pivot);
		free(left);
		free(right);
	}
	return rvalue;
}

int sorted_order(int a, int b)
{
	return a - b;
}

int reverse_order(int a, int b)
{
	return b - a;
}

int strange_order(int a, int b)
{
	if (a == 0 || b == 0)
		return 0;
	else
		return a % b;
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out
 */
void test_sorting(int *numbers, int count, compare_cb cmp, sort_method sort)
{
	int i = 0;
	int *sorted = sort(numbers, count, cmp);

	if (!sorted)
		die("Failed to sort as requested.");

	if (count <= 24) {
		for (i = 0; i < count; i++) {
			printf("%d ", sorted[i]);
		}
	} else {
		for (i = 0; i < 12; i++) {
			printf("%d ", sorted[i]);
		}
		printf("... ");
		do {
			printf("%d ", sorted[count - i]);
		} while(--i);
	}
	printf("\n");

	free(sorted);
}

inline void
shuffle(int count, int *numbers)
{
	while(count) {
		int i = rand() % count--;
		if (i != count) {
			int tmp = numbers[i];
			numbers[i] = numbers[count];
			numbers[count] = tmp;
		}
	}
}

static sort_method sort_test[] = {
	bubble_sort,
	bubble_sort2,
	merge_sort,
	quick_sort,
};

#define num_methods (sizeof(sort_test) / sizeof(*sort_test))

int main(int argc, char *argv[])
{
	struct timeval times[num_methods + 1];
	if(argc != 2)
		die("USAGE: ex18 20");

	int count = atoi(argv[1]);
	int i = 0;

	int *numbers = malloc(count * sizeof(int));
	
	if (!numbers)
		die("Memory error.");

	for (i = 0; i < count; i++) {
		numbers[i] = i;
	}
	shuffle(count, numbers);

	printf("Results:");
	for(i = 0; i < num_methods; i++) {
		gettimeofday(times + i, NULL);
		printf("\nTest %d\n", i + 1);
		test_sorting(numbers, count, sorted_order, sort_test[i]);
		test_sorting(numbers, count, reverse_order, sort_test[i]);
		test_sorting(numbers, count, strange_order, sort_test[i]);
	}
	gettimeofday(times + num_methods, NULL);

	free(numbers);

	printf("\nTimes:\n");
	for(i = 1; i <= num_methods; i++) {
		struct timeval diff;
		timersub(times + i, times + i - 1, &diff);
		printf("Test %d: %ld.%09ld seconds\n", i, diff.tv_sec, diff.tv_usec);
	}

	return 0;
}
