// STRUCTURE MODELS & DEFINITIONS

#define ESC 27 // escape
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
  
enum LanguageConfiguration {
WELCOME,
COMPRESS,
EXTRACT,
EXIT,
ABOUT,
BACK,
CANCEL,
FILENAME,
INFO1, // "compressing"
INFO2, // "decompressing",
INFO3, // "done",
ERR1, // fail
ERR2, // "wrong input",
ERR3, // file error
COUNT
};
char *lang_array[COUNT];
char *lang_path_en = "lang/en.cfg";
char *lang_path_hu = "lang/hu.cfg";
char *lang_path;


struct Node {
    char chr;
    int freq;
	struct Node *left, *right;
};

struct Tree {
	size_t size;
	struct Node **array;
};

size_t array_size = 0;

struct HuffmanCodes {
	char chr;
	int code;
	int buffsize;
};

struct Header {
	size_t name_size;
	int last_char, file_length, array_size;
};