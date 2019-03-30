CXXFLAGS = -g -Wall -pedantic -std=c++17
BOOSTFLAGS = -I C:/MinGw/include -L C:/MinGw/lib
PATHFLAGS = -I C:/Users/Krzysiek/Documents/GitHub/SPD/Utility -L C:/Users/Krzysiek/Documents/GitHub/SPD/Bin

__START__:Bin/annealing.exe
	./Bin/annealing.exe

Bin/annealing.exe: Bin Bin/annealing.o Bin/utility.o
	g++ -o Bin/annealing.exe Bin/annealing.o Bin/utility.o

Bin/annealing.o: Simulated_Annealing/annealing.cpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} ${BOOSTFLAGS} -o Bin/annealing.o Simulated_Annealing/annealing.cpp

Bin/utility.o: Utility/utility.cpp 
	g++ -c ${CXXFLAGS} -o Bin/utility.o Utility/utility.cpp

clean: 
	rm Bin/*.o Bin/*.exe