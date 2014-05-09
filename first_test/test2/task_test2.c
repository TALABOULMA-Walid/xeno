#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/task.h>

void hello (void * unused)
{
	int i=0;
	while (1) {
		i++;
		rt_printf(" i = :%d\n",i );
		rt_task_sleep(1000000LL);
		if ( (i % 10) == 0 ) {
		rt_task_sleep(1000000000LL);
		}
	}
}


void hello2 (void * unused)
{
	int j=0;
	while (1) {
		j++;
		rt_printf("\t\t j = :%d\n",j );
		rt_task_sleep(1000000LL);
		if ( (j % 10) == 0 ) {
		rt_task_sleep(1000000000LL);
		}
		}
}


int main(void)
{
	int err;
	RT_TASK task, task2;

	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	if (((err = rt_task_spawn( & task,"HELLO",0,99,T_JOINABLE,hello,NULL)) != 0 )) {
	fprintf(stderr, "rt_task_spawn : %s\n",strerror(-err));
	exit(EXIT_FAILURE);
	}

	if (((err = rt_task_spawn( & task2,"HELLO2",0,98,T_JOINABLE,hello2,NULL)) != 0 )) {
	fprintf(stderr, "rt_task_spawn : %s\n",strerror(-err));
	exit(EXIT_FAILURE);
	}

	rt_task_join( & task );
	rt_task_join( & task2);
	return 0;
}
		
