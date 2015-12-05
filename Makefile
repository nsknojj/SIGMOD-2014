# Compiler flags
CC  = gcc
CXX = g++
CFLAGS=$(USER_FLAGS) -O3 -fPIC -g -I. -I./include
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lpthread

# The name of the excution that will be built
all: query1_2 query3 query4 run

clean:
	rm -f *~ *.o *.out query1_2 query3 query4 run
