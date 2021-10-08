all: output exec clear
output: main.cpp sh.cpp
	g++ main.cpp sh.cpp
exec: 
	./a.out
clear:
	rm -f *.out