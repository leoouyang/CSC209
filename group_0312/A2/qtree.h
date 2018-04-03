#ifndef QTREE_H
#define QTREE_H
#include "questions.h"

typedef enum {
    REGULAR, LEAF
} NodeType;

union Child {
	struct str_node *fchild;
	struct QNode *qchild;
} Child;

typedef struct QNode {
	char *question;
	NodeType node_type;
	union Child children[2];
} QNode;

QNode *add_next_level (QNode *current, Node * list_node);
void add_user(QNode *root, int answers[],char *username);
void append_user(QNode *current, int answer, char *username);
Node *create_user(char *username);
QNode *traverse(QNode *current, int answer);
void free_tree(QNode *root);
Node * friend_search(char *username,QNode *root);
void print_qtree (QNode *parent, int level);
void print_users (Node *parent);

#endif
