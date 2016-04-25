#include "stm32f10x_type.h"

#define CO_TIMER_MAX_NB		10

struct co_timer {
	unsigned char used;
	unsigned char run_enable;                /* Flag indicating whether timer is enabled(1)           */
	int times_left;
	int times_init;
	unsigned int count_cur;                  /* Current value of timer (counting down)             */
	unsigned int count_init;                  /* Initial value of timer (i.e. when timer is set)    */
	void    (*timer_func)(void *);                /* Function to execute when timer times out           */
	void     *func_arg;                             /* Arguments supplied to user defined function        */
};

static struct co_timer co_timer_pool[CO_TIMER_MAX_NB];

static void timer_tick( void *pdata)
{
	int i;

	for ( i = 0; i < CO_TIMER_MAX_NB; i++ ) {
		if (co_timer_pool[i].used == 1
		    && co_timer_pool[i].run_enable == 1) {

			if (co_timer_pool[i].count_cur != 0) {
				co_timer_pool[i].count_cur--;
			}

			if (co_timer_pool[i].count_cur == 0) {

				co_timer_pool[i].count_cur = co_timer_pool[i].count_init;

				if (co_timer_pool[i].times_left == 1) {
					co_timer_pool[i].run_enable = 0;
				}
				else if (co_timer_pool[i].times_left > 1) {
					co_timer_pool[i].times_left--;
				}

				if(co_timer_pool[i].timer_func != NULL) {
					co_timer_pool[i].timer_func(co_timer_pool[i].func_arg);
				}
			}
		}
	}
}


/*-------------------------------------------------------------------------
    Function    :  co_timer_is_running             xxx

    Return:
        0 error, 1 ok
    Description:
        if the timer is running
-------------------------------------------------------------------------*/
int co_timer_is_running (int timer_handle)
{
	return co_timer_pool[timer_handle-1].run_enable;
}
/*-------------------------------------------------------------------------
    Function    :  co_timer_init             xxx

    Description:
        init the whole timer module, must be called by user, this will create a ucos task
-------------------------------------------------------------------------*/
void co_timer_init(unsigned char timer_task_prio, unsigned int *ptos )
{
	uint8 i;

	for ( i = 0; i < CO_TIMER_MAX_NB; i++ ) {
		co_timer_pool[i].used = 0;
	}
}

/*-------------------------------------------------------------------------
    Function    :  co_timer_restart             xxx

    Description:
-------------------------------------------------------------------------*/
void co_timer_restart(int timer_handle )
{

	if (co_timer_pool[timer_handle-1].used == 0) {
		return;
	}

	co_timer_pool[timer_handle-1].count_cur = co_timer_pool[timer_handle-1].count_init;
	co_timer_pool[timer_handle-1].times_left = co_timer_pool[timer_handle-1].times_init;
	co_timer_pool[timer_handle-1].run_enable = 1;

}

/*-------------------------------------------------------------------------
    Function    :  co_timer_resume             xxx

    Description:
-------------------------------------------------------------------------*/
void co_timer_resume(int timer_handle )
{
	if(!co_timer_is_running(timer_handle)) 
		co_timer_restart(timer_handle);
}


/*-------------------------------------------------------------------------
    Function    :  co_timer_del             xxx

    Description:
        del one timer
-------------------------------------------------------------------------*/
void co_timer_del(int timer_handle)
{
	co_timer_pool[timer_handle-1].used = 0;
}

/*-------------------------------------------------------------------------
    Function    :  co_timer_start             xxx
    Description:
        start a timer whose id is in parameter
-------------------------------------------------------------------------*/
void co_timer_start(int timer_handle)
{
	if (co_timer_pool[timer_handle-1].used == 0) {
		return;
	}

	co_timer_pool[timer_handle-1].run_enable = 1;
}

/*-------------------------------------------------------------------------
    Function    :  co_timer_stop             xxx

    Description:
        stop the timer
-------------------------------------------------------------------------*/
void co_timer_stop(int timer_handle)
{
	co_timer_pool[timer_handle-1].run_enable = 0;
}


/*-------------------------------------------------------------------------
    Function    :  co_timer_new             xxx
    Parameters:
        N/A
    Return:
        0:       fail
        >0:  timer handle
    Description:
        create a timer with the function registered, but did not start it
-------------------------------------------------------------------------*/
int co_timer_new(unsigned int millisecond, void (*timer_func)(void *), void *arg, int times)
{

	int i;

	for ( i = 0; i < CO_TIMER_MAX_NB; i++ ) {
		if (co_timer_pool[i].used == 0) {
			break;
		}
	}

	if (i==CO_TIMER_MAX_NB) {
		return 0;
	}

	co_timer_pool[i].run_enable = 0;
	co_timer_pool[i].count_init = millisecond;
	co_timer_pool[i].count_cur = co_timer_pool[i].count_init;
	co_timer_pool[i].times_init = times;
	co_timer_pool[i].times_left = co_timer_pool[i].times_init;
	co_timer_pool[i].timer_func = timer_func;
	co_timer_pool[i].func_arg = arg;

	co_timer_pool[i].used = 1;
	return i+1;
}

