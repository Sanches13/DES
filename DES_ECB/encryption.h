#include <string.h>
unsigned char B_blocks[8][6];
unsigned char B_blocks_decreased[8][4];
unsigned char C[17][28];
unsigned char D[17][28];
unsigned char sub_key[17][48];
unsigned char key[64];
unsigned char text[8];
unsigned char open_block[64];
long int num_of_blocks;

unsigned char get_bit(unsigned char input, int bit_number){
    return ((input & (1 << bit_number)) >> bit_number);
}

unsigned char set_bit(unsigned char input, int bit_number){
    return input | (1 << bit_number);
}

void initial_permutation(unsigned char *input){
    unsigned char current[64] = {0};

    for(int i = 0; i < 64; i++)
        current[i] = input[IP[i] - 1];

    memcpy(input, current, 64);
}

void C_block_generation(){
    for(int i = 0; i < 28; i++)
        C[0][i] = key[C_Permutation[i] - 1];

    for(int i = 1; i < 17; i++){
        memcpy(C[i], C[i - 1], 28);

        if(key_shifts[i - 1] == 1){
            unsigned char temp = C[i][0];
            for(int j = 0; j < 27; j++)
                C[i][j] = C[i][j + 1];
            C[i][27] = temp;
        }

        else{
            unsigned char temp1 = C[i][0];
            unsigned char temp2 = C[i][1];
            for(int j = 0; j < 26; j++)
                C[i][j] = C[i][j + 2];
            C[i][26] = temp1;
            C[i][27] = temp2;
        }
    }
}

void D_block_generation(){
    for(int i = 0; i < 28; i++)
        D[0][i] = key[D_Permutation[i] - 1];

    for(int i = 1; i < 17; i++){
        memcpy(D[i], D[i - 1], 28);

        if(key_shifts[i - 1] == 1){
            unsigned char temp = D[i][0];
            for(int j = 0; j < 27; j++)
                D[i][j] = D[i][j + 1];
            D[i][27] = temp;
        }

        else{
            unsigned char temp1 = D[i][0];
            unsigned char temp2 = D[i][1];
            for(int j = 0; j < 26; j++)
                D[i][j] = D[i][j + 2];
            D[i][26] = temp1;
            D[i][27] = temp2;
        }
    }
}

void decryption_C_block_generation(){
    for(int i = 0; i < 28; i++)
        C[0][i] = key[C_Permutation[i] - 1];

    for(int i = 1; i < 17; i++){
        memcpy(C[i], C[i - 1], 28);

        if(key_shifts[i - 1] == 1){
            unsigned char temp = C[i][0];
            for(int j = 0; j < 27; j++)
                C[i][j] = C[i][j + 1];
            C[i][27] = temp;
        }

        else{
            unsigned char temp1 = C[i][0];
            unsigned char temp2 = C[i][1];
            for(int j = 0; j < 26; j++)
                C[i][j] = C[i][j + 2];
            C[i][26] = temp1;
            C[i][27] = temp2;
        }
    }
}

void decryption_D_block_generation(){
    for(int i = 0; i < 28; i++)
        D[0][i] = key[D_Permutation[i] - 1];

    for(int i = 1; i < 17; i++){
        memcpy(D[i], D[i - 1], 28);

        if(key_shifts[i - 1] == 1){
            unsigned char temp = D[i][0];
            for(int j = 0; j < 27; j++)
                D[i][j] = D[i][j + 1];
            D[i][27] = temp;
        }

        else{
            unsigned char temp1 = D[i][0];
            unsigned char temp2 = D[i][1];
            for(int j = 0; j < 26; j++)
                D[i][j] = D[i][j + 2];
            D[i][26] = temp1;
            D[i][27] = temp2;
        }
    }
}

void sub_key_generation(int key_number, unsigned char *key){
    unsigned char CD[56];
    for(int i = 0; i < 28; i++) {
        CD[i] = C[key_number + 1][i];
        CD[i + 28] = D[key_number + 1][i];
    }

    for(int i = 0; i < 48; i++)
        key[i] = CD[key_permutation[i] - 1];
}

int key_generation(char* your_key){
    for(int i = 0; i < 64; i++) {
        key[i] = your_key[i] - 0x30;
        if(key[i] != 1 && key[i] != 0) {
            printf("Error key value!\n");
            return -1;
        }
    }

    C_block_generation();
    D_block_generation();
    for(int i = 0; i < 16; i++)
        sub_key_generation(i, sub_key[i + 1]);
    return 0;
}

int decryption_key_generation(char* your_key){
    for(int i = 0; i < 64; i++) {
        key[i] = your_key[i] - 0x30;
        if(key[i] != 1 && key[i] != 0) {
            printf("Error key value!\n");
            return -1;
        }
    }

    decryption_C_block_generation();
    decryption_D_block_generation();
    for(int i = 0; i < 16; i++)
        sub_key_generation(i, sub_key[i + 1]);

    unsigned char temp[48];
    for(int i = 1; i < 9; i++){
        memcpy(temp, sub_key[i], 48);
        memcpy(sub_key[i], sub_key[17 - i], 48);
        memcpy(sub_key[17 - i], temp, 48);
    }
    return 0;
}

