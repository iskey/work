#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/ioctl.h>

#include <stdarg.h>
#include <ctype.h>
#include <math.h>

#include <net/if_arp.h>
#include <net/if.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

static char *get_name(char *name, char *p)
{
	/* Extract <name> from nul-terminated p where p matches
	   <name>: after leading whitespace.
	   If match is not made, set name empty and return unchanged p */
	int namestart = 0, nameend = 0;

	while (isspace(p[namestart]))
		namestart++;
	nameend = namestart;
	while (p[nameend] && p[nameend] != ':' && !isspace(p[nameend]))
		nameend++;
	if (p[nameend] == ':') {
		if ((nameend - namestart) < IFNAMSIZ) {
			memcpy(name, &p[namestart], nameend - namestart);
			name[nameend - namestart] = '\0';
			p = &p[nameend];
		} else {
			/* Interface name too large */
			name[0] = '\0';
		}
	} else {
		/* trailing ':' not found - return empty */
		name[0] = '\0';
	}
	return p + 1;
}

int get_if_list_by_proc()
{
	FILE *proc_fd;
	proc_fd= fopen("/proc/net/dev", "r");
	if(!proc_fd){
		printf("open proc system error.\n");
	}
	
	//获取网卡列表
	char buf[512];
	while (fgets(buf, sizeof buf, proc_fd)) {
		char *s, name[128];

		s = get_name(name, buf);
		printf("name is %s.\n", name);
	}	
}

int get_if_list_by_conf()
{
	int fd;
	int ret;
	struct ifreq ifr; 

	fd = socket(AF_INET, SOCK_DGRAM, 0 );
	if (fd < 0) 
	{
		printf("socket fail\n");
	}
	
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, "bond0", sizeof(ifr.ifr_name)-1);

	struct ifconf ifc;
	struct ifreq  ioctl_devs[20];
	ifc.ifc_len = sizeof(ioctl_devs);
	ifc.ifc_buf = (char*)ioctl_devs;
	int inf_count;
	int i;
	
	printf("ifc_len = %d.\n", ifc.ifc_len);
	
	ret = ioctl (fd, SIOCGIFCONF, (char*) &ifc);
	if (!ret) {
     inf_count = ifc.ifc_len / sizeof(struct ifreq);
	 printf("ifc_len=%d, size= %d. inf_count = %d.\n", ifc.ifc_len, sizeof(struct ifreq), inf_count);
	} 
	else 
	{
     close(fd);
     printf("%s(%d): ioctl(SIOCGIFCONF): %s. Are you ROOT?\n", __FILE__, __LINE__, strerror(errno));
     return -1;
	}
	
	for(i= 0; i< 5; i++)
	{
		printf("eth name is %s\n", ioctl_devs[i].ifr_name);
	}	
}

int set_if_mac(char *if_name, char *mac)
{
	int fd;
	int ret;
	struct ifreq ifr; 

	fd = socket(AF_INET, SOCK_DGRAM, 0 );
	if (fd < 0) 
	{
		printf("socket fail\n");
	}
	
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name)-1);

	  /* get flags			*/
    if (ioctl(fd,SIOCGIFFLAGS, &ifr)== 0) 
    {
        ifr.ifr_flags &= ~IFF_UP;
        /* set flags			*/
        if (ioctl(fd,SIOCSIFFLAGS, &ifr) < 0)
        {   
        	goto ERR;
        }
    }
    else
    {
    	goto ERR;
    }
	
	/*设置MAC地址*/
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = ARPHRD_ETHER;
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name)-1);
    memcpy((unsigned char *)ifr.ifr_hwaddr.sa_data, mac, 6);
    printf("Mac:%x:%x:%x:%x:%x:%x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    /* set hardware address 	*/
    ret= ioctl(fd, SIOCSIFHWADDR, &ifr);
    if (ret < 0 )
    {
        printf("set mac address error, ret=%d\n", ret);
    }

	/* get flags			*/
    if (ioctl(fd,SIOCGIFFLAGS, &ifr)== 0) 
    {
        ifr.ifr_flags |= IFF_UP;
        /* set flags			*/
        if (ioctl(fd,SIOCSIFFLAGS, &ifr) < 0)
        {   
        	goto ERR;
        }
    }
    else
    {
    	goto ERR;
    }

    close(fd);
    return 0;

ERR:
	close(fd);
	return -1;
}

