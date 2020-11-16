#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int take_a_photo(char *addr){
    int err=0;
    char *msg = "raspistill -o ", *call;
    
    call = malloc(strlen(msg)+strlen(addr));
    strcpy(call,msg);
    strcat(call,addr);
    err = system(call);

    free(call);

    return err;
}