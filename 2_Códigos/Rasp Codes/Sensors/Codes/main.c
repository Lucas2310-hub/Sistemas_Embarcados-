#include "1_sensors_funcs.h"
#include "0_sensors_consts.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/poll.h>

#define MAX_CAP 2

struct main_par
{
    int btn, end, err;

};

volatile struct main_par pars;
volatile int state_act, state_nxt;
volatile pid_t pid_omx;
static pthread_mutex_t mutexLock;



void exit_main(){
    fputs("\nFechando...\n",stderr);
    pars.end=1;
    system("killall -s omxplayer.bin");
        
}


void *thread_exec(void *params){
    //char msg[60] = "Ctrl+C para sair\nEstado atual: %d\nPróximo estado: %d\n";
    int lock=0, tmp_end=0,tst_nxt=0, tst_act, lot, btn,qtd=0,cnt=0;
    float temp[3]={37.5,39,37};
    char msg[1024];
    //fputs("=0\nCerto\n", stderr);
    //signal(SIGINT,SIG_IGN);
    
    while(1){
        
        pthread_mutex_lock(&mutexLock);
        tmp_end=pars.end;
        tst_act=state_act;
        btn=pars.btn;
        
        //fprintf(stderr,"St: %d",tst_act);
        if(tmp_end==1){
            pthread_mutex_unlock(&mutexLock);
            break;
        }
        switch (tst_act)
        {
        case 0:
            if(lock==0)
            {
                //fputs("2",stderr);
                lock=1;
                inf_video(SCREEN1);
                //fprintf(stderr,"%d",pid_omx);
            }
            if(btn==1){
                lock=0;
                btn=0;
                tst_nxt=1;
                
            }
            
            break;
        case 1:
            if(lock==0){
                take_a_photo(PHOTO);
                lock=1;
                cnt+=1;
            }
            
            if(temp[cnt-1]<38){ 
                tst_nxt=2;
                lock=0;    
            }
            else{ tst_nxt=3;
            lock=0;}
            
            
            break;
        case 2:
            if(lock==0){
                system("killall -s omxplayer.bin");
                usleep(1);
                pid_omx = inf_video(SCREEN2);
                lock=1;
            }
            if(btn==1){
                lock=0;
                btn=0;
                tst_nxt=4;
            }
            break;
        case 3:
            if(lock=0){
                system("killall -s omxplayer.bin");
                usleep(1);
                send_video(HDMI,SCREEN3,msg);
                tst_nxt=0;
            }
            
            break;
        case 4:
            if(lock==0){
                system("killall -s omxplayer.bin");
                time_video(SCREEN4,msg,5);
                qtd+=1;
                if(qtd==MAX_CAP)tst_nxt=5;
                else tst_nxt=0;
            }
            break;
        case 5:
            if(lock==0){
                system("killall -s omxplayer.bin");
                inf_video(SCREEN5);
            }
            if(lot==0){
                lock=0;
                tst_nxt=0;
            }else lock=1;
            break;
        
        default:
            tst_nxt=0;
            lock=0;
            break;
        }
        
        state_nxt=tst_nxt;        
        pthread_mutex_unlock(&mutexLock);

        usleep(1);
    }
    pthread_exit(0);
}

void *thread_data(void *params){
    struct pollfd pol;
    int tmp_end, tmp_btn;

    signal(SIGINT,exit_main);
    
    system("echo 4		> /sys/class/gpio/export");
    system("echo both	> /sys/class/gpio/gpio4/edge");
    system("echo in		> /sys/class/gpio/gpio4/direction");
    pol.fd = open("/sys/class/gpio/gpio4/value", O_RDONLY);
    if(pol.fd < 0)
    {
        fputs("Erro abrindo /sys/class/gpio/gpio4/value\n",stderr);
        fputs("Execute este programa como root.\n",stderr);
        pars.err=1;
        pthread_exit(0);
    }
    pol.events = POLLPRI | POLLERR;
    pol.revents = 0;

    while (1)
    {
        tmp_btn=0;        
        pthread_mutex_lock(&mutexLock);
        tmp_end=pars.end;
        if(tmp_end==1){
            pthread_mutex_unlock(&mutexLock);
            break;
        }
        if(state_act==0 || state_act==2) tmp_btn=poll_btn(&pol,20,10);

        pars.btn=tmp_btn;
        pthread_mutex_unlock(&mutexLock);
        usleep(1);
    }
    close(pol.fd);
    system("echo 4 > /sys/class/gpio/unexport");
    pthread_exit(0);
}

void main(){
    pthread_t texec, tdata;
    struct main_par var;
    int temp_end, temp_tmp,err, cnt=0;
    float temp[4]={37.5,38,39};

    pars.err=0;
    pars.end=0;
    state_act=0;
    state_nxt=0;
    //printf("1\n");
    pthread_mutex_init(&mutexLock,NULL);

    pthread_create(&texec,NULL,thread_exec,NULL);
    pthread_create(&tdata,NULL,thread_data,NULL);

    //signal(SIGTERM, SIG_IGN);

    while(1){
        
        pthread_mutex_lock(&mutexLock);
        err=pars.err;
        temp_end=pars.end;
        if(err==1) kill(0,SIGINT);
        if(temp_end==1){
            pthread_mutex_unlock(&mutexLock);
            break;
        }
         
                
        state_act=state_nxt;
        pthread_mutex_unlock(&mutexLock);
        usleep(1);
    }
    pthread_join(texec,0);
    pthread_join(tdata,0);
    pthread_mutex_destroy(&mutexLock);

}