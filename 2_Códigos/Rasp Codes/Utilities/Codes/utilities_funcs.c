#include "sensors_funcs.h"
#include <stdlib.h>
#include <stdio.h>

int debounce_mavg(uint8_t pin, int n, float rate, int drt){
    uint8_t level;
    float lim=((float) n)*rate;
    int test=1,i, s, ret, pos=0, *level_i;
    level_i = (int*) calloc(n,sizeof(int));

    if(drt=-1){
        
        for(s=i=0;i<n;i++){ 
            level = bcm2835_gpio_lev(pin);
            level_i[pos] = (int) level-'0';
            pos+=(pos+1)%n;
            s+=level_i[i];
        }
        if(s>=lim){
            ret=1;
        }else ret=0;
        

    }else if(drt=0){
        while(test){
            level = bcm2835_gpio_lev(pin);
            level_i[pos] = (int) level-'0';
            pos+=(pos+1)%n;
            for(s=i=0;i<n;i++){
                level = bcm2835_gpio_lev(pin);
                level_i[pos] = (int) level-'0';
                pos+=(pos+1)%n;
                s+=level_i[i];
            }
            if(s<=lim){
                test=0;
                ret=0;
            }
            
        }
    }else{
        while(test){
            
            for(s=i=0;i<n;i++){ 
                level = bcm2835_gpio_lev(pin);
                level_i[pos] = (int) level-'0';
                pos+=(pos+1)%n;
                s+=level_i[i];
            }
            if(s>=lim){
                test=0;
                ret=1;
            }
            
        }
    }
    return ret;
}

