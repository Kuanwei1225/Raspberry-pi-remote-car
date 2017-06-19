/*
This program is used to control DC motor executed by fast cgi.
version 1.0 (2015.6)
Kuan-Wei Ho
*/
/*
Right wheel forward: GPIO 18
Right wheel back: GPIO 23
Left wheel forward: GPIO 24
Left wheel back: GPIO 25
*/
#include "fcgi_stdio.h"
#include "mygpio_ioctl.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define R_F 18
#define R_B 23
#define L_F 24
#define L_B 25

#define GO_F '1'
#define GO_B '2'
#define GO_R '3'
#define GO_L '4'

#define DEVFILE "/dev/rpigpio"

int open_file(char *filename)
{
	int fd = open(filename, O_RDWR);
	if(fd < 0)
		perror("open");
	return fd;
}
void gpioRequest(int fd, int pin)
{
	int ret = ioctl(fd, GPIO_REQUEST, &pin);
	if(ret < 0)
		printf("request error!\n");
}
void gpioFree(int fd, int pin)
{
	ioctl(fd, GPIO_FREE, &pin);
}
void gpioModeSet(int fd, struct gpio_data *data)
{
	ioctl(fd, GPIO_MODE_SET, data);
}
void gpioSet(int fd, int pin)
{
	ioctl(fd, GPIO_SET, &pin);
}
void gpioClr(int fd, int pin)
{
	ioctl(fd, GPIO_CLR, &pin);
}
void gpioWriteInit(int fd, int pin)
{
	struct gpio_data wio;
	wio.pin = pin;
	wio.data = 0;
	wio.mode = 'W';
	gpioRequest(fd, pin);
	gpioModeSet(fd, &wio);
}
void main ()
{
	char *buffer, *token;
	int length, fd, ret, strSize, i;

	while(FCGI_Accept() >= 0) {
		printf("Content-type:  text/html\n\n");
		length = atoi(getenv("CONTENT_LENGTH"));
		buffer = (char*)malloc(length + 1);
		fread(buffer, 1, length, stdin);
		buffer[length] = '\0';
		token = strtok(buffer, "=");
		token = strtok(NULL, "=");
		strSize = strlen(token);
		//gpio
		fd = open_file(DEVFILE);
		gpioWriteInit(fd, R_F);
		gpioWriteInit(fd, R_B);
		gpioWriteInit(fd, L_F);
		gpioWriteInit(fd, L_B);
		for(i=0; i<strSize; i++) {
			if(token[i] == GO_F) {
				gpioSet(fd, R_F);
				gpioSet(fd, L_F);
				gpioClr(fd, R_B);
				gpioClr(fd, L_B);
			}else if(token[i] == GO_B) {
				gpioSet(fd, R_B);
				gpioSet(fd, L_B);
				gpioClr(fd, R_F);
				gpioClr(fd, L_F);
			}else if(token[i] == GO_R) {
				gpioSet(fd, L_F);
				gpioClr(fd, R_F);
				gpioClr(fd, L_B);
				gpioClr(fd, R_B);
			}else if(token[i] == GO_L) {
				gpioSet(fd, R_F);
				gpioClr(fd, L_F);
				gpioClr(fd, R_B);
				gpioClr(fd, L_B);
			}
			sleep(2);
		}
		gpioFree(fd, R_F);
		gpioFree(fd, R_B);
		gpioFree(fd, L_F);
		gpioFree(fd, L_B);
		printf("<head>\n");
		printf("</head>\n");
		printf("<body>\n");
		printf("<p>Command : %s</p>\n", token);
		printf("</body>\n");
		free(buffer);
	}
}




