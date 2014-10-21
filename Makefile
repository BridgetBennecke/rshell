
all:
	mkdir bin
	g++ -Wall -Werror -ansi -pendantic src/main.cpp -o bin/rshell.out

rshell:
	g++ -Wall -Werror -ansi -pendantic src/main.cpp -o bin/rshell.out


