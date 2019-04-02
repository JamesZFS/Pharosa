# James Ray Tracker makefile

# Select appropriate compiler:
#CPP=g++		# use on win
CPP=g++-8	# use on mac
#CPPFLAGS=-O3 -fopenmp -static -lpthread -Wall -MMD -std=c++14      # multiple threads version
CPPFLAGS=-O3 -Wall -MMD -std=c++14    # one thread version

JRT.out: main.cpp Makefile
	$(CPP) $(CPPFLAGS) main.cpp -o JRT.out

clean:
#	del *.o *.d	# on win
	rm *.o *.d		# on mac

clean-up:
#	del *.exe *.o *.d	# on win
	rm *.out *.o *.d		# on mac

run: JRT.out
	./JRT.out
	make clean
#	nmake clean
