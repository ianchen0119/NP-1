all: clear output exec 

test: clear debug exec

output: main.cpp sh.cpp
	g++ -Werror -Wall -O3 main.cpp sh.cpp -o npshell

debug: main.cpp sh.cpp
	g++ -Werror -Wall -O3 -D DEBUG main.cpp sh.cpp -o npshell

exec: 
	./npshell
clear:
	rm -f npshell