#include "p1_process.h"
#include "csv.h"
#include <cmath>
#include <sys/wait.h>

#define NUM_FILES 5


static inline double square(double x) { return x * x; }

stats_t calculate_stats(std::vector<student_data> data){
	stats_t stats;
	stats.med = data[data.size() / 2].grade;

	double total = 0;
	for(std::vector<student_data>::iterator it = data.begin(); it != data.end(); it++){
		total = total + (*it).grade;
	}
	stats.avg = total / data.size();

	total = 0; // reuse total variable for standard deviation
	for(std::vector<student_data>::iterator it = data.begin(); it != data.end(); it++){
		total = total + square((*it).grade - stats.avg);
	}
	stats.std_dev = sqrt(total / data.size());
	return stats;
}


void get_statistics(std::string class_name[], int num_processes, int num_threads) {
	if(num_processes > NUM_FILES){
		num_processes = NUM_FILES;
	}
	
	int process_count = 0; // keeps track of the number of child processes
	pid_t fork_id;
	
	for(int i = 0; i < num_processes; i++){
		fork_id = fork();
		if(fork_id > 0){ // if this is the parent process
			printf("Child process is created. (pid: %d)\n", fork_id);
			process_count++;
		}
		else if(fork_id == 0){ // if this is the child process
			// at this point i is the index of the first file that this process needs to work on
			int files_completed = 0;
			int current_file_num;
			while((current_file_num = i + (num_processes * files_completed)) < NUM_FILES){
				std::vector<student_data> data = read_csv("input/" + class_name[current_file_num] + ".csv");
				merge_sort(&data, num_threads);
				write_csv("output/" + class_name[current_file_num] + "_sorted.csv", data);
				stats_t stats = calculate_stats(data);
				write_stats_csv("output/" + class_name[current_file_num] + "_stats.csv", stats);
				files_completed++;
			}
			exit(0);
		}
	}

	while(process_count > 0){
		fork_id = wait(NULL);
		printf("Child process is terminated. (pid: %d)\n", fork_id);
		process_count--;
	}
}
