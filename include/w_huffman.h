// HUFFMAN TREE AND CODING FUNCTIONS

char *get_code(int a, int buff_size) {
	char *buffer = (char*)malloc((buff_size)*sizeof(*buffer)); 
	if(buffer != NULL){
	int c;
	for(c = buff_size-1;c>=0;c--){
	if(a >= power(2,c)){
		a-=power(2,c);
		buffer[buff_size-c-1] = '1';
	}else
		buffer[buff_size-c-1] = '0';
	}
	buffer[buff_size] = '\0';
    }
	return buffer;
}

/*#ifdef DEBUG
char *code_from_character(struct HuffmanCodes **array, size_t size, char chr){
	int i;
	for(i=0;i<size;i++){
		if(array[i]->chr == chr)return get_code(array[i]->code,array[i]->buffsize);
	}
	return NULL;
}
#endif*/
struct Node *create_node(char chr, int freq, struct Node *left, struct Node *right) 
{ 
    struct Node *newnode = (struct Node*)malloc(sizeof(*newnode)); 
	if(newnode != NULL){
		newnode->left = left;
		newnode->right = right;
		newnode->chr = chr;
		newnode->freq = freq;	
	}
    return newnode; 
}
 
struct Tree *freq_to_heap(Frequency *arr,size_t size){ // frequency array to min heap array
	struct Tree *newtree =  (struct Tree*)malloc(sizeof(*newtree));
	if(newtree != NULL){
		int x;
		newtree->size = size;
		newtree->array = (struct Node**)malloc(sizeof(struct Node*)*size); 
		arr = sort_freq_array(arr,size);
		for(x=0;x<size;x++)
			newtree->array[x] = create_node(arr[size-x-1].chr,arr[size-x-1].freq, NULL, NULL);
	}
	return newtree;
}

struct Node *get_min_node(struct Tree* tree){
	struct Node *res_node = NULL;
	int i, res_index;
	int result = 9999999;
	for(i=0;i<tree->size;i++){
		if(tree->array[i]->freq > 0 && result > tree->array[i]->freq){
			result = tree->array[i]->freq;
			if(res_node != NULL) free(res_node);
			res_node = create_node(tree->array[i]->chr,tree->array[i]->freq,tree->array[i]->left,tree->array[i]->right);
			res_index = i;
		}
	}
	tree->array[res_index]->freq = 0;
	tree->array[res_index]->chr = '*';
	return res_node;
}

struct Tree *add_to_tree(struct Tree* tree, struct Node *newnode){

	struct Tree *new_tree =  (struct Tree*)malloc(sizeof(*new_tree)); 
	
	if(new_tree != NULL){
		new_tree->size = tree->size-1;
		new_tree->array = (struct Node**)malloc(sizeof(struct Node*)*(new_tree->size)); 
		int i, k = 0;
		for(i=0;i<tree->size;i++){
			if(tree->array[i]->freq < 1) {
				k++;
				free(tree->array[i]);
			}
			else if(newnode != NULL && newnode->freq < tree->array[i]->freq){
				new_tree->array[i-k] = create_node(newnode->chr,newnode->freq,newnode->left,newnode->right);
				k--;
				newnode = NULL;
				new_tree->array[i-k] = create_node(tree->array[i]->chr,tree->array[i]->freq,tree->array[i]->left,tree->array[i]->right);
			}
			else
				new_tree->array[i-k] = create_node(tree->array[i]->chr,tree->array[i]->freq,tree->array[i]->left,tree->array[i]->right);
			free(tree->array[i]);
		}
		if(newnode != NULL)
			new_tree->array[i-k] = create_node(newnode->chr,newnode->freq,newnode->left,newnode->right);
		free(tree->array);
		free(tree);
	}
	return new_tree;
}

