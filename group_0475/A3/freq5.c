#include "freq5.h"
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/time.h>

int main (int argc, char *argv[]){
	int ch;
	int process = 0;
	char *path = NULL;
	char *outfile = NULL;
	struct timeval start, end;
	double timediff;
	if ((gettimeofday(&start,NULL))==-1){
		perror("gettimeofday");
		exit(1);
	}
	while ((ch = getopt(argc,argv,"d:n::o::")) != -1){
		switch(ch){
			case 'd':
				path = malloc(strlen(optarg)+1);
				if (path == NULL){
					perror("malloc");
					exit(EXIT_FAILURE);
				}
				strcpy(path,optarg);
				break;
			case 'o':
				if (optarg != 0){
					outfile = malloc(strlen(optarg)+1);
					if (outfile == NULL){
						perror("malloc");
						exit(EXIT_FAILURE);
					}
					strcpy(outfile,optarg);
				}
				break;
			case 'n':
				if (optarg != 0){
					char *endptr;
					errno = 0;
					long result = strtol(optarg, &endptr,10);
					if (endptr == optarg){
						fprintf(stderr, "No digits were found\n");
						exit(EXIT_FAILURE);
					}
					if ((errno == ERANGE && (result == LONG_MAX || result == LONG_MIN))||
						(errno != 0 && result == 0)){
						perror("strtol");
						exit(EXIT_FAILURE);
					}
					process = atoi(optarg);
				}
				break;
			default:
				fprintf(stderr, "Usage: %s [-d directory] [-n process] [-o output]\n", argv[0]);
                exit(EXIT_FAILURE);
		}
	}
	char** file_list = malloc(sizeof(char*)*512);
	int file_num = get_files(file_list, path);
	if (file_num == 0){
		fprintf(stderr, "Invalid directory name or no file exists in the directory\n");
		free_list(file_list, file_num);
		exit(1);
	}
	int result[1024] = {0};
	if (process > file_num){
		process = file_num;
	}
	if (process != 0){
		int pipes[process][2];
		int sep = file_num/process + 1;
		int child;
		int loop;
		for(loop = 0; loop < process; loop++){
			pipe(pipes[loop]);
			child = fork();
			if (child == 0){
				break;
			}if(child < 0) {
				perror("fork");
				free_list(file_list, file_num);
				exit(1);
			}
		}
		if (child==0){
			close(pipes[loop][0]);
			int end;
			if ((loop+1)*sep > file_num){
				end = file_num;
			}else{
				end = (loop+1) *sep;
			}
			int start = loop*sep;
			process_files(start, end, path, file_list, result);
			for(int i = 0; i < 1024; i++){
				if(write(pipes[loop][1],&result[i],sizeof(result[i])) == -1){
					perror("write to pipe");
					free_list(file_list, file_num);
					exit(1);
				}
			}
			close(pipes[loop][1]);
		}else{
			for(int i = 0; i < process; i++){
				close(pipes[i][1]);
			}
			for(int i = 0; i < process; i++){
				int result_buffer = 0;
				for (int j = 0; j < 1024; j++){
					if(read(pipes[i][0], &result_buffer, sizeof(int)) == -1){
						perror("reading from file");
						free_list(file_list, file_num);
						exit(1);
					}
					result[j] += result_buffer;
				}
				close(pipes[i][0]);
			}
			for(int i = 0; i < process; i++) {
				wait(NULL);
			}
			free_list(file_list, file_num);
			print_result(outfile, result);
			free(path);
			free(outfile);
			if ((gettimeofday(&end,NULL))== -1){
				perror("gettimeofday");
				exit(1);
			}
			timediff = (end.tv_sec - start.tv_sec) + 
			(end.tv_usec - start.tv_usec) / 1000000.0;
			fprintf(stderr, "%.4f\n", timediff);
		}
	}else{//this happens when the -n flag is not used
		process_files(0, file_num, path, file_list, result);
		free_list(file_list, file_num);
		print_result(outfile, result);
		free(path);
		free(outfile);
		if ((gettimeofday(&end,NULL))== -1){
			perror("gettimeofday");
			exit(1);
		}
		timediff = (end.tv_sec - start.tv_sec) + 
		(end.tv_usec - start.tv_usec) / 1000000.0;
		fprintf(stderr, "%.4f\n", timediff);
	}

	return 0;
}


