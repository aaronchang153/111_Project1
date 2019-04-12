#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>

typedef struct{
    std::string studentID;
    double grade;
} student_data;

typedef struct{
	double avg;
	double med;
	double std_dev;
} stats_t;

std::vector<student_data> read_csv(std::string fname);

int write_csv(std::string fname, std::vector<student_data> data);

int write_stats_csv(std::string fname, stats_t stats);

#endif // CSV_H