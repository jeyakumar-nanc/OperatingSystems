/* simple_module.c - a simple template for a loadable kernel module in Linux,
   based on the hello world kernel module example on pages 338-339 of Robert
   Love's "Linux Kernel Development, Third Edition."
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
/*#include <linux/stdio.h*.>*/

unsigned long now_tick;
/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
	now_tick = jiffies;
	printk("Module loading..jiffies - %lu",now_tick);
	printk(KERN_ALERT "simple module initialized\n");    
return 0;
}
/* exit function - logs that the module is being removed */
static void simple_exit (void) {
	now_tick = jiffies;
	printk("Module unloading..jiffies - %lu",now_tick);
	printk(KERN_ALERT "simple module is being unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

