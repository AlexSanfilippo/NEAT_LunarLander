#Alexander Sanfilippo
#11 June, 2022
#NEAT Project

CC = mpicxx
CFLAGS = -std=c++11 -Wall -Wreturn-type -g
OBJS = testingGrounds.o 
EXECS = testingGrounds
all:${EXECS}

testingGrounds: testingGrounds.o
	${CC} ${CFLAGS} testingGrounds.o -o testingGrounds
testingGrounds.o: testingGrounds.cc gene.h Genome.h Species.h NOV.h
	${CC} ${CFLAGS} -c testingGrounds.cc

.PHONY: clean
clean:
	rm ${OBJS} ${EXECS} 


