#include "main.h"

#define ADD 1
#define CUT 2

int main() {

    printf("running...\n");

    int fd_key = open("/dev/key", O_RDWR);
    if (fd_key < 0) {
        printf("open key failed\n");
        exit(-1);
    }
    int fd_seg = open("/dev/FS4412_segdis", O_RDWR);
    if (fd_seg < 0) {
        printf("open seg failed\n");
        exit(-1);
    }
    int fd_pwm = open("/dev/pwm", O_RDWR | O_NONBLOCK);
    if (fd_pwm < 0) {
        printf("open pwm failed\n");
        exit(-1);
    }

    int keyval = 0;
    int level = 0;
     uint8_t cmd[2];
	cmd[0]=1;

    while (1) {
		printf("seg show %d..\n",level);
        cmd[1] = level+'0';
        FS4412_segdis_w_dig(fd_seg, cmd);
		printf("seg show success\n");
        ioctl(fd_pwm, SET_CNT,&level);
        if (read(fd_key, &keyval, sizeof(int)) != sizeof(int)) {
            printf("read key failed\n");
            exit(-1);
        }
        if (keyval == ADD) {
            if (level < 2) {
                level++;
            }
        } else if (keyval == CUT) {
            if (level > 0) {
                level--;
            }
        }
    }
}


