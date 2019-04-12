#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>

typedef struct{
    std::string studentID;
    float grade;
} student_data;

std::vector<student_data> read_csv(std::string fname);

#endif // CSV_H