struct Tree* create_tree(size_t size, struct Tree* prev, Frequency **arr, char firstchar, int freq){
	if(prev == NULL)
		return NULL;
	Frequency *freq_array = (Frequency*)malloc(sizeof(Frequency)*array_size);
	Frequency *temp = *arr;
	int i;
	for(i = 0;i<array_size;i++)
		freq_array[i] = temp[i];	
	free(arr);
	struct Tree *new_tree = prev;
	struct Node *newnode;
	new_tree->size = size;
	for(i=0;i<size-1;i++){
		newnode = create_node('*',0,NULL,NULL);
		newnode->left = get_min_node(new_tree);
		newnode->right = get_min_node(new_tree);
		newnode->freq = newnode->left->freq+newnode->right->freq;
		new_tree = add_to_tree(new_tree,newnode);
		//wait();
	}
	free(prev);
	freq_array[0].chr = firstchar;
	freq_array[0].freq = freq;
	*arr = freq_array;
	return new_tree;
}

int index_of_character(struct HuffmanCodes **array, size_t size, char chr){
	int i;
	for(i=0;i<size;i++){
		if(array[i]->chr == chr)
			return i;
	}
	return -1;
}

void *traverse_tree(struct HuffmanCodes **array, struct Node *node,int buff, int buff_size) {
	buff_size++;
	if(node != NULL) {
		traverse_tree(array,node->left, (buff*2),buff_size);
		traverse_tree(array,node->right,(buff*2) + 1,buff_size);
		if(node->chr != '*'){
			struct HuffmanCodes *newcode = (struct HuffmanCodes*) malloc(sizeof(struct HuffmanCodes));
			if(newcode != NULL){				
			newcode->chr = node->chr;
			newcode->code = buff;
			newcode->buffsize = buff_size;
			array[--array_size] = newcode;
			}
		}
	}
}

void free_tree(struct Node *node) {
	if(node != NULL) {
		free_tree(node->left);
		free_tree(node->right);
		free(node);
	}
}

struct HuffmanCodes **generate_huffman_codes(struct Node *head,size_t size, Frequency **arr){
	Frequency *freq_array = (Frequency*)malloc(sizeof(Frequency)*array_size);
	Frequency *temp = *arr;
	int i;
	for(i = 0;i<array_size;i++)
		freq_array[i] = temp[i];	
	free(arr);
	struct HuffmanCodes **new_codes = (struct HuffmanCodes**)malloc(size*sizeof(struct HuffmanCodes*));
	if(new_codes != NULL){
	traverse_tree(new_codes,head,0,-1);
	array_size = size;
	}
	*arr = freq_array;
	return new_codes;
}

void read_header(FILE *f, char **filename, Frequency **array, size_t *array_size, int *file_length, char *last_char){
	struct Header header;
	fread(&header,sizeof(struct Header),1,f); // read header
	char *fname = (char*)malloc((header.name_size)*sizeof(char));
	fread(fname,sizeof(char),header.name_size,f);
	fname[header.name_size] = '\0';
	*filename = fname;
	*last_char = header.last_char;
	*file_length = header.file_length;
	*array_size = header.array_size;
	Frequency *arr = (Frequency*)malloc(sizeof(*arr)*(*array_size)); 
	if(arr != NULL){
	int i;
	for(i = 0;i<*array_size;i++){
		fread(&arr[i].chr,sizeof(char),1,f);
		fread(&arr[i].freq,sizeof(int),1,f);
	}
	*array = arr;
	}
}

void create_header(FILE *f, char filename[], size_t fname_size, Frequency **array, size_t array_size, int file_length, char last_char){
	struct Header header = { fname_size, last_char, file_length, array_size };
	int i;
	for(i = 0;i<fname_size;i++){
		if(filename[i] == '?'){
			filename[i] = '\0';
			break;
		}
	}
	Frequency* arr = *array;
	fwrite(&header,sizeof(struct Header),1,f);
	fwrite(filename,sizeof(char),fname_size,f);
	for(i = 0;i<array_size;i++){
		fwrite(&arr[i].chr,sizeof(char),1,f);
		fwrite(&arr[i].freq,sizeof(int),1,f);
	}
}

