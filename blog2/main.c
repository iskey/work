#include "blog.h"

int main(int argc, char* argv[])
{
	blog_init("./blog.log", NULL);
	blog(LOG_ERR, "This is a blog() errno message.");
	blog(LOG_INF, "This is a blog() info message.")
	printf("This is a printf() message.\n");
	fprintf(stderr, "This is a stderr message.\n");
	fprintf(stdout, "This is a stdout message.\n");
	return 0;
}