#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "blog.h"

int blog_destroy(void);
FILE *g_blog_stdout, *g_blog_old_stdout;
FILE *g_blog_errout, *g_blog_old_errout;

/*
 *@stdout_file:重定向原来的标准输出到@stdout_file文件。
 *@blog_file:blog日志重定向到@blog_file文件，如果为NULL则重定向到标准输出
 **/
int blog_init(char *stdout_file, char *blog_file)
{
	int blog_stdout_fd= 1000, blog_errout_fd= 1001;
	int blog_old_stdout_fd, blog_old_errout_fd;
	int ret;

	/*重定向blog日志到标准输出.*/
	if(NULL== blog_file)
	{
		ret= dup2(1, blog_stdout_fd);
		if(-1== ret){
			fprintf(stderr, "Failed to duplicate blog stdout fd. --%s.\n",strerror(errno));
			goto err;
		}

		g_blog_stdout= fdopen(blog_stdout_fd, "w+");
		if(NULL== g_blog_stdout){
			fprintf(stderr, "Failed to open blog stdout FILE. --%s.\n",strerror(errno));
			goto err;
		}

		/*重定向blog errout到标准输出.*/
		ret= dup2(2, blog_errout_fd);
		if(-1== ret){
			fprintf(stderr, "Failed to duplicate blog errout fd. --%s.\n",strerror(errno));
			goto err;
		}

		g_blog_errout= fdopen(blog_errout_fd, "w+");
		if(NULL== g_blog_errout){
			fprintf(stderr, "Failed to open blog errout FILE. --%s.\n",strerror(errno));
			goto err;
		}


	}
	else
	{
		/*将blog日志输出到文件*/
	}

	/*重定向原来的标准输出到日志文件.*/
	if(NULL!= stdout_file)
	{
		/*关闭标准输出*/
		close(1);
		close(2);

		g_blog_old_stdout= fopen(stdout_file, "a");
		if(NULL== g_blog_old_stdout){
			fprintf(stderr, "Failed to open origin log file.\n");
			goto err;
		}	

		blog_old_stdout_fd= fileno(g_blog_old_stdout);
		ret= dup2(blog_old_stdout_fd, 1);
		if(-1== ret){
			fprintf(stderr, "failed to duplicate blog old stdout. --%s.\n",strerror(errno));
			goto err;
		}

		ret= dup2(blog_old_stdout_fd, 2);
		if(-1== ret){
			fprintf(stderr, "failed to duplicate blog old errout. --%s.\n",strerror(errno));
			goto err;
		}
	}

	return 0;
err:
	return -1;
}