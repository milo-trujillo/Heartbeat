#include <linux/module.h>    // Needed for all kernel modules
#include <linux/kernel.h>    // KERN_INFO
#include <linux/init.h>      //  __init and __exit macros
#include <linux/slab.h>      // For kmalloc()

// These are for networking from kernel-land
#include <linux/net.h>
#include <linux/tcp.h>
#include <linux/socket.h>
#include <net/sock.h>
#include <net/tcp.h>

// These three are for scheduling timers and interrupts 
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

#include <asm/uaccess.h>

// Change these two when installing on a new system
#define PORT 1234
#define ADDR 127, 0, 0, 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Milo Trujillo");
MODULE_DESCRIPTION("Heartbeat");

// Both of these are used for setting a periodic event
static struct hrtimer htimer; // This is the timer
static ktime_t kt_periode; // This is a counter used to prime the timer

static uint32_t create_address(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4)
{
	uint32_t addr = 0;

	addr += i1, addr <<= 8;
	addr += i2, addr <<= 8;
	addr += i3, addr <<= 8;
	addr += i4;

	return (addr);
}

static enum hrtimer_restart timer_function(struct hrtimer * timer)
{
	int status = -1;
	struct socket *sock= NULL;
	struct sockaddr_in servaddr;

	printk(KERN_INFO "Timer went off!\n");
	status = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);

	memset(&servaddr,0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(create_address(ADDR));

	status = sock->ops->connect(sock, (struct sockaddr *) &servaddr,
		sizeof(servaddr), O_RDWR);

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


