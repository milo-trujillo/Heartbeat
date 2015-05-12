#include <linux/module.h>    // Needed for all kernel modules
#include <linux/kernel.h>    // KERN_INFO
#include <linux/init.h>      //  __init and __exit macros

// These three are for scheduling timers and interrupts 
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Milo Trujillo");
MODULE_DESCRIPTION("Heartbeat");

// Both of these are used for setting a periodic event
static struct hrtimer htimer; // This is the timer
static ktime_t kt_periode; // This is a counter used to prime the timer

static enum hrtimer_restart timer_function(struct hrtimer * timer)
{
	printk(KERN_INFO "Timer went off!\n");

	hrtimer_forward_now(timer, kt_periode);
	return HRTIMER_RESTART;
}

static void timer_init(void)
{
	kt_periode = ktime_set(5, 0); //seconds,nanoseconds
	hrtimer_init (&htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	htimer.function = timer_function;
	hrtimer_start(&htimer, kt_periode, HRTIMER_MODE_REL);
}

static void timer_cleanup(void)
{
	hrtimer_cancel(&htimer);
}

static int __init mod_init(void)
{
	timer_init();
	printk(KERN_INFO "Started heartbeat\n"); 
	return 0;    
}

static void __exit mod_cleanup(void)
{
	timer_cleanup();
	printk(KERN_INFO "Stopped heartbeat\n");
}


//We're "registering" our module with the rest of the Kernel here.
module_init(mod_init);
module_exit(mod_cleanup);


