CC=g++
CFLAGS= -O2 -pipe -fomit-frame-pointer
# WARN= -W -Wall
# LIBS:= ncurses
LIBS:= ncursesw

main: gol.cpp
	$(CC) ${CFLAGS} gol.cpp -o gol -l${LIBS}

clean:
	rm -f gol