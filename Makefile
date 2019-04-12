EXEC=p1_exec
MS_TEST=ms_test
CSV_TEST=csv_test

CC=g++
CFLAGS=-std=c++98 -I.


%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

${EXEC}: main.o p1_process.o p1_threads.o csv.o
	g++ -o ${EXEC} main.o p1_process.o p1_threads.o -I. -lpthread 


${MS_TEST}: CFLAGS+=-DMERGE_SORT_TEST -g
${MS_TEST}: p1_threads.o csv.o
	g++ -o ${MS_TEST} p1_threads.o csv.o -I. -lpthread 

${CSV_TEST}: CFLAGS+=-DCSV_TEST -g
${CSV_TEST}: csv.o
	g++ -o ${CSV_TEST} csv.o -I.

clean:
	rm -rf ./${EXEC}
	rm -rf ./${MS_TEST}
	rm -rf ./${CSV_TEST}
	rm -rf ./*.o
