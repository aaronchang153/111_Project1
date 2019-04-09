#include "csv.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 128


std::vector<student_data> read_csv(std::string fname){
    std::vector<student_data> data;
    FILE *fp = NULL;
    fp = fopen(fname.c_str(), "r");
    if(fp != NULL){
        char buffer[BUFFER_SIZE];
        // the first line is just labels so it can be ignored
        fgets(buffer, BUFFER_SIZE, fp);

        student_data current;
        int id;
        float grade;

        while(fscanf(fp, "%d,%f", &id, &grade) != EOF){
            current.studentID = id;
            current.grade = grade;
            data.push_back(current);
        }
        fclose(fp);
    }
    return data;
}


#ifdef CSV_TEST

int main(){
    std::vector<student_data> data = read_csv("test.csv");
    for(int i = 0; i < data.size(); i++){
        printf("%d,%f\n", data[i].studentID, data[i].grade);
    }
    return 0;
}

#endif // CSV_TEST