#include "p1_process.h"
#include "csv.h"
#include <sys/wait.h>

#define NUM_FILES 5


void get_statistics(std::string class_name[], int num_processes, int num_threads) {
	if(num_processes > NUM_FILES){
		num_processes = NUM_FILES;
	}
	
	int process_count = 0; // keeps track of the number of child processes
	pid_t fork_id;
	
	for(int i = 0; i < NUM_FILES; i++){
		if(process_count < num_processes){
			fork_id = fork();
			if(fork_id > 0){ // if this is the parent process
				printf("Cild process is created. (pid: %d)\n", fork_id);
				process_count++;
			}
			else if(fork_id == 0){ // if this is the child process
				printf("%d:\tWorking on file: %s", fork_id, class_name[i].c_str());
				std::vector<student_data> data = read_csv(class_name[i]);
				merge_sort(&data, num_threads);
				// output sorted data
				// calculate and output the data
			}
		}
		else{
			fork_id = wait(NULL);
			printf("Child process is terminated. (pid: %d)\n", fork_id);
			process_count--;
		}
	}

	while(process_count > 0){
		fork_id = wait(NULL);
		printf("Child process is terminated. (pid: %d)\n", fork_id);
		process_count--;
	}
}
