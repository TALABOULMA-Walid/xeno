// ---------------------------------------------------------------------
// exemple-watchdog-01.c
// Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
// (C) 2012 - Christophe BLAESS
// http://christophe.blaess.fr
// ---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>


int main(void)
{
	int err;
	RT_TASK task;
	int nb_secondes;
	int i;
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	
	if ((err = rt_task_shadow(& task, "Essai Watchdog",
	                         99, T_JOINABLE)) != 0) {
		fprintf(stderr, "rt_task_shadow: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
	nb_secondes = 1;
	while (1) {
		rt_fprintf(stderr, "Boucle active de %d s.\n", nb_secondes);
		rt_task_sleep(500000000); // 0.5 s
		for (i = 0; i < nb_secondes; i ++)
			rt_timer_spin(1000000000ULL);
		nb_secondes ++;
	}
	return 0;
}

