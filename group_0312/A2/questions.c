#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questions.h"

Node * get_list_from_file (char *input_file_name) {
	Node * head = NULL;
	Node * tail = NULL;
    //read lines and add to the list
	FILE *read_file;
	char line[MAX_LINE + 1];
	read_file = fopen(input_file_name,"r");

	if (read_file == NULL){
		fprintf(stderr,"Error opening file\n");
	}

	while (fgets(line,MAX_LINE,read_file) != NULL) {
		line[strcspn(line,"\r\n")] = '\0';
		if (head == NULL){
			head = malloc(sizeof(Node));
			tail = head;
		}else{
			tail->next = malloc(sizeof(Node));
			tail = tail->next;
		}
		tail ->str = malloc(sizeof(char)*strlen(line) + 1);
		tail->next = NULL;
		strcpy(tail->str, line);
	}
	fclose(read_file);
	
	return head;
}

void print_list (Node *head) {
	Node *current = head;
	while (current != NULL){
		fprintf(stdout, "%s\n", current->str);
		current = current->next;
	}
}

void free_list (Node *head) {
	while (head != NULL){
		Node * next = head->next;
		free(head->str);
		free(head);
		head = next;
	}
}
