# James Ray Tracker makefile

# Select appropriate compiler:
CPP=g++
CPPFLAGS=-O3 -fopenmp -static -lpthread -Wall -MMD -std=c++14      # multiple threads version
#CPPFLAGS=-O3 -Wall -MMD -std=c++14    # one thread version

JRT.exe: main.o Render.o
	$(CPP) $(CPPFLAGS) main.o Render.o -o JRT.exe

main.o: main.cpp
	$(CPP) $(CPPFLAGS) -c main.cpp -o main.o

Render.o:
	$(CPP) $(CPPFLAGS) -c src/Render.cpp -o Render.o

clean:
	del *.exe *.o *.d

run:
	nmake JRT.exe
	jrt.exe 10 "out/image.ppm"
