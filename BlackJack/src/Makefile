CXX=g++
STD=-std=c++11
SERV=-o server
CLIENT=-o client
server: server.o Deck.o Game.o ServerListenHandle.o ClientListenHandle.o BJPlayer.o Card.o
	$(CXX) server.o Deck.o Game.o ServerListenHandle.o ClientListenHandle.o BJPlayer.o Card.o $(STD) $(SERV)

client: client.o ServerListenHandle.o Deck.o BJPlayer.o ClientListenHandle.o Card.o Game.o
	$(CXX) client.o ServerListenHandle.o Deck.o BJPlayer.o ClientListenHandle.o Card.o Game.o $(STD) $(CLIENT)

server.o:
	$(CXX) -c server.cc $(STD)

Player.o:
	$(CXX) -c ../../Elements/Player.cc $(STD)

BJPlayer.o:
	$(CXX) -c ../../Elements/BJPlayer.cc $(STD)

Deck.o:
	$(CXX) -c ../../Elements/Deck.cc $(STD)

Card.o:
	$(CXX) -c ../../Elements/Card.cc $(STD)

Game.o:
	$(CXX) -c Game.cc $(STD)

ServerListenHandle.o:
	$(CXX) -c ServerListenHandle.cc $(STD)

ClientListenHandle.o:
	$(CXX) -c ClientListenHandle.cc $(STD)

clean:
	rm *.o ./server ./client
