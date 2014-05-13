#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/alarm.h>
#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h>

static RT_ALARM alarme;

void fonction_thread (void * arg)
{
	int err;
	int numero = (int) arg;
	RT_TASK_INFO rtinfo;

	rt_task_inquire(NULL, & rtinfo);
	rt_printf("[%d] Priorite initiale %d\n", numero, rtinfo.cprio);

	while(1) {
		
		if ((err = rt_alarm_wait(& alarme)) != 0) {
			fprintf(stderr, "rt_alarm_wait(): %s\n",
			                strerror(-err));
			exit(EXIT_FAILURE);
		}
		rt_task_inquire(NULL, & rtinfo);
		rt_printf("[%d] priorite : %d, heure : %llu\n", numero, rtinfo.cprio, rt_timer_read());
	}	
}


#define NB_TACHES 5

int main(void)
{	
	int err;
	int i;
	RT_TASK task[NB_TACHES];
	char nom_tache[80];
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	if ((err = rt_alarm_create(& alarme, "Ex01")) != 0) {
		fprintf(stderr, "rt_alarm_create(): %s\n",
		                strerror(-err));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < NB_TACHES-1; i++) {
		snprintf(nom_tache, 80, "Alarme-%d\n", i+1);
		if ((err = rt_task_spawn(& (task[i]), nom_tache, 0,
		                         90-NB_TACHES + i, T_JOINABLE,
		                         fonction_thread, (void *) (i+1))) != 0) {
			fprintf(stderr, "rt_task_spawn: %s\n", strerror(-err));
			exit(EXIT_FAILURE);
		}
	}

	if ((err = rt_alarm_start(& alarme, TM_NOW, 1000000000)) != 0) {
		fprintf(stderr, "rt_alarm_start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < NB_TACHES; i ++)
		rt_task_join(& task[i]);
	return 0;
}


