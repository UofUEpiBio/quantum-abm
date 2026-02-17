main.o: main.cpp abm.hpp abm-bones.hpp abm-meat.hpp
	g++ -std=c++17 -pedantic -Wall -O2 -g main.cpp -o main.o
