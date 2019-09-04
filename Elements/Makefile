CXX=g++
STD=-std=c++11
MAIN=-o main
all: main.o Deck.o Card.o
	$(CXX) main.o Deck.o Card.o $(STD) $(MAIN)

main.o:
	$(CXX) -c main.cc $(STD)

Deck.o:
	$(CXX) -c Deck.cc $(STD)

Card.o:
	$(CXX) -c Card.cc $(STD)

clean:
	rm *.o ./main
