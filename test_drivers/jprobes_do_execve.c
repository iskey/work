#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

struct jprobe exec_jp;
extern void show_mem_simple(void);

int jp_do_execve(const char * filename,
	const char __user *const __user *argv,
	const char __user *const __user *envp,
	struct pt_regs * regs)
{
	int cnt = 0;

	printk("| pid: %6d | comm: %10s | filename = %15s |\n", current->pid, current->comm, filename);
	
	show_mem_simple();

	#if 0
	for(; *argv != NULL;argv++,cnt++)
		printk("argv[%d] = %s\n", cnt, *argv);
	#endif
		
	jprobe_return();
	return 0;
}


static __init int jprobes_exec_init(void)
{    
	exec_jp.kp.symbol_name = "do_execve";

	exec_jp.entry = JPROBE_ENTRY(jp_do_execve);

	/*×¢²ájprobes*/
	register_jprobe(&exec_jp);
	
	return 0;
}



static __exit void jprobes_exec_cleanup(void)
{
	/*³·Ïújprobes×¢²á*/
	unregister_jprobe(&exec_jp);
}

module_init(jprobes_exec_init);
module_exit(jprobes_exec_cleanup);
MODULE_LICENSE("GPL");

