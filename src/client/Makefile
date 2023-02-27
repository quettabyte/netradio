CFLAGS+=-I../include/ -Wall

all:client

client:client.o
	gcc $^ -o $@(CFLAGS)

clean:
	rm -rg *.o client


