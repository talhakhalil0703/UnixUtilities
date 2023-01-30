// Reverse the lines in a file
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct Node {
    char * data;
    size_t length;
    struct Node * next;
    struct Node * previous;
} Node;

static Node *head_pointer = NULL;
static Node *tail_pointer = NULL;
static FILE * input_stream = NULL;
static FILE * output_stream = NULL;

static void _freeNodes(){
    Node * free_pointer = tail_pointer;
    if (head_pointer !=  NULL) {
        while(free_pointer != NULL){
            free_pointer = tail_pointer->previous;
            free(tail_pointer->data);
            free(tail_pointer);
            tail_pointer = free_pointer;
        }
    }
}

static void _freeWithExitCode(int rc){
    if (input_stream != NULL && input_stream != stdin) {
        fclose(input_stream);
    }
    if (output_stream != NULL && output_stream != stdout) {
        fclose(output_stream);
    }
    _freeNodes();
    exit(rc);
}

static FILE * open_file(char * file_name, char * opt){
    FILE * input_file = fopen(file_name, opt);
    if (input_file == NULL){
        fprintf(stderr, "reverse: cannot open file '%s'\n", file_name);
        _freeWithExitCode(1);
    }
    return input_file;
}

static void _reverseList(){
    Node * previous_reference = NULL;
    Node * current = head_pointer;
    Node * next_reference = NULL;
    tail_pointer = head_pointer;
    while (current != NULL){
        next_reference = current->next;
        current->next = previous_reference;
        current->previous = next_reference;
        previous_reference = current;
        current = next_reference;
    }
    head_pointer = previous_reference;
}

static void _writeNodesToStream(FILE * file){
    Node * free_pointer = head_pointer;
    while(free_pointer != NULL){
        fprintf(file, "%s", free_pointer->data);
        free_pointer = free_pointer->next;
    }
}

static void _readInputFromStream(FILE * input_file){
    char *line = NULL;
    size_t len = 0;
    size_t read;
    bool is_head = true;

    while ((read = getline(&line, &len, input_file)) != -1){
        if (errno == ENOMEM) {
            printf("reverse: malloc failed");
            _freeWithExitCode(1);
        }

        Node * new_node = malloc(sizeof(Node));
        if (errno == ENOMEM) {
            printf("reverse: malloc failed");
            _freeWithExitCode(1);
        }
        
        new_node->data = malloc(sizeof(char)*len);
        if (errno == ENOMEM) {
            printf("reverse: malloc failed");
            _freeWithExitCode(1);
        }

        new_node->length = len;
        strcpy(new_node->data, line);
        if (is_head) {
            head_pointer = new_node;
            tail_pointer = head_pointer;
            is_head = false;
        } else {
            new_node->previous = tail_pointer;
            tail_pointer->next = new_node;
            tail_pointer = new_node;
        }
    }

    free(line);
}

static void _ensureInputOutputAreNotEqual(char* argv[]) {
    struct stat file_one_info;
    struct stat file_two_info;

    if (stat(argv[1], &file_one_info) == -1) {
        fprintf(stderr, "reverse: stat failed\n");
        _freeWithExitCode(1);
    }

    if (stat(argv[2], &file_two_info) == -1) {
        fprintf(stderr, "reverse: stat failed\n");
        _freeWithExitCode(1);
    }

    if (file_one_info.st_ino ==  file_two_info.st_ino){
        fprintf(stderr, "reverse: input and output file must differ\n");
        _freeWithExitCode(1);
    }

    return;
}

int main (int argc, char*argv[]) {
    input_stream = stdin;
    output_stream = stdout;
    switch(argc) {
        case 1:
            break;
        case 2:
            input_stream = open_file(argv[1], "r");
            break;
        case 3:
            input_stream = open_file(argv[1], "r");
            output_stream = open_file(argv[2], "w");
            _ensureInputOutputAreNotEqual(argv);
            break;
        default:
            fprintf(stderr, "usage: reverse <input> <output>\n");
            _freeWithExitCode(1);
            break;
    }
    _readInputFromStream(input_stream);
    _reverseList();
    _writeNodesToStream(output_stream);
    _freeWithExitCode(0);
    return 0;
}