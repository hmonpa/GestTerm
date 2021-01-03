OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

driver.exe: driver_gestterm.o ubicacio.o contenidor.o terminal.o
	g++ -o driver.exe driver_gestterm.o ubicacio.o contenidor.o terminal.o -lesin
	rm *.o

ubicacio.o: ubicacio.cpp ubicacio.hpp
	g++ -c ubicacio.cpp $(OPCIONS)

contenidor.o: contenidor.cpp contenidor.hpp
	g++ -c contenidor.cpp $(OPCIONS)

terminal.o: terminal.cpp terminal.hpp
	g++ -c terminal.cpp $(OPCIONS)

driver_gestterm.o: driver_gestterm.cpp ubicacio.hpp contenidor.hpp terminal.hpp
	g++ -c driver_gestterm.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch
