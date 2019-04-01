# James Ray Tracker makefile

# Select appropriate compiler:
CPP=g++		# use on win
#CPP=g++-8	# use on mac
CPPFLAGS=-O3 -fopenmp -static -lpthread -Wall -MMD -std=c++14      # multiple threads version
#CPPFLAGS=-O3 -Wall -MMD -std=c++14    # one thread version

JRT.exe: main.o Render.o Vec.o
	$(CPP) $(CPPFLAGS) main.o Render.o -o JRT.exe

main.o:
	$(CPP) $(CPPFLAGS) -c main.cpp -o main.o

Vec.o:
	$(CPP) $(CPPFLAGS) -c src/Vec.cpp -o Vec.o

Ray.o:
	$(CPP) $(CPPFLAGS) -c src/Ray.cpp -o Ray.o

Render.o:
	$(CPP) $(CPPFLAGS) -c src/Render.cpp -o Render.o


clean:
	del *.exe *.o *.d

run: JRT.exe
	jrt.exe 10 "out/image.ppm"
