DEBUG_ARGS := -Wall -Wextra -Wconversion -pedantic -g

MyLedger: Main.o Menu.o Institution.o Account.o Date.o
		g++ $(DEBUG_ARGS) -std=c++17 -L/lib/x86_64-linux-gnu/ Main.o Menu.o Institution.o Account.o Date.o -o MyLedger -lpqxx

Main.o: Main.cpp MenuItem.hpp MenuCommand.hpp Menu.cpp Menu.hpp Date.o Institution.o Account.o
		g++ $(DEBUG_ARGS) -std=c++17 -c Main.cpp

Menu.o: Menu.cpp Menu.hpp MenuItem.hpp
		g++ $(DEBUG_ARGS) -std=c++17 -c Menu.cpp

Date.o: Date.cpp Date.hpp
		g++ $(DEBUG_ARGS) -std=c++17 -c Date.cpp

Institution.o: Institution.cpp Institution.hpp DB.hpp
		g++ $(DEBUG_ARGS) -std=c++17 -L/lib/x86_64-linux-gnu/ -c Institution.cpp -lpqxx

Account.o: Account.cpp Account.hpp DB.hpp	
		g++ $(DEBUG_ARGS) -std=c++17 -c Account.cpp