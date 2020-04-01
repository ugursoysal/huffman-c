#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

//#define DEBUG
#define MAX_FILENAME 40
#include "include/w_.h"
#include "include/w_def.h"
#include "include/w_freq.h"
#include "include/w_huffman.h"

/*
int read_file(char *filename, size_t *length, char **content, Frequency** freq_array)
-> opens and reads *filename, stores it in the *content, creates a frequency array and stores it in the *freq_array
1 - error
0 - success
*/
int read_file(char *filename, size_t *length, char **content, Frequency** freq_array){
	FILE *f = NULL;
	if(filename != NULL)f=fopen(filename, "r");
	if(f == NULL){
		fclose(f);
		char str[25+strlen(filename)];
		sprintf(str,"\n%s \"%s\"",lang_array[ERR3],filename);
		return terminate(str);
	}	
	fseek(f, 0, SEEK_END); // go to EOF
	*length = ftell(f); // get length of text file
	rewind(f); // go to the beginning
	Frequency* new_freq_array;
	char* new_content = (char*)malloc(((*length)+1)*sizeof(char));
	if(new_content != NULL){
	int freq_id, i = -1, j = 0;
	char c;
    while((c = fgetc(f)) != EOF){
		new_content[++i] = c;
		if((freq_id = containsChar(new_freq_array,c,array_size)) != -1)new_freq_array[freq_id].freq++;
		else{
			new_freq_array = change_freq_array_size(new_freq_array, array_size, array_size+1);
			new_freq_array[array_size].chr = c;
			new_freq_array[array_size].freq=1;
			array_size++;
		}
	}
	*freq_array = new_freq_array;
    fclose(f);
	*content = new_content;
	}
	return 0;
}

/*
int load_lang()
-> opens and reads the language file in the specified path, stores the texts in lang_array
1 - error
0 - success
*/
int load_lang(){
	FILE *f;
	char *filename = lang_path;
	f = fopen(filename,"rt");
	if(f == NULL){
		fclose(f);
		char str[25+strlen(filename)];
		sprintf(str,"\nCouldn't open the file \"%s\"",filename);
		return terminate(str);
	}
	int a, i = 0;
	char c;
	if((c = fgetc(f)) != '|'){
		fclose(f);
		char str[65+strlen(filename)];
		sprintf(str,"\nFile is corrupted or in wrong format: \"%s\" unexpected character: %c",filename,c);
		return terminate(str);
	}
	while((c = fgetc(f)) != '_' && c != EOF){
		a = 0;
		while(c != '|' && c != '_' && c != EOF){
			lang_array[i] = change_char_array_size(lang_array[i], a, a+1);
			lang_array[i][a] = c;
			a++;
			c = fgetc(f);
		}
		lang_array[i] = change_char_array_size(lang_array[i],a,a+1);
		lang_array[i][a] = '\0';
		i++;
	}
	if(i != COUNT){
		fclose(f);
		char str[42+strlen(filename)];
		sprintf(str,"\nFile is corrupted or in wrong format: \"%s\"",filename);
		return terminate(str);	
	}
	#ifdef DEBUG
	for(i = 0;i<COUNT;i++)
		printf("\nlanguage string %d: %s\n",i,lang_array[i]);
	#endif
	fclose(f);
	return 0;
}

/*
int compress_file(char filename[], int name_size)
-> compresses file in files/input/(filename) and saves the compressed version to files/output/(filename).z
1 - error
0 - success
*/
int compress_file(char filename[], int name_size){
	Frequency *freq_array;
	clock_t tic = clock();
	char *original_string;
	char input[MAX_FILENAME];
	char *filepath;
	size_t length;
	printf("\nenter filename: ");
	scanf("%s",filepath);
	filepath = add_char_arrays(input,strlen(input),filename,name_size);
	printf("\n./%s\n",filepath);
	if(read_file(filepath, &length, &original_string,&freq_array))return 0;
	Frequency tmp[array_size];
	for(int i = 0;i<array_size;i++)tmp[i] = freq_array[i];
	struct Tree *huffman_tree = create_tree(array_size,freq_to_heap(freq_array,array_size),&freq_array,tmp[0].chr,tmp[0].freq);
	free(freq_array);
	freq_array = tmp;
	if(huffman_tree == NULL){
		char str[26+strlen(filename)];
		sprintf(str,"\n%s \"%s\"",lang_array[ERR1],filename);
		return terminate(str);
	}
	struct HuffmanCodes **head = generate_huffman_codes(huffman_tree->array[0],array_size,&freq_array);
	#ifdef DEBUG
	printf("original string: %s",original_string);
	printf("\nOK!\n\n\n\nfreq_array_size: %d\n\n\n",array_size);
	wait();
	#endif
	filepath = add_char_arrays("files/output/",13,filename,name_size);
	filepath = add_char_arrays(filepath,name_size+13,".z",2);
	free(freq_array);
	freq_array = tmp;
	if(compress(original_string,length,filename,name_size,filepath,head,&freq_array,array_size)){
		printf("\n%s",lang_array[ERR2]);
		wait();
		return 1;
	}
	free(original_string);
	free(filepath);
	length = 0;
	#ifdef DEBUG
	for(int i = 0;i<array_size;i++)
		printf("\n- [%d] %c %d",i,freq_array[i].chr,freq_array[i].freq);
	wait();
	#endif
	int c;
	for(c = 0;c<array_size;c++)free(head[c]);
	free(head);
	free_tree(huffman_tree->array[0]);
	free(huffman_tree->array);
	free(huffman_tree);
	free(freq_array);
	array_size = 0;
	clock_t toc = clock();
    printf("\n%f", (double)(toc - tic) / CLOCKS_PER_SEC);
	wait_after_scanf();
	return 0;
}

