#include "p1_threads.h"

#define AVG(a, b) (((a) + (b)) / 2)

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

typedef struct{
	int *data;
	int start;
	int end;
} merge_sort_thread_args;

void merge(int data[], int low, int mid, int high){
	int size_left  = mid - low + 1;
	int size_right = high - mid;

	int *left  = new int[size_left];
	int *right = new int[size_right];

	int i, j;
	for(i = 0; i < size_left; i++)
		left[i]  = data[low + i];
	for(j = 0; j < size_right; j++)
		right[i] = data[mid + j + 1];

	i = j = 0;
	int n = 0; // current position in original data array
	while(i < size_left && j < size_right){
		if(left[i] < right[j])
			data[n++] = left[i++];
		else
			data[n++] = right[j++];
	}
	while(i < size_left)
		data[n++] = left[i++];
	while(j < size_right)
		data[n++] = right[j++];
	
	delete[] left;
	delete[] right;
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
}