
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/task.h>


int main(void)
{
	int err;
	RT_TASK task;
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	
	if ((err = rt_task_shadow(& task, "Hello World 02",
	                         99, T_JOINABLE)) != 0) {
		fprintf(stderr, "rt_task_shadow: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
	while (1) {
		rt_printf("Hello World 02\n");
		rt_task_sleep(1000000000LL); // 1 sec.
	}
	return 0;
}

