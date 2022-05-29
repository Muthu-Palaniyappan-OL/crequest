GXX=clang++

all: crequest

crequest: main.cpp regex_matching.o
	$(GXX) $^ -o $@

regex_matching.o: include/regex_matching.cpp
	$(GXX) -c $^ -o $@

clean:
	rm *.o crequest