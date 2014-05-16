#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h>

#define DURATION 1000000000
#define TIMER_READ rt_timer_read()-REF
#define TASK_NUMBER 4
#define SECOND 1000000000
static RT_ALARM alarm;

int hp=2;

typedef  struct Element_tab_ordo {
 char name[16];
 int id;
 int duree;
 int create_thread; 
 void *(*code)();
}ELEMENT_TAB ;


RT_TASK task[TASK_NUMBER];
const SIZE_T=6;
int i=-1;
int i_prec=0;
int j=0;

ELEMENT_TAB T[]={{"task2",2,2,1,task2},{"task1",1,2,1,task1},{"task2",2,2,0,task2},{"task3",3,2,1,task3},{"task1",1,2,1,task1},{"task_vide",4,2,1,task_vide}};

void task1(void* unsued)          /// (2,2,6,6)
{
  int compteur = 0;
  rt_printf("[%lld]Début task 1\n", TIMER_READ);
  while ( compteur++ < DURATION  ){}
  rt_printf("[%lld]Fin task 1\n", TIMER_READ);
}

void task2(void* unsued)	/// (0,4,12,12)
{
  int compteur = 0;
  rt_printf("[%lld]Début task 2\n", TIMER_READ);
  while ( compteur++ < 2 * DURATION ){}
  rt_printf("[%lld]Fin task 2\n", TIMER_READ);
}

void task3(void* unsued)	/// (0,2,12,12)
{
  int compteur = 0;
  rt_printf("[%lld]Début task 3\n", TIMER_READ);
  while ( compteur++ < DURATION ){}
  rt_printf("[%lld]Fin task 3\n", TIMER_READ);
}

void task_vide(void* unsued)	/// free time filler task
{
  int compteur = 0;
  rt_printf("[%lld]Début task_vide\n", TIMER_READ);
  while ( compteur++ < DURATION ){}
  rt_printf("[%lld]Fin task_vide\n[%lld]Hyperperiod : %d\n", TIMER_READ,TIMER_READ,hp);
}
/// *********************SCHEDULER******************** ///

void scheduler()
{
  while(j++<50)
  {
  rt_alarm_wait(&alarm);
  
  if(i==(SIZE_T-1)){
    i_prec=i;
    i=I_PERM;
  }
  else {
    i_prec=i;
    i++;
  } 
  /// if the previous thread is preempted (i.e it is running )and we are not in the initial phase ( i != -1 )
  /// we suspend it
  if((i_prec!=(-1))&&(i!=(-1))&&(task[(T[i_prec].id-1)]) ){
       rt_task_suspend(&(task[(T[i_prec].id-1)]));
  }
  
  /// create the next task in the table or resume it if it was suspended
  if(T[i].create_thread==1){
    rt_task_spawn(&(task[(T[i].id-1)]),
		  T[i].name,
		  0,
		  90 + TASK_NUMBER - T[i].id,
		  T_JOINABLE,
		  T[i].code,
		  NULL);
  }
  else {
   rt_printf("[%lld]Reprise task %d\n", TIMER_READ, T[i].id);
   rt_task_resume(&(task[(T[i_prec].id-1)]));    
  }
  rt_alarm_start(& alarm, T[i].duree * SECOND, TM_INFINITE);
  }
}    
  

int main()
{
  int REF = 0;
  RT_TASK ORD;
  mlockall(MCL_CURRENT|MCL_FUTURE);
  rt_print_auto_init(1);
  ///***********************************************************************/
  if((err=rt_alarm_create(& alarm, "timetrigger_ordonnanceur")) !=0) {
    exit(EXIT_FAILURE);}
  
  ///***********************************************************************/ 
  
  
  rt_task_spawn(ORD,
		"scheduler",
		0,
		99,
		T_JOINABLE,
		scheduler,
		NULL);
  
  rt_task_sleep(1000000000LL); 	///sleep one second
  REF = rt_timer_read(); 	/// set time reference
  if((err=rt_alarm_start(& alarm, TM_NOW, TM_INFINITE))!= 0){
    exit(EXIT_FAILURE);}
  
  rt_task_join(ORD);
  rt_task_join(task[0]);
  rt_task_join(task[1]);
  rt_task_join(task[2]);
  rt_task_join(task[3]);
  rt_alarm_delete(& alarm);
  
 return 0; 
}