int main()
{	
	char mac[6]={0x00,0x23,0x32,0x33,0x33,0x00};
	set_if_mac("eth0", mac);

	get_if_list_by_proc();
}

#if 0
static int enslave(char *master_ifname, char *slave_ifname)
{
	struct ifreq ifr;
	int res;

	if (slave.flags.ifr_flags & IFF_SLAVE) {
		bb_error_msg(
			"%s is already a slave",
			slave_ifname);
		return 1;
	}

	res = set_if_down(slave_ifname, slave.flags.ifr_flags);
	if (res)
		return res;

	if (abi_ver < 2) {
		/* Older bonding versions would panic if the slave has no IP
		 * address, so get the IP setting from the master.
		 */
		res = set_if_addr(master_ifname, slave_ifname);
		if (res) {
			bb_perror_msg("%s: can't set address", slave_ifname);
			return res;
		}
	} else {
		res = clear_if_addr(slave_ifname);
		if (res) {
			bb_perror_msg("%s: can't clear address", slave_ifname);
			return res;
		}
	}

	if (master.mtu.ifr_mtu != slave.mtu.ifr_mtu) {
		res = set_mtu(slave_ifname, master.mtu.ifr_mtu);
		if (res) {
			bb_perror_msg("%s: can't set MTU", slave_ifname);
			return res;
		}
	}

	if (hwaddr_set) {
		/* Master already has an hwaddr
		 * so set it's hwaddr to the slave
		 */
		if (abi_ver < 1) {
			/* The driver is using an old ABI, so
			 * the application sets the slave's
			 * hwaddr
			 */
			if (set_hwaddr(slave_ifname, &(master.hwaddr.ifr_hwaddr))) {
				bb_perror_msg("%s: can't set hw address",
						slave_ifname);
				goto undo_mtu;
			}

			/* For old ABI the application needs to bring the
			 * slave back up
			 */
			if (set_if_up(slave_ifname, slave.flags.ifr_flags))
				goto undo_slave_mac;
		}
		/* The driver is using a new ABI,
		 * so the driver takes care of setting
		 * the slave's hwaddr and bringing
		 * it up again
		 */
	} else {
		/* No hwaddr for master yet, so
		 * set the slave's hwaddr to it
		 */
		if (abi_ver < 1) {
			/* For old ABI, the master needs to be
			 * down before setting it's hwaddr
			 */
			if (set_if_down(master_ifname, master.flags.ifr_flags))
				goto undo_mtu;
		}

		if (set_hwaddr(master_ifname, &(slave.hwaddr.ifr_hwaddr))) {
			bb_error_msg("%s: can't set hw address",
				master_ifname);
			goto undo_mtu;
		}

		if (abi_ver < 1) {
			/* For old ABI, bring the master
			 * back up
			 */
			if (set_if_up(master_ifname, master.flags.ifr_flags))
				goto undo_master_mac;
		}

		hwaddr_set = 1;
	}

	/* Do the real thing */
	strncpy_IFNAMSIZ(ifr.ifr_slave, slave_ifname);
	if (set_ifrname_and_do_ioctl(SIOCBONDENSLAVE, &ifr, master_ifname)
	 && ioctl_on_skfd(BOND_ENSLAVE_OLD, &ifr)
	) {
		goto undo_master_mac;
	}

	return 0;

/* rollback (best effort) */
 undo_master_mac:
	set_hwaddr(master_ifname, &(master.hwaddr.ifr_hwaddr));
	hwaddr_set = 0;
	goto undo_mtu;

 undo_slave_mac:
	set_hwaddr(slave_ifname, &(slave.hwaddr.ifr_hwaddr));
 undo_mtu:
	set_mtu(slave_ifname, slave.mtu.ifr_mtu);
	return 1;
}
#endif