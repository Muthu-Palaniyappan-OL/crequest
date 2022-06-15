# Make file for crequest project

GXX=g++

all: crequest

crequest: main.cpp
	$(GXX) -g -pthread $^ -o $@

clean:
	rm *.o crequest