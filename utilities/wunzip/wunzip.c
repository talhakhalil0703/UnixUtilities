#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void _decompressStream(char * file_name){
    FILE * input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        fprintf(stdout, "wunzip: cannot open file\n");
        exit(1);
    }
    struct stat buffer;
    if(stat(file_name, &buffer) != 0 ){
        fprintf(stdout, "wunzip: cannot open file\n");
        exit(1);
    }

    int data_read;
    int count;
    char character;

    while (true){
        data_read = fread(&count, 4, 1, input_file);
        if (data_read !=1) break;
        data_read = fread(&character, 1,1,input_file);
        if (data_read !=1) break;
        for (int i =0; i < count; i++){
            putchar(character);
        }
    }

    fclose(input_file);
}

int main(int argc, char* argv[]){
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++){
        _decompressStream(argv[i]);
    }
    exit(0);
}