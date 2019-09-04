CXX=g++
STD=-std=c++11
MAIN=-o main
all: main.o Deck.o
	$(CXX) main.o Deck.o $(STD) $(MAIN)

main.o:
	$(CXX) -c main.cc $(STD)

Deck.o:
	$(CXX) -c Deck.cc $(STD)

clean:
	rm *.o ./main
