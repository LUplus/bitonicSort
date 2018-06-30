#include <iostream>
#include <cstdlib>

#define ASCEND_ORDER 1
#define DESCEND_ORDER 0
#define DATA_MIN_SIZE 30000
#define DATA_SIZE 40000
#define DATA_SIZE_INV 100

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

int qcmp(const void *a, const void *b)
{
	if (isnan(*(float *)a))
	{
		if (isnan(*(float *)b))
			return 0;
		return 1;
	}
	if (isnan(*(float *)b))
		return -1;
	return ceil(*(float *)a - *(float *)b);
}

int main(void)
{
	float data[DATA_SIZE] = { 0 };
	int seg_id[DATA_SIZE] = { 0 };			// It's wrong but never used
	int seg_start[3] = { 0, DATA_SIZE / 2, DATA_SIZE };
	int n = DATA_SIZE;
	int m = 2;

	float contrast[DATA_SIZE] = { 0 };

	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		data[i] = rand();
		contrast[i] = data[i];
	}
	for (size_t i = 0; i < DATA_SIZE; i=i+10)
	{
		data[i] = NAN;
		contrast[i] = NAN;
	}

	/* Time Test */
	for (int t = DATA_MIN_SIZE; t < DATA_SIZE; t += DATA_SIZE_INV)
	{
		seg_start[1] = t / 2;
		seg_start[2] = t;

		clock_t sum = 0;
		for (int i = 0; i < 10; i++)
		{
			clock_t begin = clock();
			segmentedBitonicSort(data, seg_id, seg_start, n, m);
			clock_t finish = clock();
			sum += finish - begin;
		}

		std::cout << sum / 10 << std::endl;

		for (int i = 0; i < t; i++)
			data[i] = contrast[i];
	}

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
		int left_min = 0, right_max = len - 1;

		// 类似归并排序，步长逐渐减半
		for (int step = greatest_powerOfTwo_lessThan(len); step >= 1; step >>= 1)
		{
			int c_left_min = 0, c_right_min = 0;
			for (c_left_min = left_min; c_left_min < right_max; c_left_min += 2 * step)
			{
				c_right_min = c_left_min + step;

				// The rightmost interval has less times of comparison
				int cmpTimes = right_max - c_right_min + 1;
				cmpTimes = (step < cmpTimes) ? step : cmpTimes;

				for (int i = 0; i < cmpTimes; i++)
				{
					// Cope with NaN
					// Robust but not stable for NaN, which treated as a fixed number (INF here).
					bool left = isnan(arr[c_left_min + i]), right = isnan(arr[c_right_min + i]);
					if (left && right) {
						continue;
					}
					else if (left) {
						if ((INFINITY > arr[c_right_min + i]) == order)
							swap(arr[c_left_min + i], arr[c_right_min + i]);
					}
					else if (right) {
						if ((arr[c_left_min + i] > INFINITY) == order)
							swap(arr[c_left_min + i], arr[c_right_min + i]);
					}
					else {
						if ((arr[c_left_min + i] > arr[c_right_min + i]) == order)
							swap(arr[c_left_min + i], arr[c_right_min + i]);
					}
				}
			}
		}
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
