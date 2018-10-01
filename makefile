CLIOBJ=pack.cpp client.cpp
SRVOBJ=pack.cpp sock.cpp server.cpp

CC=g++
CFLAGS=-g
LFLAGS=
CLINAME=client
SRVNAME=server

all: $(SRVOBJ) $(CLIOBJ)
		$(CC) $(SRVOBJ) $(CFLAGS) $(LFLAGS) -o $(SRVNAME)
		$(CC) $(CLIOBJ) $(CFLAGS) $(LFLAGS) -o $(CLINAME)
client: $(CLIOBJ)
		$(CC) $(CLIOBJ) $(CFLAGS) $(LFLAGS) -o $(CLINAME)
server: $(SRVOBJ)
		$(CC) $(SRVOBJ) $(CFLAGS) $(LFLAGS) -o $(SRVNAME)
clean:
		rm -r *.o
