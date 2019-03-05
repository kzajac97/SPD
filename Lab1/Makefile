CXXFLAGS = -pedantic -Wall -std=c++17 

__START__: lab1
	./lab1

lab1: main.o utility.o 
	g++ -o lab1 utility.o main.o

main.o: main.cpp 	
	g++ -c ${CXXFLAGS} main.cpp

utility.o: utility.cpp 
	g++ -c ${CXXFLAGS} utility.cpp 

clean:
	rm -f *.o 