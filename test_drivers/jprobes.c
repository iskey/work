#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

struct jprobe exec_jp;


int jp_do_execve(struct napi_struct *napi, int budget)
{
	#if 0
	int cnt = 0;

	printk("filename = %s\n", filename);

	for(; *argv != NULL;argv++,cnt++)
		printk("argv[%d] = %s\n", cnt, *argv);
	#endif
		
	dump_stack();

	jprobe_return();
	return 0;
}


static __init int jprobes_exec_init(void)
{    
	exec_jp.kp.symbol_name = "mv_eth_poll";

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

