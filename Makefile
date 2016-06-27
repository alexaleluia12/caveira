FLAGS=-std=c++11 -Wall -g

out: comum.o main.o comum.h
	g++ -o out main.o comum.o $(FLAGS) -lGL -lGLU -lglut -lm

main.o: comum.h comum.o caveira.cpp
	g++ -c -o main.o caveira.cpp $(FLAGS)

comum.o: comum.h comum.cpp
	g++ -c comum.cpp $(FLAGS)

clear:
	rm -f comum.o out main.o
