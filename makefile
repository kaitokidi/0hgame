# Makefile super op

all: main.o everything exe

main.o: view.cpp
	g++ -c view.cpp	
	
everything:
	g++ -o view view.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
		
exe:
	./view
	
clear:
	rm *.o view
