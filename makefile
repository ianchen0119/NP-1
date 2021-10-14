all: clear output exec 

test: clear debug exec

output: main.cpp sh.cpp
	g++  main.cpp sh.cpp

debug: main.cpp sh.cpp
	g++ -D DEBUG main.cpp sh.cpp

exec: 
	./a.out
clear:
	rm -f *.out