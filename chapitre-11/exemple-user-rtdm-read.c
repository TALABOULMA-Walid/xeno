// ---------------------------------------------------------------------
// exemple-user-rtdm-read.c
// Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
// (C) 2012 - Christophe BLAESS
// http://christophe.blaess.fr
// ---------------------------------------------------------------------

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <rtdm/rtdm.h>

#define LG_BUFFER 80

int main (int argc, char * argv[])
{
	int fd;
	int i;
	char buffer[LG_BUFFER];
	
	if (argc < 2) {
		fprintf(stderr, "usage: %s rtdm_device\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	fd = rt_dev_open(argv[1], O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "%s: %s\n", argv[1], strerror(-fd));
		exit(EXIT_FAILURE);
	}
	
	while ((i = rt_dev_read(fd, buffer, LG_BUFFER)) > 0) {
		buffer[i] ='\0';
		printf("%s\n", buffer);
	}
	rt_dev_close(fd);
	return EXIT_SUCCESS;
}


