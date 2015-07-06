#include "stdio.h"
#include "string.h"
#include "errno.h"

int main(int argc, char* argv[])
{
	FILE *my_stdout, *org_stdout;
	int my_stdout_fd=1000, org_stdout_fd;
	int ret;
#if 0
	/*重定向my_stdout到标准输出.*/
	ret= dup2(1, my_stdout_fd);
	if(-1== ret){
		fprintf(stderr, "failed to duplicate org stdout. --%s.\n",strerror(errno));
		goto err;
	}

	my_stdout= fdopen(my_stdout_fd, "w+");
	if(NULL== my_stdout){
		fprintf(stderr, "failed to open my_stdout_fd. --%s.\n",strerror(errno));
		goto err;
	}

	/*关闭标准输出*/
	close(1);	
#endif
	/*重定向原来的标准输出到日志文件.*/
	org_stdout= fopen("./t.log", "w+");
	if(NULL== org_stdout){
		fprintf(stderr, "failed to open log file.\n");
		goto err;
	}	

	org_stdout_fd= fileno(org_stdout);
	//ret= dup2(org_stdout_fd, 1);
	ret= dup2(1, org_stdout_fd);
	if(-1== ret){
		fprintf(stderr, "failed to duplicate my stdout. --%s.\n",strerror(errno));
		goto err;
	}

	//fprintf(my_stdout, "this will printf to my standard output.\n");
	//fprintf(org_stdout, "this will printf to log file.\n");
	printf("this is standard printf() messages.\n");
	
	system("echo fdsafdsa");

	return 0;
err:
	return -1;
}
