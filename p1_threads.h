#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include "p1_process.h"
#include "csv.h"

typedef std::vector<student_data> DATA_VECTOR;

int merge_sort(DATA_VECTOR *data, int num_threads);

#endif
