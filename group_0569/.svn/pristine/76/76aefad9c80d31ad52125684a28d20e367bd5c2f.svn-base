PORT=<y>
CC = gcc
PORT = 51012
CFLAGS = -DPORT=$(PORT) -Wall -std=c99 -g

all:mismatch_server

mismatch_server:
	$(CC) $(CFLAGS) qtree.c questions.c categorizer.c mismatch_server.c -o mismatch_server

clean:  
	rm *.o mismatch_server
