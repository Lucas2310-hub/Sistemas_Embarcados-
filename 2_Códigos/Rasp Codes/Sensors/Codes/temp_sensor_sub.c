#include <stdio.h>
#include <stdlib.h>
#include "0_sensors_consts.h"

int temp_ok(float temp){
    
    //scanf("%f", &temp);
    
    if(temp>38) return 0;
    return 1;
}
