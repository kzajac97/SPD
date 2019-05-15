CXXFLAGS = -g -Iinc -Wall -pedantic -std=c++17
BOOSTFLAGS = -I C:/MinGw/include -L C:/MinGw/lib
PATHFLAGS = -I C:/Users/Krzysiek/Documents/GitHub/SPD/Incl -L C:/Users/Krzysiek/Documents/GitHub/SPD/Bin

__START__:Bin/spd.exe
	./Bin/spd.exe

Bin/spd.exe: Bin Bin/main.o Bin/annealing.o Bin/flowshop.o Bin/neh.o Bin/scharge.o Bin/utility.o Bin/carlier.o
	g++ -o Bin/spd.exe Bin/main.o Bin/flowshop.o Bin/neh.o Bin/annealing.o Bin/scharge.o Bin/carlier.o Bin/utility.o

Bin/main.o: Src/main.cpp 
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/main.o Src/main.cpp

Bin/flowshop.o: Src/flowshop.cpp Incl/flowshop.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/flowshop.o Src/flowshop.cpp

Bin/neh.o: Src/neh.cpp Incl/neh.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/neh.o Src/neh.cpp

Bin/annealing.o: Src/annealing.cpp Incl/annealing.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/annealing.o Src/annealing.cpp

Bin/scharge.o: Src/scharge.cpp Incl/scharge.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/scharge.o Src/scharge.cpp

Bin/carlier.o: Src/carlier.cpp Incl/carlier.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/carlier.o Src/carlier.cpp

Bin/utility.o: Src/utility.cpp Incl/utility.hpp
	g++ -c ${CXXFLAGS} ${PATHFLAGS} -o Bin/utility.o Src/utility.cpp

clean: 
	rm Bin/*.o Bin/*.exe