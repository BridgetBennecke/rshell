flags=-Wall -Werror -ansi -pedantic
test=if [ ! -d bin ]; then mkdir bin; fi

all:
	$(test)
	g++ $(flags) src/main.cpp -o bin/rshell

rshell:
	$(test)
	g++ $(flags) src/main.cpp -o bin/rshell

ls:
	$(test)
	g++ $(flags) src/ls.cpp -o bin/ls

cp:
	$(test)
	g++ $(flags) src/cp.cpp -o bin/cp


