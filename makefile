CPPFLAGS= -Wall -Wextra -pedantic -g

all:	Test_Interpolate

Test_Interpolate:	Test_Interpolate.o
				g++ $(CPPFLAGS) -pthread Test_Interpolate.o -o Test_Interpolate

Test_Interpolate.o:	Test_Interpolate.cpp
					g++ $(CPPFLAGS) -pthread -c Test_Interpolate.cpp

run:	Test_Interpolate
		./Test_Interpolate

checkmem:	Test_Interpolate
			valgrind --leak-check=full ./Test_Interpolate

clean:
			rm -f *.o Test_Interpolate
