#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


/**
 * _openFile - opens the file with the given filename and mode
 * @param file_name: the path of the file to be opened
 * @param opt: the mode in which to open the file (e.g. "r" for read)
 * @return filestream
*/
static FILE * _openFile(char * file_name, char * opt){
    FILE * input_file = fopen(file_name, opt);
    if (input_file == NULL){
        fprintf(stdout, "wgrep: cannot open file\n");
        exit(1);
    }
    struct stat buffer;
    if(stat(file_name, &buffer) != 0 ){
        fprintf(stdout, "wgrep: cannot open file\n");
        exit(1);
    }

    return input_file;
}

/**
 * _searchStream - Searches the Stream for the substring 
 * and prints the line containing the substring
*/
void _searchStream(FILE * stream, char * search_term){
    size_t len = 0;
    char * line = NULL;
    while (getline(&line, &len,  stream) != -1){
        if (strstr(line, search_term) != NULL){
            fprintf(stdout, "%s", line);
        }
    }

    free(line);
}

int main(int argc, char* argv[]){
    char * search_term = argv[1];

    switch (argc) {
        case 1:
            fprintf(stdout, "wgrep: searchterm [file ...]\n");
            exit(1);
            break;
        case 2:
            _searchStream(stdin, search_term);
        default:
        for (int i = 2; i < argc; i++){
            FILE * stream = _openFile(argv[i], "r");
            _searchStream(stream, search_term);
            fclose(stream);
        }
    }

    exit(0);
}