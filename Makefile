

out: comum.o main.o comum.h
	g++ -o out main.o comum.o -Wall -g -lGL -lGLU -lglut -lm

main.o: comum.h comum.o caveira.cpp
	g++ -c -o main.o caveira.cpp -Wall -g

comum.o: comum.h comum.cpp
	g++ -c comum.cpp -Wall -g

clear:
	rm -f comum.o out main.o
