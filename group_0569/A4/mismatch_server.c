#include "categorizer.h"
#ifndef PORT
#define PORT 30000
#endif



Client *top = NULL;
QNode *root = NULL;
Node *interests = NULL;
int ques_num;

char *in_tree = "You have already answered the questions. Use \"get_all\" to get the best mismatches\n";
char *not_in_tree = "Sorry, you haven't answered the questions yet.\n";
char *message_prompt = "Message from %s: %s\n";
char *record_prompt = "recorded activity for %s\n";
char *no_user_prompt = "NO such user is in connection.\n";
char *user_prompt = "What is your user name?\n";

int setup(void);
static Client* addclient(int fd, struct sockaddr_in addr);
static void removeclient(int fd);
void error(char *msg);
int tokenize(char *cmd, char **cmd_argv);
int process_args(int cmd_argc, char **cmd_argv, struct client *cl, struct client *head,int listenfd);


int main(int argc, char **argv) {
	int listenfd;
	int fd, maxfd;
	fd_set readfds; //set of socket descriptors

	struct sockaddr_in peer;
	socklen_t socklen;

	if (argc < 2) {
		printf ("To run the program ./mismatch_server <name of input file>\n");
		return 1;
	}
	if(init_tree(argv[1], &root, &interests) == 1){
		printf("Fail to construct tree\n");
		return 1;
	}

	listenfd = setup();
	maxfd = listenfd;
	socklen = sizeof(peer);
	Client *current_client;
	FD_ZERO(&readfds);
	Client *p;
	while (1) {
		FD_SET(listenfd,&readfds);
		if (top)
			for(p=top;p;p = p->next)
				FD_SET(p->fd, &readfds);
        fd = select(maxfd + 1 , &readfds , NULL , NULL , NULL);
		if (fd < 0) {
			perror("select");
		}
		else{

		    for(p=top;p;p = p->next)
		    	if (FD_ISSET(p->fd,&readfds))
		    		break;
		    if (p){
		    		if (p->state == 2){
		    			int b = login(p,top);
		    			if (b == 0){
		    				p->state = find_user(root,p->name,p->answers,0);
		    			}
		    			else if (b == -1){
						FD_CLR(p->fd, &readfds);
		    				removeclient(p->fd);
		    			}
		    		}
		    		else if (p->state == 3){
		    			read_answer(p);
		    		}
		    		else{
						char *msg[3];
						char result[BUFSIZE];
						if (read_line(p, result) == 1){
							FD_CLR(p->fd, &readfds);
							removeclient(p->fd);//return a whole line read
						}
						else{
							int argc = tokenize(result, msg);
							int r;
							r = process_args(argc, msg, p, top,listenfd);
							if (r == -1){
								//p->state = -1;
								FD_CLR(p->fd, &readfds);
								removeclient(p->fd);
							}
						}

		    		}


		    }//client message received
			//If something happened on the master socket , then its an incoming connection
		    if (FD_ISSET(listenfd, &readfds)) {
		            if ((fd = accept(listenfd, (struct sockaddr *)&peer, (socklen_t*)&socklen))<0){
		                perror("accept");
		            }
		            if(fd > maxfd){
		            	maxfd = fd;
		            }
		            current_client = addclient(fd,peer);
		            write(current_client->fd, user_prompt, strlen(user_prompt)+1);//ask for the user's name and validate
		            FD_SET(current_client->fd,&readfds);

		            /*if (top!= NULL){
		            	Client *temp = top;
		            	while(temp != NULL){
		            		printf("%s %d\n",temp->name,temp->fd);
		            		temp = temp->next;
		            	}
		            }else{
		            	printf("top is null\n");
		            }*/
		    }
		}
	}
  return 0;
}
 /* Copy from the Sample Server Code
 */
static void removeclient(int fd){
    struct client **p;
    for (p = &top; *p && (*p)->fd != fd; p = &(*p)->next)
	;
    if (*p) {
	struct client *t = (*p)->next;
	close(fd);
	printf("Removing client %s\n", inet_ntoa((*p)->addr.sin_addr));
	fflush(stdout);
	free(*p);
	*p = t;
    } else {
	fprintf(stderr, "Trying to remove fd %d, but I don't know about it\n", fd);
	fflush(stderr);
    }
}


/*Search the first inbuf characters of buf for a network newline ("\r\n").
  Return the location of the '\r' if the network newline is found,
  or -1 otherwise.
  Definitely do not use strchr or any other string function in here. (Why not?)
*/

/*
 * Copy from the Sample Server Code
 */

