#include <ctype.h>
#include "qtree.h"
#include "categorizer.h"


char *welcome = "Welcome.\nGo ahead and enter user commands>\n";
char *question_prompt = "Do you like %s?\n";
char *test_complete = "Test complete.\n";
char *neg_result = "No completing personalities found. Please try again later.\n";
char *pos_result = "Here are you best mismatches:\n";
char *collect_info = "Collecting your interests\n";
char *active= "An user with this name is currently active.\n";

void get_all(Node *interests, QNode *root, struct client *cl){
	Node *oppo_list = find_opposite(root, cl->answers);
    print_friends(oppo_list, cl->name, cl->fd);
}

int do_test(Node *interests, QNode *root, struct client *cl){
	if (cl->state != 3){
		write(cl->fd, collect_info, strlen(collect_info)+1);
		cl->state = 3;
	}
	if (cl->count_ans == 0){
		cl->prev = cl->curr = root;
	}
	// iterate over list of interests
	Node *i = interests;

	if (i){
		char print_out[strlen(question_prompt) + 40];
		sprintf(print_out, question_prompt, i->str);
		write(cl->fd, print_out, strlen(print_out)+1);
		cl->in = i;
		return 0;
	}
	else{
		return -1;
	}

}

void read_answer(struct client *cl){

	// iterate over list of interests
	Node *i = cl->in;
	int ans;
	// read answer to prompt
	char answer[BUFSIZE];
	read_line(cl, answer);
	ans = validate_answer(answer, cl->fd);

	// if answer if not valid, continue to loop
	if (ans == 2){
		char print_out[strlen(question_prompt) + 40];
		sprintf(print_out, question_prompt, i->str);
		write(cl->fd, print_out, strlen(print_out)+1);
		cl->in = i;
	}
	else{
		cl->prev = cl->curr;
		cl->curr = find_branch(cl->curr, ans);
		cl->answers[cl->count_ans] = ans;
		cl->count_ans++;
		i = i->next;
		cl->in = i;
		if (do_test(cl->in,cl->curr,cl) == -1){
			// add user to the end of the list
			Node *user_list = cl->prev->children[ans].fchild;
			cl->prev->children[ans].fchild = add_user(user_list, cl->name);
			cl->state = 1;
			write(cl->fd, test_complete, strlen(test_complete)+1);
			printf("recorded activity for %s\n", cl->name);
		}

	}
}

int find_network_newline(const char *buf, int inbuf) {
  // Step 1: write this function
	for (int i=0;i<inbuf;i++){
		if (buf[i] == '\r'){
			return i;
		}
	}
  return -1; // return the location of '\r' if found
}

int read_line(struct client *cl, char *result){
	int room = BUFSIZE - cl->inbuf; // room == capacity of the whole buffer
	char *after = cl->buf + cl->inbuf;        // start writing at beginning of buf
	int where;
	int nbytes;
	while ((nbytes = read(cl->fd, after, room)) > 0) {
		cl->inbuf += nbytes;
		where = find_network_newline(cl->buf, cl->inbuf);
		if (where >= 0) { // OK. we have a full line
			cl->buf[where] = '\0';
			cl->buf[where+1] = '\0';
			//printf("Next message: %s\n", cl->buf);
			strncpy(result, cl->buf, strlen(cl->buf)+1);
			where += 2;
			cl->inbuf -= where;
			memmove(cl->buf,cl->buf+where,cl->inbuf);
			return 0;
		}
		room = BUFSIZE - cl->inbuf;
		after = cl->buf + cl->inbuf;
	}
	return 1;
}
int init_tree(char *filename, QNode **root_p, Node **interests_p){
	QNode *current = NULL;
	*interests_p = get_list_from_file (filename);
    if (*interests_p == NULL)
        return 1;
    *root_p = add_next_level (current,  *interests_p);
    return 0;
}

int login(struct client *cl, struct client *top){
	char name[BUFSIZE];
	int exist = 0;
	struct client *temp;
	//print_greetings();
	if (read_line(cl, name) == 1){
		return -1;
	}
	// if user name is not valid, continue to loop
	for(temp = top; temp; temp=temp->next)
		if (temp->fd != cl->fd)
			if(!strcmp(temp->name, name)){
				write(cl->fd, active, strlen(active)+1);
				exist = 1;
			}
	if(exist)
		return 1;
	if (!validate_user(name, cl->fd))
		return 1;
	strncpy(cl->name, name, MAX_NAME-1);
	write(cl->fd, welcome, strlen(welcome)+1);
	return 0;
	}


/*void wrap_up(){
    //end of main loop - the user typed "q"
    print_qtree (root, 0);
    
    free_list (interests);
    free_qtree(root);
    
    exit(0);
}*/

int validate_user(char *name, int fd){
    int valid = 1;
    
    int len = strlen(name);
    char *warning = "WARNING: User name is longer than 128. It has been truncated to 127.\n";
    if (len >= 128){
    	write(fd, warning, strlen(warning)+1);
    	len = 127;
    }
    if (len < 8)
        valid = 0;
    
    for (int i = 0; i < len; i++){
        if (!isalnum(name[i])){
            valid = 0;
            break;
        }
    }

    if (!valid){
		char *error = "ERROR: User name must be at least 8, and can only contain alphanumeric characters.\n";
		write(fd, error, strlen(error)+1);
    }
    return valid;
}

int validate_answer(char *answer, int fd){
    char *invalid_message = "ERROR: Answer must be one of 'y', 'n', 'q'.\n";
    
    if (strlen(answer) > 3){
        write(fd, invalid_message, strlen(invalid_message + 1));
        return 2;
    }
    if (answer[0] == 'n' || answer[0] == 'N')
        return 0;
        
    if (answer[0] == 'y' || answer[0] == 'Y')
        return 1;
        
    write(fd, invalid_message, strlen(invalid_message + 1));
    return 2;
}

// print list of potential friends for user
void print_friends(Node *list, char *name, int fd){
    int friends = 0;
    char result[BUFSIZE];
    // iterate over user list and count the number of friends
    while (list) {
	// ignore this user
        friends++;
	    // if this is the first friend found, print successful message    
		if (friends == 1){
			write(fd, pos_result, strlen(pos_result)+1);
		}
	    // if friend was found, print his/her name
        sprintf(result, "%s\n", list->str);
		write(fd, result, strlen(result)+1);
        list = list->next;
    }
    
    // if friends were found, print the number of friends    
    if (!friends){
		write(fd, neg_result, strlen(neg_result)+1);
    }
}

Node *find_opposite(QNode *current, int *answers){
    // current is an inner node
    int i = 0;
    while (current->node_type == REGULAR){
    	if(answers[i]){
      	current = current->children[0].qchild;
      }else {
      	current = current->children[1].qchild;
      }
      i++;
    }
    Node *result;
    if(answers[i]){
      result = current->children[0].fchild;   
    }else {
      result = current->children[1].fchild;   
    }
    return result;
}
