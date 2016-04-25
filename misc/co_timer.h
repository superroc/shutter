#ifndef _CO_TIMER_H
#define _CO_TIMER_H

int co_timer_is_running (int timer_id);
void co_timer_init(unsigned char timer_task_prio, unsigned int *ptos );
void co_timer_del(int timer_handle);
void co_timer_restart(int timer_handle );
void co_timer_resume(int timer_handle );

void co_timer_start(int timer_handle);
void co_timer_stop(int timer_handle);
int co_timer_new(unsigned int millisecond, void (*timer_func)(void *), void *arg, int times);

#endif

