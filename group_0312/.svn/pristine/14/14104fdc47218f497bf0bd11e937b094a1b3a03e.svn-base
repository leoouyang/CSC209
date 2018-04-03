CC = gcc
CFLAGS = -Wall -g -std=c99
all: questions qtree

questions:
	$(CC) $(CFLAGS) questions.c test1.c -o test1
    
qtree:
	$(CC) $(CFLAGS) qtree.c questions.c test2.c -o test2
	
categorizer:
	$(CC) $(CFLAGS) qtree.c questions.c categorizer.c -o categorizer
	
clean_test1:  
	rm test1
	
clean_test2:  
	rm test2
	
clean_categorizer:  
	rm categorizer