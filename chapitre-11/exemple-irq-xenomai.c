
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/intr.h>
#include <native/task.h>
#include <native/timer.h>

int main(int argc, char * argv[])
{
	RT_INTR interruption;
	int irq;
	
	if ((argc != 2) || (sscanf(argv[1], "%d", & irq) != 1)) {
		fprintf(stderr, "usage: %s irq\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	
	if ((err = rt_task_shadow(& task, "Exemple IRQ",
	                         99, T_JOINABLE)) != 0) {
		fprintf(stderr, "rt_task_shadow: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
	
	rt_intr_create(& interruption, "Exemple", irq, I_PROPAGATE);
	
	while (1) {
		rt_intr_enable(& interruption);
		erreur = rt_intr_wait(& interruption, TM_INFINITE);
		if (erreur < 0) {
			fprintf(stderr, "Erreur %d\n", erreur);
			break;
		}
		rt_printf("%lld\n", rt_timer_read());
	}
	return 0;
}


