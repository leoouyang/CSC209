#include "qtree.h"

QNode *add_next_level (QNode *current, Node *list_node) {
	int str_len;
	
	str_len = strlen (list_node->str);
	current = (QNode *) calloc (1, sizeof(QNode));

	current->question =  (char *) calloc (str_len +1, sizeof(char ));
	strncpy ( current->question, list_node->str, str_len );
	current->question [str_len] = '\0';  
	current->node_type = REGULAR;
	
	if (list_node->next == NULL) {
		current->node_type = LEAF;
		return current;
	}
	
	current->children[0].qchild = add_next_level ( current->children[0].qchild, list_node->next);
	current->children[1].qchild = add_next_level ( current->children[1].qchild, list_node->next);

	return current;
}

void print_qtree (QNode *parent, int level) {
	int i;
	for (i=0; i<level; i++)
		printf("\t");
	
	printf ("%s type:%d\n", parent->question, parent->node_type);
	if(parent->node_type == REGULAR) {
		print_qtree (parent->children[0].qchild, level+1);
		print_qtree (parent->children[1].qchild, level+1);
	}
	else { //leaf node
		for (i=0; i<(level+1); i++)
			printf("\t");
		print_users (parent->children[0].fchild);
		for (i=0; i<(level+1); i++)
			printf("\t");
		print_users (parent->children[1].fchild);
	}
}

void print_users (Node *parent) {
	if (parent == NULL)
		printf("NULL\n");
	else {
		printf("%s, ", parent->str);
		while (parent->next != NULL) {
			parent = parent->next;
			printf("%s, ", parent->str);
		}
		printf ("\n");
	}
}

void add_user(QNode *root, int answer[], char *username){
	QNode *current = root;
	int i = 0;
	while(current->node_type == REGULAR){
		current = traverse(current, answer[i]);
		i++;
	}
	if (current->node_type == LEAF){
		append_user(current, answer[i], username);
	}
}

void append_user(QNode *current, int answer, char *username){
	if (current->children[answer].fchild == NULL){
		current->children[answer].fchild = create_user(username);
	}else{
		Node *curr = current->children[answer].fchild;
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = create_user(username);
	}
}
Node *create_user(char *username){
	Node *result = calloc(1, sizeof(Node));
	result->str = calloc(strlen(username)+1, sizeof(char));
	strcpy(result->str, username);
	return result;
}

QNode *traverse(QNode *current, int answer){
	return current->children[answer].qchild;
}

void free_tree(QNode *root){
	QNode *current = root;
	if (current->node_type == LEAF){
		Node *left = current->children[0].fchild;
		if (left != NULL){
			Node *temp_left = left;
			while (temp_left != NULL){
				left = left->next;
				free(temp_left->str);
				free(temp_left);
				temp_left = left;
			}
		}
		Node *right = current->children[1].fchild;
		if (right != NULL){
			Node *temp_right = right;
			while (temp_right != NULL){
				right = right->next;
				free(temp_right->str);
				free(temp_right);
				temp_right = right;
			}
		}
		current->children[0].fchild = NULL;
		current->children[1].fchild = NULL;
		free (current->question);
		free (current);
	}else{
		free_tree(current->children[0].qchild);
		free_tree(current->children[1].qchild);
		free(current->question);
		free(current);
	}
	root = NULL;
}

Node * friend_search(char *username,QNode *root){
	QNode *current = root;
	Node *result = NULL;
	if (current->node_type == LEAF){
		Node *left = current->children[0].fchild;
		while (left != NULL){
			if (!strcmp(left->str, username)){
				result = (current->children[0].fchild);
			}
			left = left->next;
		}
		Node *right = current->children[1].fchild;
		while (right != NULL){
			if (!strcmp(right->str, username)){
				result = (current->children[1].fchild);
			}
			right = right->next;
		}
		return result;
	}else{
		Node *l = friend_search(username,current->children[0].qchild);
		Node *r = friend_search(username,current->children[1].qchild);
		if (l== NULL && r == NULL){
			result = NULL;
		}
		else if (l != NULL){
			result = l;
		}
		else{
			result = r;
		}
		return result;
	}
}