/*
int compress_file(char filename[], int name_size)
-> extracts file in files/input/(filename) and saves the decompressed version to files/output/(original filename before compression)
1 - error
0 - success
*/
int extract_file(char *filename, int name_size){
	clock_t tic = clock();
	char *original_string;
	char *filepath;
	char *new_file;
	size_t length;
	filepath = add_char_arrays("files/input/",12,filename,name_size);
	printf("\n.%s\n",filepath);
	if(decompress(filepath,&length,&original_string,&new_file))return 0;
	filepath = add_char_arrays("files/output/",13,new_file,strlen(new_file));
	printf("\n\n.%s -> %s\n\n",new_file,filepath);
	FILE *f = fopen(filepath,"wt");
	fwrite(original_string,length,1,f);
	fclose(f);
	free(new_file);
	free(original_string);
	free(filepath);
	length = 0;
	clock_t toc = clock();
    printf("\n%f time spent", (double)(toc - tic) / CLOCKS_PER_SEC);
	wait_after_scanf();
	return 0;
}

/*
int show_about()
-> shows small text about program
1 - exits the program
0 - returns to the main menu
*/
int show_about(){
	system("cls");
	printlines(4);
	printf("\nabout:\nAhmet Ugur Soysal\nCDQ7TE\nBasics of Programming 1 Homework\nBME");
	printf("\n1-%s",lang_array[BACK]);
	printf("\n2-%s",lang_array[EXIT]);
	char c;
	c = getch();
	switch(c - 48){
		case 1:return 0;
		case 2:return 1;
		default: 
			if(c == ESC) return 1;// (or press ESC to exit the program)
			printf("\n%s",lang_array[ERR1]);
			return show_about();
	}
	return 0;
}

/*
int show_main_menu()
-> shows main program options
1 - exits the program
0 - shows main menu again
*/
int show_main_menu(){
	system("cls");
	printlines(4);
	printf(lang_array[WELCOME]);
	printf("\n1-%s",lang_array[COMPRESS]);
	printf("\n2-%s",lang_array[EXTRACT]);
	printf("\n3-%s",lang_array[EXIT]);
	printf("\n4-%s",lang_array[ABOUT]);
	char c;
	c = getch();
	char filename[MAX_FILENAME];
	switch(c - 48){
		case COMPRESS: // get the filename and compress the file
			printf("\n%s",lang_array[FILENAME]);
			while(scanf("%s",&filename) != 1)
				printf("\n%s",lang_array[FILENAME]);
			while(strlen(filename) > MAX_FILENAME-1){
				printf("\ntoo long! (max %d)",MAX_FILENAME-1);
				printf("\n%s",lang_array[FILENAME]);
				scanf("%s",&filename);
			}
			system("cls");
			return compress_file(filename,strlen(filename));
		case EXTRACT:  // get the filename and extract the file
			printf("\n%s",lang_array[FILENAME]);
			while(scanf("%s",&filename) != 1)
				printf("\n%s",lang_array[ERR2]);
			while(strlen(filename) > MAX_FILENAME-1){
				printf("\ntoo long! (max %d)",MAX_FILENAME-1);
				printf("\n%s",lang_array[FILENAME]);
				scanf("%s",&filename);
			}
			system("cls");		
			return extract_file(filename,strlen(filename));
		case EXIT:
			return 1;
		case ABOUT:
			return show_about();
			/*case 5:
			// for debugging
			break;*/
		default: 
			if(c == ESC) return 1; // (or press ESC to exit the program)
			printf("\n%s",lang_array[ERR1]);
			break;
	}
	return 0;
}

int main()
{
	system("cls");
	printlines(4);
	printf("\n1-English");
	printf("\n2-Hungarian (beta)");
	char c;
	while((c = getch())-48 < 1 || c-48 > 2){
		if(c == ESC)return 0;
	};	

	switch(c-48){
		case 2:
		lang_path = lang_path_hu;
		break;
		default:
		lang_path = lang_path_en;
		break;
	}
	system("cls");	

	printf("* loading...");
	if(load_lang())return 0;
	while(!show_main_menu());

	int i;
	for(i = 0;i<COUNT;i++)
		free(lang_array[i]);	
	return 0;
}