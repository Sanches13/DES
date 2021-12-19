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

    if((key_generation(argv[1])) != 0)
        return -1;

    FILE *fp1, *fp2;
    if((fp1 = fopen(argv[2], "r")) == NULL){
        printf("Error when opening input file\n");
        return -1;
    }

    long int file_size = get_file_size(fp1);
    if(file_size == 0){
        printf("Your input file is empty!\n");
        fclose(fp1);
        return -1;
    }

    if((fp2 = fopen("encrypt_file", "w")) == NULL){
        printf("Error when creating output file\n");
        fclose(fp1);
        return -1;
    }

    unsigned char text[8];
    unsigned char open_block[64];
    unsigned char IV[64] = {0};
    unsigned char round_IV[64];
    long int num_of_blocks;

    if((int) (argv[3][0] - 0x30) == 1) {
        if((generate_IV(IV) == -1)) {
            fclose(fp1);
            fclose(fp2);
            return -1;
        }

        put_IV_in_file(fp2, IV);
        fprintf(fp2, "%ld", (8 - (file_size % 8)) % 8);
    }

    else if((int) (argv[3][0] - 0x30) == 2){
        for(int i = 0; i < 32; i++)
            fscanf(fp1, "%c", &IV[i]);

        unsigned char elem;
        fscanf(fp1, "%c", &elem);
        file_size = file_size - 33 - (elem - 0x30);
    }

    else{
        printf("Error value of mode!\n");
        fclose(fp1);
        fclose(fp2);
        return -1;
    }

    (file_size % 8 == 0) ? (num_of_blocks = file_size / 8) : (num_of_blocks = (file_size / 8) + 1);

    printf("IV: ");
    for(int i = 0; i < 32; i++)
        printf("%d", IV[i]);
    printf("\n");

    for(int k = 0; k < num_of_blocks; k++){
        memset(text, 0, 8);

        for(int j = 0; j < 8 && j < file_size; j++)
            fscanf(fp1, "%c", &text[j]);

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                open_block[j + i*8] = get_bit(text[i], 7 - j);

        memcpy(round_IV, IV, 64);

        encryption(open_block, round_IV, text);

        if((int) (argv[3][0] - 0x30) == 2){
            for(int i = 0; i < 8 && (i + k * 8) < file_size; i++)
                fprintf(fp2, "%c", text[i]);
        }
        else{
            for(int i = 0; i < 8; i++)
                fprintf(fp2, "%c", text[i]);
        }

        increase_IV(IV);
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}