int compress(char *context, size_t length, char filename[], size_t name_size, char *filepath, struct HuffmanCodes **array, Frequency **freq_array, size_t size){
	FILE *f;
	Frequency *a = *freq_array;
	f=fopen(filepath, "wb");
	if(f == NULL){
		fclose(f);
		char str[25+strlen(filename)];
		sprintf(str,"\n%s \"%s\"",filename);
		return terminate(str);
	}else{
	char *getcode;
	char buffer;
	int fullness = 0;
	int i = 0, c, b;
	create_header(f, filename, name_size, freq_array, array_size, length, context[length-1]); 
	i = 0;
	while(i != length){
		b = 0;
		if((c = index_of_character(array,size,context[i])) < 0 || c > array_size){
				i++;
				if(context[i] != 0) printf("error: generating code for character '%c' (%d) failed",context[i],context[i]);
				continue;
		}
		getcode = get_code(array[c]->code,array[c]->buffsize);
		#ifdef DEBUG
		printf("\n|char: %c -> code: %s f: %d",context[i],getcode,fullness);
		#endif
		while(b < array[c]->buffsize){
			buffer *= 2;
			fullness++;
			if(getcode[b] == '1')
				buffer += 1;
			if(fullness == 8){
				//system("cls");
				//printf("%s [%d/%d]",lang_array[INFO1],i,length); // too slow
				#ifdef DEBUG
				printf("\ni:%d b: %d, fullnes: %d, buffer: %d, code: %c%c%c%c%c%c%c%c",i,b,fullness,buffer,BYTE_TO_BINARY(buffer));
				#endif
				fputc(buffer,f);
				fullness = 0;
				buffer = (buffer % 2 == 0) ? 0 : 1;
			}																												
			b++;
		}
		free(getcode);
		i++;
	}
	if(fullness != 0)
		fputc((buffer << (8-b)),f);
	fputc('\0',f);
	fclose(f);
	}
	printf("\n%s\n./files/output/%s.z\n\n",lang_array[INFO3],filename);
	return 0;
}

int decompress(char *filename, size_t *length, char **content, char **new_file){
	Frequency *freq_array;
	FILE *f;
	f=fopen(filename, "rb");
	if(f == NULL){
		fclose(f);
		char str[25+strlen(filename)];
		sprintf(str,"\n%s \"%s\"",lang_array[ERR3],filename);
		return terminate(str);
	}
	char last;
	char *n_file;
	read_header(f, &n_file, &freq_array, &array_size, length, &last);
	*new_file = n_file;
	char *new_content = (char*)malloc(sizeof(char)*(*length));
	if(new_content != NULL){
	int a;
	Frequency tmp[array_size];
	for(int i = 0;i<array_size;i++)tmp[i] = freq_array[i];
	struct Tree *huffman_tree = create_tree(array_size,freq_to_heap(freq_array,array_size),&freq_array,tmp[0].chr,tmp[0].freq);
	free(freq_array);
	freq_array = tmp;
	if(huffman_tree == NULL){
		fclose(f);
		char str[26+strlen(filename)];
		sprintf(str,"\n%s \"%s\"",lang_array[ERR1],filename);
		return terminate(str);
	}
	int j = 0, i = 0;
	char c = '*';
	struct Node *current = huffman_tree->array[0];
    while(i != (*length)-1){
		c = fgetc(f);
		j = 7;
		while(j>-1){
			if(c & power(2,j)){
				current = current->right;
			}else
				current = current->left;
			j--;
			if(current->chr != '*'){
			/*if(current->chr == '\0')new_content[i] = '\n';
			else */
			new_content[i] = current->chr;
			//system("cls");
			//printf("%s [%d/%d]",lang_array[INFO2],i,*length); // too slow
			current = huffman_tree->array[0];
			i++;
			}
		}
	}
	new_content[i] = last;
	*content = new_content;
	free_tree(huffman_tree->array[0]);
	free(huffman_tree->array);
	free(huffman_tree);
	}
	free(freq_array);
    fclose(f);
	return 0;
}