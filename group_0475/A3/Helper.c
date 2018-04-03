#include "Helper.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int get_files(char** file_list, char *dir_name){
	int i = 0;
	struct dirent *dir;
	DIR* d;

	d = opendir(dir_name);
	if (d){
		while ((dir = readdir(d)) != NULL){
			if (strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")){
				file_list[i] = malloc(strlen(dir->d_name)+1);
				strcpy(file_list[i], dir->d_name);
				i++;
			}
		}
		closedir (d);
	}
	return i;
}

int power(int base, int power){
	int result = 1;
	for (int i = 0; i<power; i++){
		result = result * base;
	}
	return result;
}

int dna2num(char* dna){
	//convert a DNA subsequence with length 5 to a number from 0 to 1024
	int result = 0;
	int base;
	for (int i = 0; i<5; i++){
		base = power(4,4-i);
		if (dna[i] == 'a' || dna[i] == 'A'){
			result += base*0;
		}else if (dna[i] == 'c' || dna[i] == 'C'){
			result += base*1;
		}else if (dna[i] == 'g' || dna[i] == 'G'){
			result += base*2;
		}else if (dna[i] == 't' || dna[i] == 'T'){
			result += base*3;
		}else{
			result = -1;
			break;
		}
	}
	return result;
}

void num2dna(int index, char *result){
	int base;
	int temp;
	char alphabet[4] = {'a','c','g','t'};
	for (int i = 0; i < 5; i++){
		base = power(4, 4-i);
		temp = index / base;
		result[i] = alphabet[temp];
		index = index % base;
	}
	result[5] = '\0';
}

void process_files(int start, int end, char* path, char* file_list[], int result[]){
	for(int i = start; i < end; i++){
		char filename[MAX_LINE+1] = "";
		strncpy(filename, path, MAX_LINE);
		strcat(filename, "/");
		strcat(filename, file_list[i]);
		FILE *fd = fopen(filename, "r");
		if (fd == NULL){
			fprintf(stderr, "Error opening file %s\n", file_list[i]);
			exit(1);
		}
		char line[MAX_LINE+1];
		while (fgets(line,MAX_LINE,fd) != NULL) {
			line[strcspn(line,"\r\n")] = '\0';
			char temp[6];
			int result_i;
			for (int current = 0; current < (strlen(line)-4); current++){
				for (int iterator = 0; iterator < 5; iterator++){
					temp[iterator] = line[current+iterator];
				}
				temp[5] = '\0';
				result_i = dna2num(temp);
				if (result_i != -1){
					result[result_i]++;
				}
			}
		}
		fclose(fd);
	}
}
void free_list(char *file_list[], int file_num){
	for(int i = 0; i < file_num; i++){
		free(file_list[i]);
	}
	free(file_list);
}
void print_result(char *outfile, int result[]){
	FILE *output;
	if (outfile != NULL){
		output = fopen(outfile, "w");
	}else{
		output = stdout;
	}
	char print_temp[6];
	for(int index = 0; index < 1024; index++){
		if(result[index] != 0){
			num2dna(index,print_temp);
			fprintf(output,"%s,%d\n", print_temp, result[index]);
		}
	}
}
