// FREQUENCY FUNCTIONS & DEFINITION

typedef struct Frequency {
    char chr;
    int freq;
} Frequency;

int containsChar(Frequency *arr, char ch, size_t array_size){
	for(int c=0;c<array_size;c++){
		if(arr[c].freq != 0 && arr[c].chr == ch)
			return c;
	}
	return -1;
}

Frequency *read_fix_array(Frequency *arr, size_t size){
	Frequency *new_array = (Frequency*)malloc(size*sizeof(*new_array)); 
	for(size_t i = 1; i < size+1;i++)
		new_array[i-1] = arr[i];
	free(arr);
	return new_array;
}

Frequency *fix_array(Frequency *arr, size_t size){
	Frequency *new_array = (Frequency*)malloc((size+1)*sizeof(*new_array)); 
	new_array[0].chr = '_';
	new_array[0].freq = 0;
	for(size_t i = 1; i < size+1;i++)
		new_array[i] = arr[i-1];
	free(arr);
	return new_array;
}

Frequency *change_freq_array_size(Frequency *arr, size_t size, size_t newsize){
	Frequency *new_array = (Frequency*)malloc(newsize*sizeof(*new_array)); 
	for(size_t i = 0; i < size;i++)
		new_array[i] = arr[i];
	free(arr);
	return new_array;
}

Frequency *sort_freq_array(Frequency *arr, size_t size){
	Frequency *new_array = (Frequency*)malloc(size*sizeof(*new_array)); 
	int i = 0, j = 0, x = 0;
	int curr_size = 1;
	for(i = 0;i<size;i++)new_array[i].freq = 0;
	for(i = 0;i<size;i++){
		for(x = 0;x<size;x++){
			if(x == curr_size)break;
			if(new_array[x].freq <= arr[i].freq){
				for(j = curr_size-1;j>x;j--)new_array[j] = new_array[j-1];
				new_array[x] = arr[i];
				curr_size++;
				break;
			}
		}
	}
	#ifdef DEBUG
	for(x=0;x<size;x++)printf("\n[%d] sort_freq_array char: %c | freq: %d",x, new_array[x].chr,new_array[x].freq);
	#endif
	free(arr);
	return new_array;
}