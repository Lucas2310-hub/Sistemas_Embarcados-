#include <stdlib.h>
#include <stdio.h>
#include <bcm2835.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//#define GPIO_ADDR "/sys/class/gpio/"

int poll_btn(struct pollfd *pfd, int N, float Nlim)
{
	char buffer, *vals;
	int pos = 0, test = 1, i, s,tst;
	vals = (char*) calloc(N,sizeof(char));
	lseek(pfd->fd,0,SEEK_SET);
	read(pfd->fd, &buffer, 1);
	for(i=0;i<N;i++) vals[i] = buffer-'0';
	poll(pfd,1,-1);
	while(test)
	{
		lseek(pfd->fd, 0, SEEK_SET);
		read(pfd->fd, vals+pos, 1);
		vals[pos] -='0';
		pos = (pos+1)%N;
		for(s=i=0; i<N; i++) s += vals[i];
		if(buffer=='0') {
			test = s<Nlim;
			tst = 1;}
		else{
			test=s>Nlim;
			tst=0;
			}
	}
	free(vals);
	return tst;
}

int take_child(pid_t pid, char *msg){
	FILE *fr;
	char command[35], tc[5];
	
	sprintf(command,"ps --ppid %d | cut -d \' \' -f 1",pid);

	fr = popen(command, "r");

	while(fgets(tc,5,fr)!=NULL) strcat(msg,tc);

	return 0;
}

