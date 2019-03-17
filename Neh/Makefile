CXXFLAGS = -pedantic -Wall -std=c++17 

__START__: neh.exe
	./neh.exe

neh.exe: main.o utility.o 
	g++ -o neh.exe utility.o main.o

main.o: main.cpp 	
	g++ -c ${CXXFLAGS} main.cpp

utility.o: utility.cpp 
	g++ -c ${CXXFLAGS} utility.cpp 

clean:
	rm -f *.o *.exe