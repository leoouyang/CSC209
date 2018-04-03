#include "qtree.h"

void print_greetings();

int main (int argc, char **argv) {
	
	QNode *root = NULL;	
	char answer[MAX_LINE];	
	char name[MAX_LINE];
	

	Node * interests = NULL;

	if (argc < 2) {
        printf ("To run the program ./categorizer <name of input file>\n");
        return 1;
    }
	
    interests = get_list_from_file ( argv[1]);
	
    if (interests == NULL)
        return 1;

	root = add_next_level (root,  interests);
	free_list (interests);

	//main application loop
	while (answer[0] != 'q' && answer[0] != 'Q'){
		print_greetings();
		int valid_name = 0;
		while (valid_name == 0 && answer[0]!= 'q'){
			printf("Please enter your name. Type 'q' to quit\n");
			fgets(name, MAX_LINE, stdin);
			name[strcspn(name,"\r\n")] = '\0';
			if (strlen(name) == 1 && name[0] == 'q'){
				answer[0] = 'q';
			}else{
				if (strlen(name) >= 8 && strlen(name) <= 128){
					valid_name = 1;
				}else{
					fprintf(stdout, "The length of user name should be between 8 and 128\n");
				}
				for(int i = 0; i < strlen(name); i++){
					if (!(((int)name[i] <= 122 && (int)name[i] >= 97)||((int)name[i] <= 90 && (int)name[i] >= 65)||((int)name[i] <= 57 && (int)name[i] >= 48))){
						valid_name = 0;
						fprintf(stdout, "User name contains characters that is not alphanumeric.\n");
					}
				}
			}
		}
		if (friend_search(name,root) == NULL){
			QNode *current = root;
			while (current->node_type != LEAF && answer[0] != 'q' && answer[0] != 'Q'){
				fprintf(stdout, "%s\n",current->question);
				fgets(answer, MAX_LINE, stdin);
				answer[strcspn(answer,"\r\n")] = '\0';
				if (strlen(answer) > 3){
					fprintf(stdout, "The answer is too long.\n");
				}
				else if (answer[0] != 'Q'&& answer[0] != 'q'&& answer[0] != 'Y'&& answer[0] != 'y'&& answer[0] != 'N' && answer[0] != 'n'){
					fprintf(stdout, "The answer is invalid.\n");
				}
				else{
					if (answer[0] == 'Y' || answer[0] == 'y'){
						current = current->children[1].qchild;
					}
					else if (answer[0] == 'N' || answer[0] == 'n'){
						current = current->children[0].qchild;
					}
				}
			}
			if (current->node_type == LEAF && answer[0] != 'q' && answer[0] != 'Q'){
				fprintf(stdout, "%s\n",current->question);
				fgets(answer, MAX_LINE, stdin);
				answer[strcspn(answer,"\r\n")] = '\0';
				if (answer[0] == 'N' || answer[0] == 'n'){
					append_user(current, 0, name);
				}else if(answer[0] == 'Y' || answer[0] == 'y'){
					append_user(current, 1, name);
				}else{
					fprintf(stdout, "The answer is invalid.\n");
				}
			}
		}
			//print recommdation for friends
		if (answer[0] != 'q' && answer[0] != 'Q'){
			Node *potentialfds = friend_search(name,root);
			if (potentialfds == NULL){
				fprintf(stderr, "potentialfds should not be NULL at this point.\n");
			}else{
				if (!strcmp(potentialfds->str, name) && potentialfds->next == NULL){
					fprintf(stdout, "Sorry, no users with similar interests joined yet\n");
				}
				else{
					fprintf(stdout, "friend recommendations for user %s\n",name);
					int count = 0;
					while (potentialfds != NULL){
						if (strcmp(potentialfds->str, name)){
							fprintf(stdout, "%s\n", potentialfds->str);
							count ++;
						}
						potentialfds = potentialfds->next;
					}
					fprintf(stdout, "You have total %d potential friend(s)!!!\n",count);
				}
			}
		}
	}
    //end of main loop - the user typed "q"
    print_qtree (root, 0);
    free_tree(root);
	return 0;
}

void print_greetings () {
	printf ("----------------------------------------------\n");
	printf ("Friend recommender system. Find people who are just like you!\n");
	printf ("CSC209 fall 2016 team. All rights reserved\n");
	printf ("----------------------------------------------\n");
}
