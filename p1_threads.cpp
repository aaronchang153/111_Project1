#include "p1_threads.h"

#define AVG(a, b) (((a) + (b)) / 2)

typedef struct{
	int *data;
	int start;
	int end;
} merge_sort_thread_args;

void merge(int data[], int low, int mid, int high){
	// split data into left and right sections
	// NOTE: mid is the last element of the left array
	int size_left  = mid - low + 1;
	int size_right = high - mid;

	int *left  = new int[size_left];
	int *right = new int[size_right];

	int i, j;
	for(i = 0; i < size_left; i++)
		left[i]  = data[low + i];
	// at this point, i is the index after the last element in left
	// i.e. the first element of right
	for(j = i; j <= high; j++)
		right[j-i] = data[j];

	// fill data with the minimum elements from left and right in order
	i = j = 0;
	int n = 0; // current position in original data array
	while(i < size_left && j < size_right){
		if(left[i] < right[j])
			data[n++] = left[i++];
		else
			data[n++] = right[j++];
	}
	// include the rest of the left array
	while(i < size_left)
		data[n++] = left[i++];
	// include the rest of the right array
	while(j < size_right)
		data[n++] = right[j++];
	
	delete[] left;
	delete[] right;
}

void merge_threads(int data[], merge_sort_thread_args *args, int start, int end){
	if((end - start) > 1){
		int mid = AVG(start, end);
		merge_threads(data, args, start, mid);
		merge_threads(data, args, mid+1, end);
	}
	merge(data, args[start].start, AVG(args[start].start, args[end].end), args[end].end);
}

void *merge_sort_thread(void *argp){
	// might need to go back and trace which array the midpoint is included in
	merge_sort_thread_args *args = (merge_sort_thread_args *) argp;

	if(args->start < args->end){
		merge_sort_thread_args arg1, arg2;

		int mid = AVG(args->start, args->end);

		arg1.data = arg2.data = args->data;

		arg1.start = args->start;
		arg1.end   = mid;
		arg2.start = mid + 1;
		arg2.end   = args->end;

		merge_sort_thread((void *) &arg1);
		merge_sort_thread((void *) &arg2);
		merge(args->data, args->start, mid, args->end);
	}

	return NULL;
}

int merge_sort(int data[], int length, int max_threads){
	int num_threads = (max_threads > length) ? length : max_threads;

	pthread_t threads[num_threads];
	int sub_array_size = length / max_threads;

	// divide the array into "num_threads" number of segments and 
	// compute the start and end points of each segment
	int i;
	merge_sort_thread_args args[num_threads];
	for(i = 0; i < num_threads; i++)
		args[i].data = data;
	args[0].start = 0;
	args[0].end = sub_array_size - 1;
	for(i = 1; i < num_threads - 1; i++){
		args[i].start = args[i-1].end + 1;
		args[i].end   = args[i].start + sub_array_size - 1;
	}
	args[num_threads-1].start = args[num_threads-2].end + 1;
	args[num_threads-1].end = length - 1;

	// create threads to sort each segment
	for(i = 0; i < num_threads; i++){
		pthread_create(&threads[i], NULL, merge_sort_thread, (void *) &args[i]);
	}
	for(i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}

	// merge the sorted segments of the array
	merge_threads(data, args, 0, num_threads - 1);

	return 0;
}

#ifdef MERGE_SORT_TEST

void print_arr(int *arr, int n){
	printf("{");
	for(int i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("}\n");
}

int main(){
	int arr[10] = {6, 34, 2, 10, 11, 42, 23, 81, 34, 67};

	print_arr(arr, 10);

	merge_sort(arr, 10, 2);

	print_arr(arr, 10);

	return 0;
}

#endif // MERGE_SORT_TEST