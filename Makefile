GXX=g++

all: crequest

crequest: util.o main.cpp
	$(GXX) -g -pthread $^ -o $@

util.o: include/util.cpp
	$(GXX) -g -c $^ -o $@

clean:
	rm *.o crequest