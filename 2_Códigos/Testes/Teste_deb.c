#include "sensors_funcs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    char *test1_c = malloc(strlen(argv[1]));
    int test1_i, bt=1;

    bcm2835_init();
    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_07,BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_aren(RPI_V2_GPIO_P1_07);

    strcpy(test1_c,argv[1]);
    while(bt){
        if(bcm2835_gpio_eds(RPI_V2_GPIO_P1_07)==HIGH){
            bt = debounce_mavg(RPI_V2_GPIO_P1_07,10,0.5,0);
            test1_i = take_a_photo(test1_c);
        }
    }

    bcm2835_gpio_clr_aren(RPI_V2_GPIO_P1_07);
    free(test1_c);
    
    return 0;
}