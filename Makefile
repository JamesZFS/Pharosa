# James Ray Tracker makefile

# Select appropriate compiler:
#CPP=g++		# use on win
CPP=g++-8	# use on mac
#CPPFLAGS=-O3 -fopenmp -static -lpthread -Wall -MMD -std=c++14      # multiple threads version on win
CPPFLAGS=-O3 -fopenmp -Wall -MMD -std=c++14      # multiple threads version on mac
#CPPFLAGS=-O3 -Wall -MMD -std=c++14    # one thread version

Pharosa.out: main.cpp Makefile
	$(CPP) $(CPPFLAGS) main.cpp -o Pharosa.out

smallpt.out: smallpt.cpp Makefile
	$(CPP) $(CPPFLAGS) smallpt.cpp -o smallpt.out

clean:
#	del *.o *.d	# on win
	rm *.d		# on mac

clean-up:
#	del *.exe *.o *.d	# on win
	rm *.out *.o *.d		# on mac

run: Pharosa.out
	./Pharosa.out
	make clean
#	nmake clean

compare: smallpt.out Pharosa.out
	./smallpt.out 40 standard.ppm
	./Pharosa.out
	make clean
