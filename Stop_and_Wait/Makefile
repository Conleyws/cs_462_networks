CPP = g++
CFLAGS = -g -Wall -pedantic -std=c++11
LIBS = -pthread
all: client server
client: client.o Sieve.o
	$(CPP) $(CFLAGS) -o $@ $^ $(LIBS)

server: server.o Sieve.o
	$(CPP) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp Makefile
	$(CPP) $(CFLAGS) -c -o $@ $< $(LIBS)

%.o: %.cpp %.h Makefile
	$(CPP) $(CFLAGS) -c -o $@ $< $(LIBS)

clean:
	rm -f *.o main
