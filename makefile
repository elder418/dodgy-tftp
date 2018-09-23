OBJ=base.cpp

CC=g++
CFLAGS=
LFLAGS=
OBJNAME=test

all: $(OBJ)
		$(CC) $(OBJ) $(CFLAGS) $(LFLAGS) -o $(OBJNAME)

clean:
		rm -r *.o
