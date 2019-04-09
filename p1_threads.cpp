#include "p1_threads.h"

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

		int mid = (args->start + args->end) / 2;

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

int merge_sort(int data[], int length, int num_threads);