#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/task.h>

void hello (void * unused)
{
	while (1) {
		rt_printf("Hello from Xenomai Realtime Space\n");
		rt_task_sleep(1000000000LL);
	}
}


int main(void)
{
	int err;
	RT_TASK task;

	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	if (((err = rt_task_spawn( & task,"HELLO",0,99,T_JOINABLE,hello,NULL)) != 0 )) {
	fprintf(stderr, "rt_task_spawn : %s\n",strerror(-err));
	exit(EXIT_FAILURE);
	}

	rt_task_join( & task );
	return 0;
}
		
