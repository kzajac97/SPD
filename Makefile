CXXFLAGS = -g -Wall -pedantic -std=c++17
BOOSTFLAGS = -I C:/MinGw/include -L C:/MinGw/lib
PATHFLAGS = -I C:\Users\Krzysiek\Documents\GitHub\SPD\Utility -L C:\Users\Krzysiek\Documents\GitHub\SPD\Bin

__START__:Bin/neh.exe
	./Bin/neh.exe

Bin/neh.exe: Bin Bin/neh.o Bin/utility.o
	g++ -o Bin/neh.exe Bin/neh.o Bin/utility.o

Bin/neh.o: Neh/neh.cpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/neh.o Neh/neh.cpp

Bin/utility.o: Utility/utility.cpp 
	g++ -c ${CXXFLAGS} -o Bin/utility.o Utility/utility.cpp

clean: 
	rm Bin/*.o Bin/*.exe