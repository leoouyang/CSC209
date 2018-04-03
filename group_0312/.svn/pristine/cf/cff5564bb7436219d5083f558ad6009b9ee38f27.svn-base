#include "qtree.h"

int main (int argc, char ** argv) {
    QNode *root = NULL;
	Node *q_list;
    
    if (argc < 2) {
        printf ("To run the program ./test2 <name of input file>\n");
        return 1;
    }
    
    q_list = get_list_from_file (argv[1]);
    
    if (q_list == NULL) {
        printf ("The list is empty\n");
        return 1;
    }
    
    //that is only the first test to test creation of the question tree
    root = add_next_level (root, q_list);
	print_qtree (root, 0);
    //your tests here
    if (argc > 3) {
    	Node *curr = q_list;
    	int list_length = 0;
    	while(curr != NULL){
    		list_length++;
    		curr = curr->next;
    	}
    	int answers[list_length];
    	if ((argc - 3) != list_length){
    		fprintf(stderr, "Warning: The number of answers is incorrect!\n");
    		return 1;
    	}else{
    		for (int i = 3; i < argc; i++){
    			answers[i-3] = strtol(argv[i], NULL, 10);
    		}
    		add_user(root, answers, argv[2]);
    		print_qtree (root, 0);
    	}
    }
    free_list(q_list);
    free_tree(root);
    return 0;
}
