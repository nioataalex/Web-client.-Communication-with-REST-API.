CC=gcc
CFLAGS=-I.

build: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client client.c requests.c functions.c helpers.c buffer.c parson.c -Wall

run: client
	./client

clean:
	rm -f *.o client
