flags=-Wall -Werror -ansi -pedantic
test=if [ ! -d bin ]; then mkdir bin; fi

all:
	$(test)
	g++ $(flags) src/main.cpp -o bin/rshell

rshell:
	$(test)
	g++ $(flags) src/main.cpp -o bin/rshell



