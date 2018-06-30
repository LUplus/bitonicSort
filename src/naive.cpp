#include <iostream>
#include <cstdlib>

#define ASCEND_ORDER 1
#define DESCEND_ORDER 0
#define DATA_SIZE 10000

void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int n, int m);
void divide(float *arr, int len, bool order);
void bitonicSort(float *arr, int len, bool order);
void swap(float &a, float &b);
int greatest_powerOfTwo_lessThan(int a);

void printFloatArray(float *arr, int n)
{
	for (int i = 0; i < n; i++)
		printf("%.1f ", arr[i]);
	putchar('\n');
}

/*	Used in qsort(), 
	for testing
*/
int qcmp(const void *a, const void *b)
{
	return ceil(*(float *)a - *(float *)b);
}

int main(void)
{
	float data[DATA_SIZE] = { 0 };
	int seg_id[DATA_SIZE] = { 0 };
	int seg_start[2] = { 0, DATA_SIZE };
	int n = DATA_SIZE;
	int m = 1;

	float contrast[DATA_SIZE] = { 0 };

	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		data[i] = rand();
		contrast[i] = data[i];
	}

	std::qsort(contrast, DATA_SIZE, sizeof(contrast[0]), qcmp);

	// printFloatArray(data, n);
	segmentedBitonicSort(data, seg_id, seg_start, n, m);
	// printFloatArray(data, n);

	int counter = 0;
	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		if (data[i] != contrast[i])
		{
			printf("%.1f %.1f\n", data[i], contrast[i]);
			counter++;
		}
	}
	printf("Error: %d.\n", counter);

	system("PAUSE");
}

/*	Parameters:
		@data[n]: input float array
		@seg_id[n]: segment index of each float
		@seg_start[m+1]: seg_start[i] indicates the first element of i-th segment where 0 <= i <= m-1,
			and seg_start[m] = n
		@n: the size of data
		@m: the number of segments 
*/
void segmentedBitonicSort(float *data, int *seg_id, int *seg_start, int n, int m)
{
	// Each segment
	for (int k = 0; k < m; k++)
	{
		divide(data + seg_start[k], seg_start[k + 1] - seg_start[k], ASCEND_ORDER);
	}
}

/*	Divide and Conquer:
		Divide the array into two parts,
		recursively call divide() to sort two sub-arrays by different order,
		and finally call bitonicSort() to sort the array by @order
	Parameters:
		@arr: base address of current array
		@len: size of current array
		@order: ASC or DESC
*/
void divide(float *arr, int len, bool order)
{
	if (len > 1)
	{
		int mid = len >> 1;
		divide(arr, mid, !order);
		divide(arr + mid, len - mid, order);
		bitonicSort(arr, len, order);
	}
}

/*	It's easy to prove that after swap(),
	max(arr[i]~arr[mid]) <= min(arr[mid+1]~arr[len-1]).
	Recusively sort these two sub-array.
*/
void bitonicSort(float *arr, int len, bool order)
{
	if (len > 1)
	{
		int mid = greatest_powerOfTwo_lessThan(len);
		int cmpTimes = len - mid;

		for (int i = 0; i < cmpTimes; i++)
			if ((arr[i] > arr[mid + i]) == order)
				swap(arr[i], arr[mid + i]);

		bitonicSort(arr, mid, order);
		bitonicSort(arr + mid, len - mid, order);
	}
}

void swap(float &a, float &b)
{
	float c = a;
	a = b;
	b = c;
}

int greatest_powerOfTwo_lessThan(int a)
{
	int k = 1;
	// 2^n < a <= 2^(n+1)
	while (k < a)
		k = k << 1;
	// return 2^n
	return k >> 1;
}