void E_function(unsigned char *R_block, unsigned char *extended_R_block){
    for(int i = 0; i < 48; i++)
        extended_R_block[i] = R_block[E[i] - 1];
}

void generate_decreased_B_blocks(){
    unsigned char a, b;
    for(int i = 0; i < 8; i++){
        a = 0;
        b = 0;
        if(B_blocks[i][0] == 1)
            a = set_bit(a, 0);
        a <<= 1;
        if(B_blocks[i][5] == 1)
            a = set_bit(a, 0);

        for(int j = 1; j < 5; j++){
            if(j != 1)
                b <<= 1;
            if(B_blocks[i][j] == 1)
                b = set_bit(b, 0);
        }

        b = S[i][a * 16 + b];

        for(int j = 0; j < 4; j++)
            B_blocks_decreased[i][j] = get_bit(b, 3 - j);
    }
}

void main_encryption_function(unsigned char *R_block, int R_block_num){
    unsigned char extended_R_block[48];
    E_function(R_block, extended_R_block);
    for(int i = 0; i < 48; i++)
        extended_R_block[i] = (extended_R_block[i] + sub_key[R_block_num + 1][i]) % 2;

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 6; j++)
            B_blocks[i][j]= extended_R_block[j + i*6];

    generate_decreased_B_blocks();

    for(int i = 0; i < 32; i++)
        R_block[i] = B_blocks_decreased[(P[i] - 1) / 4][(P[i] - 1) % 4];
}

void final_permutation(unsigned char *block){
    unsigned char current[64];
    for(int i = 0; i < 64; i++)
        current[i] = block[final_message_permutation[i] - 1];

    memcpy(block, current, 64);
}

void encryption(unsigned char *open_block, unsigned char *cipher_text){
    initial_permutation(open_block);

    unsigned char L[17][32], R[17][32];
    for(int i = 0; i < 32; i++){
        L[0][i] = open_block[i];
        R[0][i] = open_block[i + 32];
    }

    for(int i = 1; i < 17; i++){
        for(int j = 0; j < 32; j++)
            L[i][j] = R[i - 1][j];

        main_encryption_function(R[i - 1], i - 1);
        for(int j = 0; j < 32; j++){
            R[i][j] = (L[i - 1][j] + R[i - 1][j]) % 2;
        }
    }

    unsigned char cipher[64];
    for(int i = 0; i < 32; i++){
        cipher[i] = R[16][i];
        cipher[i + 32] = L[16][i];
    }

    final_permutation(cipher);

    memset(cipher_text, 0, 8);

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(cipher[j + i * 8] == 1)
                cipher_text[i] = set_bit(cipher_text[i], 7 - j);
}

int file_encryption(FILE *input, long int file_size, char *your_key){

    if((key_generation(your_key)) != 0)
        return -1;

    FILE *output;
    if((output = fopen("encrypt_file", "w")) == NULL){
        printf("Error when creating output file\n");
        fclose(output);
        return -1;
    }

    fprintf(output, "%ld", (8 - (file_size % 8)) % 8);

    (file_size % 8 == 0) ? (num_of_blocks = file_size / 8) : (num_of_blocks = (file_size / 8) + 1);

    for(int k = 0; k < num_of_blocks; k++){
        memset(text, 0, 8);

        for(int j = 0; j < 8 && j < file_size; j++)
            fscanf(input, "%c", &text[j]);

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                open_block[j + i*8] = get_bit(text[i], 7 - j);

        encryption(open_block, text);

        for(int i = 0; i < 8; i++)
            fprintf(output, "%c", text[i]);
    }
    fclose(output);
    return 0;
}

int file_decryption(FILE *input, long int file_size, char *your_key){

    if((decryption_key_generation(your_key)) != 0)
        return -1;

    FILE *output;
    if((output = fopen("decrypt_file", "w")) == NULL){
        printf("Error when creating output file\n");
        fclose(output);
        return -1;
    }

    unsigned char elem;
    fscanf(input, "%c", &elem);
    file_size = file_size - 1 - (elem - 0x30);

    (file_size % 8 == 0) ? (num_of_blocks = file_size / 8) : (num_of_blocks = (file_size / 8) + 1);

    for(int k = 0; k < num_of_blocks; k++){
        memset(text, 0, 8);

        for(int j = 0; j < 8 && j < file_size; j++)
            fscanf(input, "%c", &text[j]);

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                open_block[j + i*8] = get_bit(text[i], 7 - j);

        encryption(open_block, text);

        for(int i = 0; i < 8 && (i + k * 8) < file_size; i++)
            fprintf(output, "%c", text[i]);
    }
    fclose(output);
    return 0;
}