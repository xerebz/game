CFLAGS = \
	-Wall \
	-Weffc++ \
	-Werror \
	-Wextra \
	-Wnon-virtual-dtor \
	-Wshadow \
	-Wstrict-aliasing \
	-pedantic
	-std=c++2a \

all: client server

# compiling with both g++ and clang++ to catch any issues at compile time as possible

client: client.cpp state.h state.cpp
	g++ $(CFLAGS) client.cpp state.cpp -lcurses -o client
	clang++ $(CFLAGS) -lcurses client.cpp state.cpp -o client

server: server.cpp state.h state.cpp
	g++ $(CFLAGS) server.cpp state.cpp -o server
	clang++ $(CFLAGS) server.cpp state.cpp -o server

.PHONY: clean
clean:
	rm client server
