CC=g++

all: main

main: gol.cpp
	$(CC) gol.cpp -o gol

clean:
	rm -f gol