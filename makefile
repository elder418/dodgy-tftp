CLIOBJ=packet.cpp client.cpp
SRVOBJ=packet.cpp server.cpp

CC=g++
CFLAGS=
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
