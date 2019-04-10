#include "p1_threads.h"

#define AVG(a, b) (((a) + (b)) / 2)

typedef struct{
	DATA_VECTOR *data;
	int start;
	int end;
} merge_sort_thread_args;

void merge(DATA_VECTOR &data, int low, int mid, int high){
	// split data into left and right sections
	// NOTE: mid is the index of the last element of the left array
	int size_left  = mid - low + 1;
	int size_right = high - mid;

	DATA_VECTOR left, right;

	int i, j;
	for(i = 0; i < size_left; i++)
		left.push_back(data[low + i]);
	// at this point, low+i is the index after the last element in left
	// i.e. the first element of right
	for(j = 0; j < size_right; j++)
		right.push_back(data[mid + j + 1]);

	// fill data with the minimum elements from left and right in order
	i = j = 0; // current position in left and right respectively
	int n = low; // current position in original data array
	while(i < size_left && j < size_right){
		if(left[i].grade < right[j].grade)
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
}

// merges each of the sorted segments of the original data array
void merge_threads(DATA_VECTOR &data, merge_sort_thread_args *args, int start, int end){
	// start and end refer to segments of the original array that were sorted by each thread
	if((end - start) == 1){ // base case: merge two segments together
		merge(data, args[start].start, args[start].end, args[end].end);
	}
	else if((end - start) == 2){ // almost base case: merge three segments together
		merge_threads(data, args, start, start + 1);
		merge(data, args[start].start, args[start+1].end, args[end].end);
	}
	else{ // recursive case
		int mid = AVG(start, end);
		merge_threads(data, args, start, mid);
		merge_threads(data, args, mid+1, end);
		merge(data, args[start].start, args[mid].end, args[end].end);
	}
}

// recursive merge sort run by each thread
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
		merge(*args->data, args->start, mid, args->end);
	}

	return NULL;
}

int merge_sort(DATA_VECTOR *data, int max_threads){
	if(max_threads < 1){
		return -1;
	}

	int length = data->size();
	int num_threads = (max_threads > length) ? length : max_threads;

	pthread_t threads[num_threads];
	int sub_array_size = length / num_threads;

	// divide the array into "num_threads" number of segments and 
	// compute the start and end points of each segment
	int i;
	merge_sort_thread_args args[num_threads];
	for(i = 0; i < num_threads; i++)
		args[i].data = data;
	args[0].start = 0;
	args[0].end = sub_array_size - 1;
	if(num_threads > 1){
		// this only needs to be done if there's more than one thread
		for(i = 1; i < num_threads - 1; i++){
			args[i].start = args[i-1].end + 1;
			args[i].end   = args[i].start + sub_array_size - 1;
		}
		args[num_threads-1].start = args[num_threads-2].end + 1;
		args[num_threads-1].end = length - 1;
	}

	// create threads to sort each segment
	for(i = 0; i < num_threads; i++){
		pthread_create(&threads[i], NULL, merge_sort_thread, (void *) &args[i]);
	}
	for(i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}

	// merge the sorted segments of the array
	if(num_threads > 1){
		// don't need to merge the results from each thread if there's only one thread
		merge_threads(*data, args, 0, num_threads - 1);
	}

	return 0;
}

#ifdef MERGE_SORT_TEST

#include <time.h>

#define SIZE 100
#define MAX_NUM 1000
#define NUM_THREADS 4

int check_sorted_vector(DATA_VECTOR v){
	for(int i = 1; i < v.size(); i++){
		if(v[i].grade < v[i-1].grade){
			return 0;
		}
	}
	return 1;
}

void print_vector(DATA_VECTOR v){
	printf(">>> START\n");
	for(int i = 0; i < v.size(); i++){
		printf("%10d\t%10f\n", v[i].studentID, v[i].grade);
	}
	printf("<<< END\n\n");
}

int main(){
	srand((unsigned) time(NULL));
	DATA_VECTOR data;
	student_data entry;
	for(int i = 0; i < SIZE; i++){
		entry.studentID = i;
		entry.grade = (float) (rand() % MAX_NUM);
		data.push_back(entry);
	}
	print_vector(data);
	merge_sort(&data, NUM_THREADS);
	print_vector(data);

	if(check_sorted_vector(data))
		printf("SORTED\n");
	else
		printf("NOT SORTED\n");

	return 0;
}

#endif // MERGE_SORT_TEST