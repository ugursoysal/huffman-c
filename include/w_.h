// USEFUL FUNCTIONS

void wait(){
	printf("\nPress \"Enter\" to continue.");
	while(getchar() != '\n');
}

void wait_after_scanf(){
	printf("\nPress \"Enter\" to continue.");
	while(getchar() != '\n');
	while(getchar() != '\n');
}

int terminate(char *err){
	printf(err);
	wait(); // error
	return 1;
}

void printlines(int count){
	int i;
	for(i = 0;i<count;i++)
		printf("\n");
}

int power(int c,int e){
	int result = c;
	if(e == 1)
		result = 2;
	else if(e < 1)
		result = 1;
	while(e > 1){
		result *= c;
		e--;
	}
	return result;
}

char *add_char_arrays(char *arr1, size_t size1, char *arr2, size_t size2){
	char *new_array = (char*)malloc((size1+size2)*sizeof(*new_array)); 
	if(new_array != NULL){
	size_t i;
	for(i = 0; i < size1;i++)
		new_array[i] = arr1[i];
	for(i; i < size1+size2;i++)
		new_array[i] = arr2[i-size1];
	new_array[i++] = '\0';
	}
	return new_array;
}

char *add_char_to_array(char *arr1, size_t size, char a){
	char *new_array = (char*)malloc((size+1)*sizeof(*new_array)); 
	if(new_array != NULL){
	size_t i;
	for(i = 0; i < size;i++)
		new_array[i] = arr1[i];
	new_array[i++] = a;
	new_array[i++] = '\0';
	free(arr1);
	}else printf("\nperror");
	return new_array;
}

char *change_char_array_size(char *arr, size_t size, size_t newsize){
	char *new_array = (char*)malloc(newsize*sizeof(*new_array)); 
	if(new_array != NULL){
		for(size_t i = 0; i < size;i++)
			new_array[i] = arr[i];
		for(size_t i = size;i<newsize;i++)
			new_array[i] = 0;
	}else printf("\nkerror");
	free(arr);
	return new_array;
}

int valid_fname(char *filename){
	char bad_chars[] = "!@%^*~|?";
	int i;
	for (i = 0; i < strlen(bad_chars); ++i)
		if (strchr(filename, bad_chars[i]) != NULL)
			return 1;
	return 0;
}