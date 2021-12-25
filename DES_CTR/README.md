## DES file encryptor in CTR mode

### How to start a program

```bash
make
./des <key> <name_of_file> <mode>
```

The 1st argument <key>: your 64-bits key (String of zeros and ones);

The 2nd argument <name_of_file>: name of encrypted file;

The 3rd argument <mode>: mode of program working (1 - encryption; 2 - decryption).

Program adds 33 bytes to the beginning of file. The first 32 bytes are the Inicialisation Vector (IV). The 33rd byte is a number of appended zero bytes.


