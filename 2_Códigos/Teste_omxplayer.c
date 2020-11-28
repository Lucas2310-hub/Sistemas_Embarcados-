#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "sensors_funcs.h"


int main(int argc, char *argv[]){

    char result[1024];
    int i;
      
    time_video(argv[1],result,10);
    
    printf("Mensagem:\n%s",result);

    return 0;
}