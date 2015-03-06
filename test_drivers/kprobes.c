#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

struct kprobe exec_kp;

static int exec_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	printk("[Kprobes] Enter into %s\n", p->symbol_name);
	printk("[Kprobes] pt_regs:%p, pid:%d, jiffies:%ld\n", regs, current->tgid, jiffies);
	//dump_stack();
	
	return 0;
}

static __init int kprobes_exec_init(void)
{    

	/*设置要检测的函数do_execve()的地址*/
	//exec_kp.addr = (kprobe_opcode_t *)0xc00bbfb8;

	/*如果不想直接给addr成员设置地址，也可以使用符号名*/
//	exec_kp.symbol_name = "mv_eth_start";
//	exec_kp.symbol_name = "k_BrdCtrlIoctlRtc";
//	exec_kp.symbol_name = "netif_receive_skb";
	exec_kp.symbol_name = "mv_eth_tx";
	exec_kp.pre_handler = exec_pre_handler;

	/*注册kprobes*/
	register_kprobe(&exec_kp);
	
	return 0;
}



static __exit void kprobes_exec_cleanup(void)
{
	/*撤销kprobes注册*/
	unregister_kprobe(&exec_kp);
}

module_init(kprobes_exec_init);
module_exit(kprobes_exec_cleanup);
MODULE_LICENSE("GPL");

