#ifndef CATEGORIZER_H
#define CATEGORIZER_H
#endif
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "qtree.h"

#define BUFSIZE 1024
#define MAX_NAME 128
#define INPUT_ARG_MAX_NUM 3
#define DELIM " \r\n"

typedef struct client{
	int fd; //file descriptor to write into and to read from
	int answers [100];
	//before user entered a name, he cannot issue commands
	struct sockaddr_in addr;
	int state;//0 means user not in tree, 1 means user in tree, -1 disconnected.
	char name [MAX_NAME];
	char buf [BUFSIZE]; //each cli    // Note that we're passing in valid pointers for the second and third
    // arguments to accept here, so we can actually store and use client
    // information.ent has its own buffer
	int inbuf; //and a pointer to the current end-of-buf position
	struct client *next;
	int count_ans;
	QNode *prev;
	QNode *curr;
	Node *in;
}Client;

int init_tree(char *filename, QNode **root_p, Node **interests_p);//return 1 if no tree is built
int login(struct client *cl, struct client *top);
int read_line(struct client *cl, char *result);
int do_test(Node *interests, QNode *root, struct client *cl);
void get_all(Node *interests, QNode *root, struct client *cl);
int find_network_newline(const char *buf, int inbuf);
void read_answer(struct client *cl);

void print_friends(Node *list, char *name, int fd);
Node *find_opposite(QNode *current, int *answers);
int validate_answer(char *answer, int fd);
int validate_user(char *name, int fd);
void wrap_up();
