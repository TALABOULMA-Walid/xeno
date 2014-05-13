// ---------------------------------------------------------------------
// exemple-semaphore-01.c
// Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
// (C) 2012 - Christophe BLAESS
// http://christophe.blaess.fr
// ---------------------------------------------------------------------

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/sem.h>
#include <native/task.h>
#include <native/timer.h>

#define NB_TACHES 6
static RT_SEM  sem;
static int tient_semaphore[NB_TACHES];

void fonction_thread (void * arg)
{
	int i;
	int num = (int) arg;
	for (i = 0; i < 4; i ++) {
		tient_semaphore[num] = 0;
		rt_sem_p(& sem, TM_INFINITE);
		tient_semaphore[num] = 1;
		sleep(2);
		rt_sem_v(& sem);
	}
	tient_semaphore[num] = -1; // fini
}


void observateur(void * unused)
{
	int i;
	rt_printf("Avec semaphore    Sans semaphore\n");
	while (1) { 
		for (i = 0; i < NB_TACHES; i ++)
			if (tient_semaphore[i] == 1)
				rt_printf("%d ", i);
		rt_printf ("          ");
		for (i = 0; i < NB_TACHES; i ++)
			if (tient_semaphore[i] == 0)
				rt_printf("%d ", i);
		rt_printf("\n");
		rt_task_sleep(1000000000);
	}
}


int main(int argc, char * argv[])
{
	int i;
	int err;
	RT_TASK task[NB_TACHES];
	RT_TASK observ;
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	if ((err = rt_sem_create(& sem, "Sem-01", NB_TACHES-2, S_PRIO)) != 0) {
		fprintf(stderr, "rt_sem_create:%s\n",
		         strerror(-err));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < NB_TACHES; i ++) {
		if ((err = rt_task_spawn(& task[i], NULL, 0, 99,
	                  T_JOINABLE, fonction_thread, (void *) i) != 0)) {
			fprintf(stderr, "rt_task_spawn:%s\n",
			        strerror(-err));
			exit(EXIT_FAILURE);
		}
	}

	if ((err = rt_task_spawn(& observ, NULL, 0, 99,
	                  T_JOINABLE, observateur, NULL) != 0)) {
		fprintf(stderr, "rt_task_spawn:%s\n",
		        strerror(-err));
		exit(EXIT_FAILURE);
	}

	for (i  = 0; i < NB_TACHES; i ++)	
		rt_task_join(& task[i]);

	rt_task_delete(& observ);
	rt_sem_delete(& sem);

	return EXIT_SUCCESS;
}


