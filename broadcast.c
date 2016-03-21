#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <time.h>  
#include <string.h>  
#include <stdio.h>  
#include <unistd.h>  
  
#define HELLO_PORT 12345  
#define HELLO_GROUP "225.0.0.37"  
#define RAW_DATA_LEN 250
#define HEAD 0x57579090

typedef struct _BROADCAST_CMD_{
	unsigned short head;
	unsigned char cmd;
	unsigned short data_len;
	unsigned char raw_data[RAW_DATA_LEN];
	unsigned short crc;
}BROADCAST_CMD;

enum{
	CMD_SET_IP= 0,
	CMD_SET_NETMASK,
	CMD_SET_IP_NETMASK,
};

struct __IP_MASK__{
	unsigned long ip;
	unsigned long netmask;
};
  
int main(int argc, char *argv[])  
{  
	struct sockaddr_in addr;  
	int fd, cnt;  
	struct ip_mreq mreq;  
	char *message="Hello, World!";  
	
	printf("ip addr is %s, netmask is %s\n", argv[1], argv[2]);
	
	struct __IP_MASK__ ip_mask;
	BROADCAST_CMD cmd;
	
	ip_mask.ip= inet_addr(argv[1]);
	ip_mask.netmask= inet_addr(argv[2]);
	
	cmd.head = HEAD;
	cmd.cmd= CMD_SET_IP;
	cmd.data_len= sizeof(struct __IP_MASK__);
	
	memcpy(cmd.raw_data, &ip_mask, sizeof(ip_mask));
	
  
	/* create what looks like an ordinary UDP socket */  
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)   
	{  
		perror("socket");  
		exit(1);
	}  
  
	/* set up destination address */  
	memset(&addr,0,sizeof(addr));  
	addr.sin_family=AF_INET;  
	addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);  
	addr.sin_port=htons(HELLO_PORT);  
  
	/* now just sendto() our destination! */  
	while (1)  
	{  
		if (sendto(fd,&cmd, sizeof(BROADCAST_CMD), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)   
		{  
			perror("sendto");  
			exit(1);  
		}  
		sleep(1);  
	}  
}  





















#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <time.h>  
#include <string.h>  
#include <stdio.h>  
#include <unistd.h>  
  
#define HELLO_PORT  12345  
#define HELLO_GROUP "225.0.0.37"  
#define MSGBUFSIZE 256  
#define RAW_DATA_LEN 250
#define HEAD 0x57579090

typedef struct _BROADCAST_CMD_{
	unsigned long head;
	unsigned char cmd;
	unsigned short data_len;
	unsigned char raw_data[RAW_DATA_LEN];
	unsigned short crc;
}BROADCAST_CMD;

enum{
	CMD_SET_IP= 0,
	CMD_SET_NETMASK,
	CMD_SET_IP_NETMASK,
};

struct __IP_MASK__{
	unsigned long ip;
	unsigned long netmask;
};
  

int main(int argc, char *argv[])  
{  
	struct sockaddr_in addr;  
	int fd, nbytes,addrlen;  
	struct ip_mreq mreq;  
	char msgbuf[MSGBUFSIZE];  
	BROADCAST_CMD *cmd;
	struct __IP_MASK__ *ip_mask;
	struct in_addr ip,netmask;
  
	u_int yes=1; /*** MODIFICATION TO ORIGINAL */  
  
	/* create what looks like an ordinary UDP socket */  
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)   
	{  
		perror("socket");  
		exit(1);  
	}  
  
  
	/**** MODIFICATION TO ORIGINAL */  
	/* allow multiple sockets to use the same PORT number */  
	if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)   
	{  
		perror("Reusing ADDR failed");  
		exit(1);  
	}  
	/*** END OF MODIFICATION TO ORIGINAL */  
  
	/* set up destination address */  
	memset(&addr,0,sizeof(addr));  
	addr.sin_family=AF_INET;  
	addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */  
	addr.sin_port=htons(HELLO_PORT);  
  
	/* bind to receive address */  
	if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0)  
	{  
		perror("bind");  
		exit(1);  
	}  
  
	/* use setsockopt() to request that the kernel join a multicast group */  
	mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);  
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);  
	if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)   
	{  
		perror("setsockopt");  
		exit(1);  
	}  
  
	/* now just enter a read-print loop */  
	while (1)   
	{  
		addrlen=sizeof(addr);  
		if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0, (struct sockaddr *) &addr,(socklen_t*)&addrlen)) < 0)   
		{  
			perror("recvfrom");  
			exit(1);  
		}  
		cmd= (BROADCAST_CMD*) msgbuf;
		ip_mask= cmd->raw_data;
		
		printf("cmd->head is 0x%08x\n", (cmd->head));
		printf("cmd->cmd is 0x%08x\n", (cmd->cmd));
		
		//printf("ip_mask->ip is 0x%08x\n", (ip_mask->ip));
		//printf("ip_mask->netmask is 0x%08x\n", (ip_mask->netmask));
		
		memcpy(&ip, &(ip_mask->ip), 4);
		memcpy(&netmask, &(ip_mask->netmask), 4);
		//printf("ip_mask->ip is %s\n", inet_ntoa(ip));
		//printf("ip_mask->netmask is %s\n", inet_ntoa(netmask));
		
		unsigned char s_ip[50], s_netmask[50], exe_cmd[300];
		snprintf(s_ip, 50, "%s", inet_ntoa(ip));
		snprintf(s_netmask, 50, "%s", inet_ntoa(netmask));
		
		snprintf(exe_cmd, 300, "ifconfig %s %s netmask %s", argv[1], s_ip, s_netmask);
		printf("command is %s\n", exe_cmd);
	}  
  
	return 0;  
}  
