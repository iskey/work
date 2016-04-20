#include <sys/types.h>   /* for type definitions */
#include <winsock2.h>    /* for win socket API calls */
#include <ws2tcpip.h>    /* for win socket structs */
#include <stdio.h>       /* for printf() */
#include <stdlib.h>      /* for atoi() */
#include <string.h>      /* for strlen() */

#include "inttypes.h"
#include "stdint.h"

#define MAX_LEN  1024    /* maximum string size to send */
#define MIN_PORT 1024    /* minimum port allowed */
#define MAX_PORT 65535   /* maximum port allowed */

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define RAW_DATA_LEN 250
#define CMD_HEAD 0x57579090

typedef struct _BROADCAST_CMD_{
    uint32_t 	head;
    uint8_t 	cmd;
    uint16_t 	data_len;
    uint8_t 	raw_data[RAW_DATA_LEN];
    uint16_t 	crc;
}BROADCAST_CMD;

enum{
    CMD_SET_IP = 0,
    CMD_SET_NETMASK,
    CMD_SET_IP_NETMASK,
};

struct __IP_MASK__{
    uint32_t ip;
    uint32_t netmask;
};

#define INVERT_TO_BIG_ENDIAN16(val) (((uint16_t)val& (uint16_t)0xff00) >>8	| \
    ((uint16_t)val& (uint16_t)0x00ff) << 8)

#define INVERT_TO_BIG_ENDIAIN32(val) (((uint32_t)val& (uint32_t)0xff000000)>> 24		| \
    ((uint32_t)val& (uint32_t)0x00ff0000) >> 8 | \
    ((uint32_t)val& (uint32_t)0x0000ff00) << 8 | \
    ((uint32_t)val& (uint32_t)0x000000ff) << 24)

#define INVERT_TO_BIG_ENDIAN64(val) (((uint64_t) (val) & (uint64_t)(0x00000000000000ffU)) << 56) | \
    (((uint64_t)(val)& (uint64_t)(0x000000000000ff00U)) << 40) | \
    (((uint64_t)(val)& (uint64_t)(0x0000000000ff0000U)) << 24) | \
    (((uint64_t)(val)& (uint64_t)(0x00000000ff000000U)) << 8) | \
    (((uint64_t)(val)& (uint64_t)(0x000000ff00000000U)) >> 8) | \
    (((uint64_t)(val)& (uint64_t)(0x0000ff0000000000U)) >> 24) | \
    (((uint64_t)(val)& (uint64_t)(0x00ff000000000000U)) >> 40) | \
    (((uint64_t)(val)& (uint64_t)(0xff00000000000000U)) >> 56)))

#define TO_NET16(val) INVERT_TO_BIG_ENDIAN16(val)
#define TO_NET32(val) INVERT_TO_BIG_ENDIAIN32(val)
#define TO_NET64(val) INVERT_TO_BIG_ENDIAN64(val)

int isBigEndian()
{
    union MyUnion
    {
    	uint8_t a[2];
    	uint16_t b;
    };

    union MyUnion test;
    test.b = 1;
    return test.a[0];
}

void usage(char *cmd)
{
    printf("Usage:\"%s [ip] [netmask]\"\n \t e.g.: %s eth0 1.1.1.1 255.255.255.0\n", cmd, cmd);
}

#pragma comment(lib,"Ws2_32.lib") 

int send_ip_set_cmd(char* local_ip, char* remote_ip, char* remote_netmask)
{
	int sock;                   /* socket descriptor */
    char send_str[MAX_LEN];     /* string to send */
    struct sockaddr_in mc_addr; /* socket address structure */
    int send_len;               /* length of string to send */
    //char* mc_addr_str;          /* multicast IP address */
    //unsigned short mc_port;     /* multicast port */
    unsigned char mc_ttl = 1;     /* time to live (hop count) */

    struct __IP_MASK__ ip_mask;
    BROADCAST_CMD cmd;
	
	ip_mask.ip = inet_addr(remote_ip);
	ip_mask.netmask = inet_addr(remote_netmask);
   if (isBigEndian()){
		cmd.head = CMD_HEAD;
		cmd.cmd = CMD_SET_IP;
		cmd.data_len = sizeof(struct __IP_MASK__);
    }
    else{
		cmd.head = TO_NET32(CMD_HEAD);
		cmd.cmd = CMD_SET_IP;
		cmd.data_len = TO_NET16(sizeof(struct __IP_MASK__));
    }
	
    memcpy(cmd.raw_data, &ip_mask, sizeof(ip_mask));
	
	
	/* create a socket for sending to the multicast address */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket() failed");
		exit(1);
	}
	
	SOCKADDR_IN local_addr;
	local_addr.sin_addr.S_un.S_addr=inet_addr(local_ip);
	local_addr.sin_family=PF_INET;
	local_addr.sin_port=htons(15501);
	
	if(-1== bind(sock, (SOCKADDR *)& local_addr, sizeof(SOCKADDR))){
		printf("bind error.\n");
	}
	
	/* set the TTL (time to live/hop count) for the send */
	if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(const char*)&mc_ttl, sizeof(mc_ttl))) < 0) {
		perror("setsockopt() failed");
		exit(1);
	}
	
	/* construct a multicast address structure */
	memset(&mc_addr, 0, sizeof(mc_addr));
	mc_addr.sin_family = AF_INET;
	mc_addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
	mc_addr.sin_port = htons(HELLO_PORT);
	
	
	if (sendto(sock, (const char*)&cmd, sizeof(BROADCAST_CMD), 0, (struct sockaddr *) &mc_addr, sizeof(mc_addr)) < 0)
	{
		perror("sendto error.\n");
		exit(1);
	}
	

	printf("command send ok. ip addr is %s, netmask is %s\n", remote_ip, remote_netmask);
	
	
	closesocket(sock);

	return 0;
}

int main(int argc, char *argv[])
{

    char szHostName[128];
	char szT[20];
	WSADATA wsaData;            /* Windows socket DLL structure */

    /* validate number of arguments */
    if (argc != 3) {
    	usage(argv[0]);
    	exit(1);
    }

    printf("ip addr is %s, netmask is %s\n", argv[1], argv[2]);
    //mc_addr_str = argv[1];       /* arg 1: multicast IP address */
    //mc_port = atoi(argv[2]); /* arg 2: multicast port number */

	/* validate the port range */
    if ((HELLO_PORT < MIN_PORT) || (HELLO_PORT > MAX_PORT)) {
		fprintf(stderr, "Invalid port number argument %d.\n",
			HELLO_PORT);
		fprintf(stderr, "Valid range is between %d and %d.\n",
			MIN_PORT, MAX_PORT);
		exit(1);
    }
	
	/* Load Winsock 2.0 DLL */
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup() failed");
		exit(1);
	}

	if( gethostname(szHostName, 128) == 0 )
    {
        // Get host adresses
        struct hostent * pHost; 
        int i;  
        pHost = gethostbyname(szHostName); 
        for( i = 0; pHost!= NULL && pHost->h_addr_list[i]!= NULL; i++ )  
        {
			char str[100];         
			char addr[20];
			int j;         
			char * eth_name=inet_ntoa (*(struct in_addr *)pHost->h_addr_list[i]);
			send_ip_set_cmd(eth_name, argv[1], argv[2]);
        }
    }

	WSACleanup();  /* Cleanup Winsock */
	
	return 0;
}