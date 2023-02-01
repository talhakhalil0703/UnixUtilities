#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static int c;
static int current_character;
static int current_character_count;
static bool first_character = true;

void _compressStream(char * file_name){
    FILE * input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        fprintf(stdout, "wzip: cannot open file\n");
        exit(1);
    }
    struct stat buffer;
    if(stat(file_name, &buffer) != 0 ){
        fprintf(stdout, "wzip: cannot open file\n");
        exit(1);
    }


    while (true){
        c = fgetc(input_file);
        if (c == EOF) break;
        if (c == current_character){
            current_character_count += 1;
        } else {
            if (first_character) {
                first_character = false;
            } else {
                //Output character
                fwrite(&current_character_count, 4,1, stdout);
                printf("%c", current_character);
            }
            current_character_count = 1;
            current_character = c;
        }
    }
    fclose(input_file);
}

int main(int argc, char* argv[]){
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++){
        _compressStream(argv[i]);
    }

    fwrite(&current_character_count, 4,1, stdout);
    printf("%c", current_character);
    exit(0);
}