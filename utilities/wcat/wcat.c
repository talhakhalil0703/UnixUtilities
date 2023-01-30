#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * _printFile - Prints the content of a file
 * @param file_name: the path of the file to be printed
*/
void _printFile(char * file_name){
    FILE * input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        fprintf(stdout, "wcat: cannot open file\n");
        exit(1);
    }
    struct stat buffer;
    if(stat(file_name, &buffer) != 0 ){
        fprintf(stdout, "wcat: cannot open file\n");
        exit(1);
    }

    size_t len = 1000;
    char * line = malloc(len);
    while (fgets(line, len,  input_file) != NULL){
        fprintf(stdout, "%s", line);
    }

    free(line);
    fclose(input_file);
}

int main(int argc, char* argv[]){
    if (argc == 1) exit(0);

    for (int i = 1; i < argc; i++){
        _printFile(argv[i]);
    }
    exit(0);
}