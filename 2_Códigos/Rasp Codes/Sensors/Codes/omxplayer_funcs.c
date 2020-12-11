#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "0_sensors_consts.h"
#include "1_sensors_funcs.h"

volatile pid_t pid_omx;


int send_video(char *dev, char *file, char *msg){
    FILE *fr;
    char *func=NULL, *call=NULL, temp[50];
    
    if (!strcmp(HDMI,dev))
    {
        call = (char*) realloc(func, strlen(file)+strlen(HDMI)+10);
        strcpy(call,"omxplayer ");
        strcat(call,HDMI);
        strcat(call, file);
        
    }

    fr=popen(call,"r");
    if(fr==NULL) return -1;
    free(call);
    while(fgets(temp,50,fr)!=NULL) strcat(msg,temp);
    if(pclose(fr)<0) return -2;
    return 0;

}


int time_video(char *file, char *msg, int time){
    FILE *fr;
    char *func=NULL, *call=NULL, temp[20], tmsg[50];
    int fd[2], tc=0,tcpid=0;
    pid_t pid;
    
    
    //signal(SIGALRM, set_tout);
    
    if(pipe(fd) <0) return -1;

    pid=fork();

    if(pid<0) return -2;

    
    if(pid==0){
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("omxplayer","omxplayer","--loop","--no-osd",file,NULL);
    }else{
        //alarm(time);
        close(fd[1]);
        sleep(0.1);
        //take_child(pid, tmsg);
        //pid_omx = atoi(tmsg);
        

        sleep(time);
        
        while (read(fd[0],temp,20)!=0) strcat(msg,temp);

        system("killall -s omxplayer.bin");  
        //printf("Tmsg: %s\n", tmsg);
        //kill(pid,SIGTERM);
    }
    close(fd[0]);
        
    return 0;
};

pid_t inf_video(char *file){
    char tmsg[50]="", logg;
    pid_t pid, pid_om;
    
    //logg = open(LOG, O_RDWR | O_CREAT);
    
    pid=fork();

    if(pid<0) return -2;

    
    if(pid==0){
        //dup2(logg,STDOUT_FILENO);
        execlp("omxplayer","omxplayer","--loop","--no-osd",file,NULL);
    }
     
    sleep(0.1);
    take_child(pid, tmsg);
    fprintf(stderr,"%s",tmsg);
    pid_om = atoi(tmsg);
        
    return pid_om;
};