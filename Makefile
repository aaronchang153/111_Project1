EXEC=p1_exec
MS_TEST=ms_test

CC=g++
CFLAGS=-std=c++98 -I.

${MS_TEST}: CFLAGS+=-DMERGE_SORT_TEST -g


%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

${EXEC}: main.o p1_process.o p1_threads.o
	g++ -o ${EXEC} main.o p1_process.o p1_threads.o -I. -lpthread 

${MS_TEST}: p1_threads.o
	g++ -o ${MS_TEST} p1_threads.o -I. -lpthread 

clean:
	rm -rf ./${EXEC}
	rm -rf ./${MS_TEST}
	rm -rf ./*.o
