#include "csv.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ID_LENGTH 15
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
        char *tok;
        char id[ID_LENGTH];
        double grade;

        while(fgets(buffer, BUFFER_SIZE, fp) != NULL){ // read line-by-line until EOF
            tok = strtok(buffer, ",");
            if(tok != NULL){
                std::string tmp(tok);
                current.studentID = tmp;
                tok = strtok(NULL, ",");
                if(tok != NULL){
                    grade = atof(tok);
                    current.grade = grade;
                    data.push_back(current);
                }
            }
        }
        fclose(fp);
    }
    return data;
}

int write_csv(std::string fname, std::vector<student_data> data){
    FILE *fp = NULL;
    fp = fopen(fname.c_str(), "w");
    if(fp == NULL){
        return -1;
    }
    
    fprintf(fp, "Rank,Student ID,Grade\n");
    int rank = 1;
    for(int i = data.size() - 1; i >= 0; i--){
        fprintf(fp, "%d,%s,%.10lf\n", rank++, data[i].studentID.c_str(), data[i].grade);
    }
    fclose(fp);
    return 0;
}

int write_stats_csv(std::string fname, stats_t stats){
    FILE *fp = NULL;
    fp = fopen(fname.c_str(), "w");
    if(fp == NULL){
        return -1;
    }
    
    fprintf(fp, "Average,Median,Std. Dev\n");
    fprintf(fp, "%lf,%lf,%lf\n", stats.avg, stats.med, stats.std_dev);
    fclose(fp);
    return 0;
}


#ifdef CSV_TEST

#include <iostream>

int main(){
    std::vector<student_data> data = read_csv("input/algorithm.csv");
    //for(int i = 0; i < data.size(); i++){
    //    std::cout << data[i].studentID << "," << data[i].grade << std::endl;
    //}
    write_csv("test_out.csv", data);
    return 0;
}

#endif // CSV_TEST