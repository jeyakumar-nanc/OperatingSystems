/* simple_module.c - a simple template for a loadable kernel module in Linux,
   based on the hello world kernel module example on pages 338-339 of Robert
   Love's "Linux Kernel Development, Third Edition."
 */

#include <linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/time.h>
#include<linux/mm.h>
#include<linux/log2.h>
#include<linux/moduleparam.h>
#include <linux/jiffies.h>



unsigned int objnum = 2000;
static struct task_struct *task0;
unsigned long now_tick;

//#define GFP_KERNEL



static struct spage{
	unsigned int pageArray[8];
};


module_param(objnum,int,0);
MODULE_PARM_DESC(objnum, "A single unsigned integer parameter");


	
static int simple_task(void *arguments)
{
	int order,totalmem;

	now_tick = jiffies;
	
	struct page *mypage;
			
	printk(KERN_ALERT "simple module is being loaded\n");

	printk("kernal memory Page size is : %ld",PAGE_SIZE);
	
	printk("Value of objnum is %d \n",objnum);		
	
	printk("Size of struct is %ld \n",sizeof(struct spage));
	
	printk("No of struct type instances would fit a page : %ld",PAGE_SIZE/sizeof(struct spage));
	
	totalmem = objnum*sizeof(struct spage);
	printk("Total memory required %d \n",totalmem);
	
	order = order_base_2(totalmem);
	
	printk(KERN_ALERT "No of pages to be allocated %d",order);
	
	//counting num of ticks in milliseconds during page alloc
	printk(KERN_ALERT "Allocating pages..jiffies in milliseconds - %lu",now_tick*1000/HZ);
	mypage = alloc_pages(GFP_KERNEL,order);
	
	if(!mypage)
		return -ENOMEM;
	else
		__free_pages(mypage,order);
	
	now_tick = jiffies;
	//counting num of ticks in milliseconds during page dealloc
	printk(KERN_ALERT "After Freeing pages..jiffies in milliseconds - %lu",now_tick*1000/HZ);
				
	return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {	
	
	task0 = kthread_create(simple_task,NULL,"thread1");
	printk(KERN_ALERT "Creating thread1\n");
	kthread_bind(task0,0);
	
	if(task0){
		printk(KERN_ALERT "Waking up task1\n");
		wake_up_process(task0); 
						
	}
	else
		printk(KERN_ALERT "Error while creating threads\n");

	return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
	now_tick = jiffies;
	//counting num of ticks in milliseconds during page dealloc
	printk(KERN_ALERT "After Freeing pages..jiffies in milliseconds - %lu",now_tick*1000/HZ);
	
	printk(KERN_ALERT "simple module is being unloaded\n");			

}

module_init (simple_init);
module_exit (simple_exit);

MODULE_LICENSE("GPL");

