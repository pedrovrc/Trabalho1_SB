# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

# The build target 
main: main.o preprocessamento.o macros.o tradutor.o
	$(CC) $(CFLAGS) -o montador main.o preprocessamento.o macros.o tradutor.o

main.o: main.cpp preprocessamento.h macros.h tradutor.h
	$(CC) $(CFLAGS) -c main.cpp

preprocessamento.o: preprocessamento.h
	$(CC) $(CFLAGS) -c preprocessamento.h

macros.o: macros.h
	$(CC) $(CFLAGS) -c macros.h

tradutor.o: tradutor.h
	$(CC) $(CFLAGS) -c tradutor.h

clean:
	$(RM) $main