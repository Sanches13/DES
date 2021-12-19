#include <stdio.h>
#include <string.h>
#include "permutations.h"
#include "encryption.h"

long int get_file_size(FILE *fp){
    fseek(fp, 0, SEEK_END);
    long int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return file_size;
}

int main(int argc, char* argv[]) {

    if(argc != 4){
        printf("Please, use format: %s <key> <file> <mode>\n", argv[0]);
        return -1;
    }

    if((strlen(argv[1])) != 64){
        printf("Error length of key!\n");
        return -1;
    }

    if(((int) (argv[3][0] - 0x30) != 1) && ((int) (argv[3][0] - 0x30) != 2)){
        printf("Error value of mode!\n");
        return -1;
    }

    FILE *fp;
    if((fp = fopen(argv[2], "r")) == NULL){
        printf("Error when opening input file\n");
        return -1;
    }

    long int file_size = get_file_size(fp);
    if(file_size == 0){
        printf("Your input file is empty!\n");
        fclose(fp);
        return -1;
    }

    switch ((int) (argv[3][0] - 0x30)) {
        case 1:
            if((file_encryption(fp, file_size, argv[1])) != 0)
                return -1;
            break;
        case 2:
            if((file_decryption(fp, file_size, argv[1])) != 0)
                return -1;
            break;
    }

    fclose(fp);
    return 0;
}
