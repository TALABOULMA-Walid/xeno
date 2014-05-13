
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <rtdm/rtdm.h>


int main (int argc, char * argv[])
{
	int fd;
	int i;

	if (argc < 2) {
		fprintf(stderr, "usage: %s rtdm_device\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	fd = rt_dev_open(argv[1], O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "%s: %s\n", argv[1], strerror(-fd));
		exit(EXIT_FAILURE);
	}
	
	for (i = 2; i < argc; i ++)
		rt_dev_write(fd, argv[i], strlen(argv[i]));
		
	rt_dev_close(fd);
	exit(EXIT_SUCCESS);
}


