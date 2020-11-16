#include <stdio.h>
#include <stdlib.h>
#include "sensors_funcs.h"

int main(int argc, char *argv[]){
    char *test1_c = malloc(strlen(argv[1]));
    int test1_i;

    strcpy(test1_c,argv[1]);
    test1_i = take_a_photo(test1_c);

    free(test1_c);
    printf("%d", test1_i);

    return 0;
}
