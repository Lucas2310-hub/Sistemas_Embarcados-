#include <string.h>
#include <bcm2835.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int take_a_photo(char *addr);
int debounce_mavg(uint8_t pin, int n, float rate, int drt);