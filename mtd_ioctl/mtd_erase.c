#include "stdio.h"
#include "string.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include "errno.h"
#include "mtd-abi.h"
#include "stdlib.h"

#define RELEASE
#ifdef RELEASE
#define BLOG_STR_(a,b,c) a#b"\n"
#else
#define BLOG_STR_(a,b,c) a#b c"\n"
#endif

#define LOG_NOW	LOG_INF
#define LOG_DBG	20
#define LOG_INF	40
#define LOG_ERR	100

#if (LOG_NOW<=LOG_ERR)
    #define _LOG_ERR(format,...) fprintf(stderr, BLOG_STR_("LOG_ERR: ",format,"     ---- FILE:%s, %s():%d."), ##__VA_ARGS__,__FILE__,__func__,__LINE__);
#else
    #define _LOG_ERR(format,...)
#endif
#if (LOG_NOW<=LOG_INF)
    #define _LOG_INF(format,...) fprintf(stderr, BLOG_STR_("LOG_INF: ",format,"     ---- FILE:%s, %s():%d."), ##__VA_ARGS__,__FILE__,__func__,__LINE__);
#else
    #define _LOG_INF(format,...)
#endif
#if (LOG_NOW<=LOG_DBG)
    #define _LOG_DBG(format,...) fprintf(stderr, BLOG_STR_("LOG_DBG: ",format,"     ---- FILE:%s, %s():%d."), ##__VA_ARGS__,__FILE__,__func__,__LINE__);
#else
    #define _LOG_DBG(format,...)
#endif

#define blog(level, format, ...) _##level(format, ##__VA_ARGS__);

#define MTD_DEV "/dev/mtd2"

typedef int INT;
typedef int WORD32;

//#define BOOL int
typedef enum _BOOL_{
	FALSE,
	TRUE
}BOOL;

#if 0
struct mtd_info_user {
	unsigned char type;
	unsigned long flags;
	unsigned long size;	 // Total size of the MTD
	unsigned long erasesize;
	unsigned long oobblock;  // Size of OOB blocks (e.g. 512)
	unsigned long oobsize;   // Amount of OOB data per block (e.g. 16)
	unsigned long ecctype;
	unsigned long eccsize;
};

struct erase_info_user {
	unsigned int start;
	unsigned int length;
};

#define MEMGETINFO              _IOR('M', 1, struct mtd_info_user)
#define MEMERASE                _IOW('M', 2, struct erase_info_user)
#define MEMWRITEOOB             _IOWR('M', 3, struct mtd_oob_buf)
#define MEMREADOOB              _IOWR('M', 4, struct mtd_oob_buf)
#define MEMLOCK                 _IOW('M', 5, struct erase_info_user)
#define MEMUNLOCK               _IOW('M', 6, struct erase_info_user)
#define MEMGETREGIONCOUNT	_IOR('M', 7, int)
#define MEMGETREGIONINFO	_IOWR('M', 8, struct region_info_user)
#define MEMSETOOBSEL		_IOW('M', 9, struct nand_oobinfo)
#define MEMGETOOBSEL		_IOR('M', 10, struct nand_oobinfo)
#define MEMGETBADBLOCK		_IOW('M', 11, loff_t)
#define MEMSETBADBLOCK		_IOW('M', 12, loff_t)
#define OTPSELECT		_IOR('M', 13, int)
#define OTPGETREGIONCOUNT	_IOW('M', 14, int)
#define OTPGETREGIONINFO	_IOW('M', 15, struct otp_info)
#define OTPLOCK		_IOR('M', 16, struct otp_info)

#endif
BOOL fs_common_EraseFlash(INT swFd, WORD32 dwOffset, WORD32 dwBytes, WORD32 dwBlockSize)
{
   INT err;
   struct erase_info_user erase;

   if(dwBlockSize<= 0)/* 整体擦除 */
   {
       erase.start  = dwOffset;
       erase.length = dwBytes;

       err= ioctl(swFd, MEMERASE, &erase);
       if(err)
       {
           blog(LOG_ERR,"memerase error,err:%d \n",err);
           return FALSE;
       }
   }
   else/* 分块擦除 */
   {
       WORD32 eraseBlock= dwBytes/ dwBlockSize;
       WORD32 eraseCnt= 0;
       erase.length= dwBlockSize;

       while(eraseCnt<= eraseBlock)
       {
            erase.start= (eraseCnt++)* dwBlockSize;

            #if 0
            /* 查询是否是坏块 */
            err = ioctl(swFd, MEMGETBADBLOCK, &(erase.start));
            if (err == -1){
                blog(LOG_ERR,"Skipped bad block at Page 0x%0x.\n",erase.start);
                continue;
            }
            #endif

            /* 擦除块 */
            err= ioctl(swFd, MEMERASE, &erase);
            if(err)
            {
                #if 0 
                blog(LOG_ERR,"Erase block failed at Page 0x%0x. Maybe it is a bad block not taged.\n",erase.start);
                return FALSE;
                #endif
                blog(LOG_ERR,"Skipped block at Page 0x%0x.\n",erase.start);
                continue;
            }
       }
   }
   
   return TRUE;
}


int main(int argc, char* argv[])
{
	int fd, fd2;
	BOOL ret;
	int io_ret;

	blog(LOG_DBG, "MEMGETINFO is :0x%0x.", MEMGETINFO);
	blog(LOG_DBG, "MEMERASE is :0x%0x.", MEMERASE);

	if((fd= open(MTD_DEV, O_RDWR))< 0)
	{
		blog(LOG_ERR, "open %s error.", MTD_DEV);
		goto err;
	}
  blog(LOG_DBG, "mtd device fd is %d.", fd);

	struct mtd_info_user *meminfo;
  meminfo= malloc(sizeof(struct mtd_info_user));
	memset(meminfo, 0, sizeof(meminfo));
	io_ret= ioctl(fd, MEMGETINFO, meminfo);
	if(io_ret!= 0)
	{
		blog(LOG_ERR, "ioctl cmd MEMGETINFO error. errno:%d.", errno);
		goto err;
	}	

	blog(LOG_INF, "Erase size is %dK, block size is %dK.", meminfo->erasesize/1024, meminfo->size/1024/1024);
	ret= fs_common_EraseFlash(fd, 0, meminfo->erasesize, meminfo->size);
	if(ret!= TRUE){
		blog(LOG_ERR, "erase flash error.");
		goto err;
	}

	return 0;
err:
	close(fd);
	return -1;
}
