#include "sensors_consts.h"
#include <bcm2835.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <time.h>

int take_a_photo(char *addr);
int poll_btn(struct pollfd *pf, int N, float Nlim);
int send_video(char *dev, char *file, char *msg);
int time_video(char *file, char *msg, int time);
int take_child(pid_t pid, char *msg);
