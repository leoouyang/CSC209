#ifndef HELPER_H_
#define HELPER_H_
#define MAX_LINE 256

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

int get_files(char** file_list, char *dir_name);
int power(int base, int power);
int dna2num(char* dna);//return -1 if illegal character is in DNA
void num2dna(int index, char *result);
void process_files(int start, int end, char* path, char* file_list[], int result[]);
void print_result(char *outfile, int result[]);
void free_list(char *file_list[], int file_num);

#endif