static Client* addclient(int fd, struct sockaddr_in addr) {
    struct client *p = malloc(sizeof(struct client));
    if (!p) {
	fprintf(stderr, "out of memory!\n");  /* highly unlikely to happen */
	exit(1);
    }
    printf("Adding client %s\n", inet_ntoa(addr.sin_addr));
    fflush(stdout);
    p->fd = fd;
    p->addr = addr;
    p->state = 2;
    p->inbuf = 0;
    p->next = top;
    top = p;
    printf("New connection on port %d\n", ntohs(p->addr.sin_port));
    return p;
}


void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

int setup(void) {
 int on = 1, status;
 struct sockaddr_in self;
 int listenfd;
 if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
   perror("socket");
   exit(1);
 }

 // Make sure we can reuse the port immediately after the
 // server terminates.
 status = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                     (const char *) &on, sizeof(on));
 if(status == -1) {
   perror("setsockopt -- REUSEADDR");
 }

 self.sin_family = AF_INET;
 self.sin_addr.s_addr = INADDR_ANY;
 self.sin_port = htons(PORT);
 memset(&self.sin_zero, 0, sizeof(self.sin_zero));  // Initialize sin_zero to 0

 printf("Listening on %d\n", PORT);

 if (bind(listenfd, (struct sockaddr *)&self, sizeof(self)) == -1) {
   perror("bind"); // probably means port is in use
   exit(1);
 }

 if (listen(listenfd, 5) == -1) {
   perror("listen");
   exit(1);
 }
 return listenfd;
}

/*
* Read and process commands
*/
int process_args(int cmd_argc, char **cmd_argv, struct client *cl, struct client *head,int listenfd) {
	//QNode *qtree = *root;

	if (cmd_argc <= 0) {
		return 0;

	} else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
		/* Return an appropriate value to denote that the specified
		 * user is now need to be disconnected. */
		return -1;

	} else if (strcmp(cmd_argv[0], "do_test") == 0 && cmd_argc == 1) {
		/* The specified user is ready to start answering questions. You
		 * need to make sure that the user answers each question only
		 * once.
		 */
		if (cl->state == 1){
			write(cl->fd, in_tree, strlen(in_tree)+1);
		}else if(cl->state == 0){
			do_test(interests, root, cl);
		}else{
			printf("%d. SHOULDN'T HAPPEN!!!\n", cl->state);
		}
	} else if (strcmp(cmd_argv[0], "get_all") == 0 && cmd_argc == 1) {
		/* Send the list of best mismatches related to the specified
		 * user. If the user has not taked the test yet, return the
		 * corresponding error value (different than 0 and -1).
		 */
		if (cl->state == 0){
			write(cl->fd, not_in_tree, strlen(not_in_tree)+1);
		}else if(cl->state == 1){
			get_all(interests, root, cl);
		}else{
			printf("%d. SHOULDN'T HAPPEN!!!\n", cl->state);
		}

	} else if (strcmp(cmd_argv[0], "post") == 0 && cmd_argc == 3) {
		/* Send the specified message stored in cmd_argv[2] to the user
		 * stored in cmd_argv[1].
		 */
		Client *find;
		char *target_name = cmd_argv[1];
		for(find=top;find;find = find->next){
			if (strcmp(find->name,target_name) == 0){
				break;
			}
		}
		if (find){
			char print_out[BUFSIZE];
			sprintf(print_out, message_prompt,cl->name,cmd_argv[2]);
			write(find->fd, print_out, strlen(print_out)+1);
		}
		else{
			write(cl->fd,no_user_prompt,strlen(no_user_prompt)+1);
		}

	}
	else {
		/* The input message is not properly formatted. */
		//printf("%s, %d, \n",cmd_argv[0],cmd_argc);
		error("Incorrect syntax");
	}
	return 0;
}

/*
* Tokenize the command stored in cmd.
* Return the number of tokens, and store the tokens in cmd_argv.
*/
int tokenize(char *cmd, char **cmd_argv) {
   int cmd_argc = 0;
   char *next_token = strtok(cmd, DELIM);
   while (next_token != NULL) {
       cmd_argv[cmd_argc] = next_token;
 //      printf("%s\n", cmd_argv[cmd_argc]);
       cmd_argc++;
       if (cmd_argc == (INPUT_ARG_MAX_NUM - 1)) {
           cmd_argv[cmd_argc] = strtok(NULL, "");
  //         printf("%s\n", cmd_argv[cmd_argc]);
           cmd_argc++;
           break;
       }
       next_token = strtok(NULL, DELIM);
   }
   return cmd_argc;
}
