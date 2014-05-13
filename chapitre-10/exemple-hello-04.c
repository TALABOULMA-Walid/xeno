
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <rtdk.h>
#include <native/task.h>


void fonction_hello_world (void * unused)
{
	int i = 1;
	while (1) {
		rt_fprintf(stderr, "Ecriture de 256 octets : %d\n", i++);
		printf( "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF"
		        "0123456789ABCDEF123456789ABCDEF");
			rt_task_sleep(1000000000LL);
	}
}

void gestionnaire_sigxcpu(int unused)
{
	#define NB_ADRESSES 128
	void * adresses[NB_ADRESSES];
	int    nb_adresses;
	nb_adresses = backtrace(adresses, NB_ADRESSES);
	backtrace_symbols_fd(adresses, nb_adresses, STDERR_FILENO);
}


int main(void)
{
	int err;
	RT_TASK task;
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	signal(SIGXCPU, gestionnaire_sigxcpu);
	
	if ((err = rt_task_spawn(& task, "Hello 01",
	                         0, 99, T_JOINABLE | T_WARNSW,
	                         fonction_hello_world, NULL)) != 0) {
		fprintf(stderr, "rt_task_spawn: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
	rt_task_join(& task);
	return 0;
}

