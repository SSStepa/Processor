#include <stdio.h>
#include "basic.h"

const int FILE_ELEMS = 3492;
const int FILE_AMOUNT = 6572;

// 6572
int main () {
    char mem[FILE_ELEMS] = {};
    char filename[20] = {};

    FILE *outFile = fopen("BadAppleOut1.asm", "w");

    fprintf(outFile, "PUSH %d\nPOP [%d]\n", '\033', 0);
    fprintf(outFile, "PUSH %d\nPOP [%d]\n", '[', 1);
    fprintf(outFile, "PUSH %d\nPOP [%d]\n", 'H', 2);

    for (size_t ind = 1; ind <= FILE_AMOUNT; ind++) {
        sprintf(filename, "./badApple/bad-apple-ascii/frames-ascii/out%04d.jpg.txt", ind);
        string_t string = {};
        GetFileFull(filename, &string);
        for (size_t elem = 0; elem < FILE_ELEMS; elem++) {
            if(string.str[elem] != mem[elem]) {
                mem[elem] = string.str[elem];
                fprintf(outFile, "PUSH %d\nPOP [%d]\n", string.str[elem], elem + 3);
            }
        }
        fprintf(outFile, "DRAW\n");
        free(string.str);
    }
    fclose(outFile);
}