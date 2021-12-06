/* simple_module.c - a simple template for a loadable kernel module in Linux,
   based on the hello world kernel module example on pages 338-339 of Robert
   Love's "Linux Kernel Development, Third Edition."
 */

//#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/types.h>
#include <asm/atomic.h>


unsigned long now_tick;
static int get_current_cpu;
static struct task_struct *task0,*task1,*task2,*task3;
volatile atomic_t race;
//int i=0;
#define iters 1000000

static int simple_task(void *arguments)
{
	int i=0;
	//	now_tick = jiffies;
	//printk("Module loading..jiffies - %lu",now_tick);
	printk(KERN_ALERT "simple module is being loaded\n");
	get_current_cpu = get_cpu();

	printk("Getting current CPU %d to binding thread\n",get_current_cpu);
	
	for(i=0;i<iters;i++){
		atomic_add(1,&race);
	}	
	return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {	
	
	atomic_set(&race,0);
	task0 = kthread_create(simple_task,NULL,"thread1");
	printk(KERN_ALERT "Creating thread1\n");
	
	task1 = kthread_create(simple_task,NULL,"thread2");
	printk(KERN_ALERT "Creating thread2\n");
	kthread_bind(task1,1);	
		
	
	task2 = kthread_create(simple_task,NULL,"thread3");
	printk(KERN_ALERT "Creating thread3\n");
	kthread_bind(task2,2);
		
	
	task3 = kthread_create(simple_task,NULL,"thread4");
	printk(KERN_ALERT "Creating thread4\n");
	kthread_bind(task3,3);		
	
	
	if(task0 && task1 && task2 && task3){
		printk(KERN_ALERT "Waking up task1\n");
		wake_up_process(task0);
		
		printk(KERN_ALERT "Waking up task2\n");
		wake_up_process(task1);
		
		printk(KERN_ALERT "Waking up task3\n");
		wake_up_process(task2);
		
		printk(KERN_ALERT "Waking up task4\n");
		wake_up_process(task3);		
	}
	else
		printk(KERN_ALERT "Error while creating threads\n");

	return 0;
	}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
//	now_tick = jiffies;
//	printk("Module unloading..jiffies - %lu",now_tick);
	printk(KERN_ALERT "simple module is being unloaded\n");
	printk("Race value %d\n",atomic_read(&race));

}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE("GPL");

