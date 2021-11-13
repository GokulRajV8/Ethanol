main: tester.o node.o layer.o network.o trainer.o tools.o
	g++ -o tester tester.o node.o layer.o network.o trainer.o tools.o

tester.o: tester.cpp ethanol.h
	g++ -c tester.cpp

node.o: node.cpp ethanol.h
	g++ -c node.cpp

layer.o: layer.cpp ethanol.h
	g++ -c layer.cpp

network.o: network.cpp ethanol.h
	g++ -c network.cpp

trainer.o: trainer.cpp ethanol.h
	g++ -c trainer.cpp

tools.o: tools.cpp ethanol.h
	g++ -c tools.cpp