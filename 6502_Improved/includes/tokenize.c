#include "tokenize.h"

//Local function to read file size for dynamic allocation
static size_t size_of_file(FILE* file) {
    if (file == NULL)
        return -1;
    
    fseek(file, 0, SEEK_END);
    size_t sz = ftell(file);
    fseek(file, 0, SEEK_SET);

    return sz + 1;
}

//For this we need some reserverd keywords to interpret
//Which for most of them will just be the instructions
static Tokens keywords[] = {
    {LDA_, "LDA"}, {LDX_, "LDX"}, {LDY_, "LDY"},
    {STA_ ,"STA"}, {STX_ ,"STX"}, {STY_, "STY"},
    {ADC_, "ADC"}, {INC_, "INC"}, {DEC_, "DEC"},
    {ASL_, "ASL"}, {AND_, "AND"}, {CMP_, "CMP"},
    {CPX_, "CPX"}, {CPY_, "CPY"}, {JMP_, "JMP"},
    {ROL_, "ROL"}, {INX_, "INX"}, {INY_, "INY"},
    {DEX_, "DEX"}, {DEY_, "DEY"}, {TAX_, "TAX"},
    {TXA_, "TXA"}, {TAY_, "TAY"}, {TYA_, "TYA"},
    {TXS_, "TXS"}, {TSX_, "TSX"}, {PHA_, "PHA"},
    {PLA_, "PLA"}, {NOP_, "NOP"}, {SEC_, "SEC"},
    {SED_, "SED"}, {SEI_, "SEI"}, {CLC_, "CLC"},
    {CLD_, "CLD"}, {CLI_, "CLI"},
};

//Need to tokenize the file and return tokenized list
void tokenize_file(char* path) {
    //Initialize the tokens list
    Token_List* tokens = initList();

    //Reading file buffer
    FILE* fasm = fopen(path, "r");

    //File not found
    if (fasm == NULL) {
        fprintf(stderr, "[-] ERROR: 6502asm could not find file '%s'...\n", path);
        exit(-1);
    }

    //Init vars
    char ch;
    int index = 0;

    //Dynamic allocation incremented twice. Once for EOF token and once for the null byte
    char* textbuffer = (char*)malloc(size_of_file(fasm) + 1 * sizeof(char));

    //Allocate the buffer
    while ((ch = fgetc(fasm)) != EOF)
        textbuffer[index++] = (char)ch;

    //EOF
    textbuffer[index] = EOF_;
    
    //Restate the index
    index = 0;
    ch = textbuffer[index];

    //Store token
    ASM6502_TOKENS token = UNKNOWN_;

    //While 'ch' is not at the end of file
    //This is where the tokenization process will occur
    while (ch != EOF_) {
        //Get character at index
        ch = textbuffer[index];

        //Check if character is alphabetical (instruction names)
        if (isalpha(ch)) {
            //(Temp for now) Store buffer
            char currentTokString[16];

            int i = 0;
            while (isalpha(ch) || isdigit(ch)) {
                //Store character at buffer
                currentTokString[i++] = ch;
                //Increment character
                ch = textbuffer[++index];
            }
            currentTokString[i] = '\0';
            //Check for the instruction keyword
            for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
                //Push the token to the list
                if (!strncmp(currentTokString, keywords[i].value, 16))
                    add_tok_l(tokens, keywords[i]);
            }
        }
        else {
            switch (ch) {
                case '#':
                    ch = textbuffer[++index];
                    Tokens hashtok = {HASHTAG_, "#"};
                    add_tok_l(tokens, hashtok);
                    break;
                default:
                    index++;
                    break;
            }
        }
    }

    #pragma region TOKEN PRINTING DEBUGGING
    printf("\n===========================\n");

    Token_Node* cur = tokens->head;
        
    while (cur != NULL) {
        printf("%s\n", cur->data.value);
        cur = cur->next;
    }

    printf("===========================\n");
    #pragma endregion

    //Deallocate
    fclose(fasm);
    free_tok_l(tokens);
    free(textbuffer);

    exit(0);
}

//Linked List functionalities
//Init list
Token_List* initList() {
    Token_List* list = (Token_List*)malloc(sizeof(Token_List));

    if (!list)
        return NULL;
    list->head = NULL;
    return list;
}
//Init node
Token_Node* initNode(Tokens token) {
    Token_Node* node = (Token_Node*)malloc(sizeof(Token_Node));

    if (!node)
        return NULL;

    node->data = token;
    node->next = NULL;
    return node;
}
//Add to list
void add_tok_l(Token_List* list, Tokens data) {
    Token_Node* cur = NULL;
    if (list->head == NULL)
        list->head = initNode(data);
    else {
        cur = list->head;
        while (cur->next != NULL)
            cur = cur->next;
        
        cur->next = initNode(data);
    }
}
//Push to list
void push_tok_l(Token_List* list, Tokens data) {
    Token_Node* tmp = initNode(data);
    tmp->next = list->head;
    list->head = tmp;
}
//Pop from list
int pop_tok_l(Token_List* list) {
    if (list->head == NULL)
        return -1;

    Token_Node* h = list->head;
    list->head = list->head->next;

    free(h);
    return 0;
}
//Free list
void free_tok_l(Token_List* list) {
    Token_Node* cur = list->head;
    Token_Node* next = cur;

    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }

    free(list);
}